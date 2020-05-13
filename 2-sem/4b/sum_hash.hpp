#pragma once

#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include "table.hpp"

namespace sum_hash
{

	struct Bucket
	{
		/*
		-1 Ц удалЄн
		0  Ц пуст
		1  Ц заполнен
		*/
		int key = -1, id = -1, state = 0, pos = -1;
		static const int size = 16;

		void read(std::fstream& file)
		{
			file.read((char*)&this->key, sizeof this->key);
			file.read((char*)&this->state, sizeof this->state);
			file.read((char*)&this->id, sizeof this->id);
			file.read((char*)&this->pos, sizeof this->pos);
		}

		void write(std::fstream& file)
		{
			file.write((char*)&this->key, sizeof this->key);
			file.write((char*)&this->state, sizeof this->state);
			file.write((char*)&this->id, sizeof this->id);
			file.write((char*)&this->pos, sizeof this->pos);
		}

	};

	void add(const int key, Item* item)
	{
		int i, h, indexPos, dataPos;
		Bucket index;

		i = h = calcHash(key);
		std::fstream file = openFile<Bucket>(SUM_FILE);

		indexPos = i * Bucket::size;
		file.seekg(indexPos);
		index.read(file);
		while (index.state == 1 && index.key != key)
		{
			i = (i + SUM_STEP) % SIZE;
			indexPos = i * Bucket::size;
			file.seekg(indexPos);
			index.read(file);
			if (i == h)
				throw std::overflow_error("“аблица заполнена!");
		}

		file.seekp(0, std::ios::end);
		dataPos = file.tellg();
		item->write(file);

		file.seekg(indexPos);
		index.pos = dataPos;
		index.state = 1;
		index.key = key;
		index.write(file);

		file.close();
	}

	Item* find(const int key)
	{
		int i, h, indexPos;
		Bucket index;

		h = i = calcHash(key);
		std::fstream file = openFile<Bucket>(SUM_FILE);

		indexPos = i * Bucket::size;
		file.seekg(indexPos);
		index.read(file);
		while (index.state != 0 && index.key != key)
		{
			i = (i + SUM_STEP) % SIZE;
			indexPos = i * Bucket::size;
			file.seekg(indexPos);
			index.read(file);
			if (i == h)
				return nullptr;
		}

		if (index.state == 0)
		{
			file.close();
			return nullptr;
		}
		file.seekg(index.pos);
		Item *item = new Item;
		item->read(file);
		return item;
	}

	void remove(const int key)
	{
		int i, h, indexPos, dataPos;
		Bucket index;

		h = i = calcHash(key);
		std::fstream file = openFile<Bucket>(SUM_FILE);

		indexPos = i * Bucket::size;
		file.seekg(indexPos);
		index.read(file);
		while (index.state != 0 && index.key != key)
		{
			i = (i + SUM_STEP) % SIZE;
			indexPos = i * Bucket::size;
			file.seekg(indexPos);
			index.read(file);
			if (i == h)
				throw std::out_of_range("”казанный элемент не найден!");
		}

		if (index.state != 1)
			throw std::out_of_range("”казанный элемент не найден!");

		dataPos = index.pos;
		file.seekp(0, std::ios::end);
		int endPos, s;
		s = endPos = file.tellg();

		file.seekg(dataPos);
		Item item;
		item.read(file);
		s -= file.tellg();
		char* buffer = new char[s];
		file.read(buffer, s);
		file.seekg(dataPos);
		file.write(buffer, s);
		file.sync();

		// ќбрезаем лишнее
		std::filesystem::resize_file(std::filesystem::path(SUM_FILE), (int)file.tellg());
		s = endPos - file.tellg();

		// ћен€ем pos у индексов
		file.sync();
		file.seekg(indexPos);
		index.pos = index.state = index.key = -1;
		index.write(file);

		file.seekg(0, std::ios::beg);
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

		file.close();
	}

	void update(const int key, Item* item)
	{
		sum_hash::remove(key);
		sum_hash::add(key, item);
		sum_hash::find(1);
		sum_hash::find(2);
	}

}