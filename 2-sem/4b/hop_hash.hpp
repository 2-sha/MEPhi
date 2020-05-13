#pragma once

#include <bitset>
#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include "table.hpp"

#if BUCKET_SIZE > 64
	#error "BUCKET_SZIE должен быть от 1 до 64, чтобы уместиться в uint64_t"
#endif

namespace hop_hash
{

	struct Bucket
	{
		std::bitset<BUCKET_SIZE> bitmap = 0;
		int key = -1, id = -1, pos = -1;
		bool isEmpty = true;
		static const int size = 21;

		void read(std::fstream &file)
		{
			file.read((char*)&this->key, sizeof this->key);
			file.read((char*)&this->id, sizeof this->id);
			file.read((char*)&this->pos, sizeof this->pos);
			file.read((char*)&this->isEmpty, sizeof this->isEmpty);
			unsigned long long bits = 0;
			file.read((char*)&bits, sizeof bits);
			bitmap = bits;
		}

		void write(std::fstream& file)
		{
			file.write((char*)&this->key, sizeof this->key);
			file.write((char*)&this->id, sizeof this->id);
			file.write((char*)&this->pos, sizeof this->pos);
			file.write((char*)&this->isEmpty, sizeof this->isEmpty);
			unsigned long long bits = bitmap.to_ullong();
			file.write((char*)&bits, sizeof bits);
		}
	};

	Bucket table[SIZE];

	void add(const int key, Item* item)
	{
		int i, h, indexPos, dataPos;
		Bucket index;
		i = h = calcHash(key);

		std::fstream file = openFile<Bucket>(HOP_FILE);

		// Ищем пустую ячейку
		indexPos = i * Bucket::size;
		file.seekg(indexPos);
		index.read(file);
		while (!index.isEmpty)
		{
			if (index.bitmap.all())
				throw std::overflow_error("Бакет заполнен");

			int j = 0;
			while (index.bitmap.test(j))
				j++;
			i = (i + j) % SIZE;
			indexPos = i * Bucket::size;
			file.seekg(indexPos);
			index.read(file);
		}

		// Добавляем динамический элемент
		file.seekp(0, std::ios::end);
		dataPos = file.tellg();
		item->write(file);

		// Добавляем статический элемент
		file.seekg(indexPos);
		index.pos = dataPos;
		index.isEmpty = false;
		index.key = key;
		index.write(file);

		// Перемещаем его по возможности назад
		int dist = (SIZE + i - h) % SIZE;
		while (dist >= BUCKET_SIZE)
		{
			Bucket tmpIndex;
			int tmp = (SIZE + i - (BUCKET_SIZE - 1)) % SIZE;

			int tmpIndexPos = tmp * Bucket::size;
			file.seekg(tmpIndexPos);
			tmpIndex.read(file);

			if (tmpIndex.bitmap.all())
				throw std::overflow_error("Всё, приплыли");

			int j = 0;
			while (!tmpIndex.bitmap.test(j))
				j++;

			std::swap(tmpIndex.pos, index.pos);
			std::swap(tmpIndex.key, index.key);
			std::swap(tmpIndex.isEmpty, index.isEmpty);
			tmpIndex.bitmap[j] = 0;
			tmpIndex.bitmap[BUCKET_SIZE - 1] = 1;

			file.seekg(tmpIndexPos);
			tmpIndex.write(file);

			file.seekg(indexPos);
			index.write(file);

			i = tmp;
			index = tmpIndex;
			indexPos = tmpIndexPos;
			dist = (SIZE + i - h) % SIZE;
		}

		// Обновляем bitmap бакета, в который поместили элемент
		indexPos = h * Bucket::size;
		file.seekg(indexPos);
		index.read(file);
		index.bitmap[dist] = 1;
		file.seekg(indexPos);
		index.write(file);
		file.close();
	}

	Item* find(const int key)
	{
		int h, indexPos;
		Bucket index, indexOffset;
		h = calcHash(key);

		std::fstream file = openFile<Bucket>(HOP_FILE);

		indexPos = h * Bucket::size;
		file.seekg(indexPos);
		index.read(file);
		if (index.isEmpty)
			return nullptr;

		for (int i = 0; i < BUCKET_SIZE; i++)
		{
			int offset = (h + i) % SIZE;
			file.seekg(offset * Bucket::size);
			indexOffset.read(file);
			if (index.bitmap[i] && indexOffset.key == key)
			{
				file.seekg(indexOffset.pos);
				Item* item = new Item;
				item->read(file);
				return item;
			}
		}
		return nullptr;
	}

	void remove(const int key)
	{
		int h, dataPos;
		Bucket index, indexOffset;
		h = calcHash(key);
		
		std::fstream file = openFile<Bucket>(HOP_FILE);

		file.seekg(h * Bucket::size);
		index.read(file);
		if (index.isEmpty)
			throw std::out_of_range("Указанный элемент не найден");

		for (int i = 0; i < BUCKET_SIZE; i++)
		{
			int offset = (h + i) % SIZE;
			file.seekg(offset * Bucket::size);
			indexOffset.read(file);
			if (index.bitmap[i] && indexOffset.key == key)
			{
				dataPos = indexOffset.pos;
				file.seekp(0, std::ios::end);
				int endPos, s;
				s = endPos = file.tellg();

				file.seekg(dataPos);
				Item item;
				item.read(file);
				s -= file.tellg();
				if (s > 0)
				{
					char* buffer = new char[s];
					file.read(buffer, s);
					file.seekg(dataPos);
					file.write(buffer, s);
				}
				else
					file.seekg(dataPos);
				file.sync();

				// Обрезаем лишнее
				std::filesystem::resize_file(std::filesystem::path(HOP_FILE), (int)file.tellg());
				s = endPos - file.tellg();

				file.sync();
				file.seekg(offset * Bucket::size);
				indexOffset.isEmpty = true;
				indexOffset.pos = indexOffset.key = -1;
				if (i == 0)
					indexOffset.bitmap[i] = 0;
				indexOffset.write(file);

				if (i != 0)
				{
					file.seekg(h * Bucket::size);
					index.bitmap[i] = 0;
					index.write(file);
				}

				// Сдвигаем pos у остальных индексов
				if (s > 0)
				{
					file.seekg(0, std::ios::beg);
					int indexPos = 0;
					for (int j = 0; j < SIZE; j++)
					{
						indexPos = file.tellg();
						index.read(file);
						if (index.pos > dataPos)
						{
							index.pos -= s;
							file.seekg(indexPos);
							index.write(file);
						}
						file.sync();
					}
				}
				file.close();
				return;
			}
		}
		throw std::out_of_range("Указанный элемент не найден");
	}

	void update(const int key, Item* item)
	{
		hop_hash::remove(key);
		hop_hash::add(key, item);
	}

}