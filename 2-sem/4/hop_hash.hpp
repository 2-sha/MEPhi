#pragma once

#include <bitset>
#include <string>
#include <exception>
#include "table.hpp"

namespace hop_hash
{

	struct Bucket
	{
		std::bitset<BUCKET_SIZE> bitmap;
		size_t key;
		Item *item = nullptr;
	};

	Bucket table[SIZE];

	void add(const size_t key, Item *item)
	{
		size_t h = calcHash(key);

		// Ищем пустую ячейку
		size_t i = h;
		while (table[i].item != nullptr)
		{
			if (table[i].bitmap.all())
				throw new std::overflow_error("Бакет заполнен");

			size_t j = 0;
			while (table[i].bitmap.test(j))
				j++;
			i += j;
			i %= SIZE;
		}

		// Добавляем новый элемент
		table[i].item = item;
		table[i].key = item->hopKey;

		// Перемещаем его по возможности назад
		size_t dist = (SIZE + i - h) % SIZE;
		while (dist >= BUCKET_SIZE)
		{
			size_t tmp = (SIZE + i - (BUCKET_SIZE - 1)) % SIZE;

			if (table[tmp].bitmap.all())
				throw new std::overflow_error("Бакет заполнен");

			size_t j = 0;
			while (!table[tmp].bitmap.test(j))
				j++;

			std::swap(table[tmp].item, table[i].item);
			std::swap(table[tmp].key, table[i].key);
			table[tmp].bitmap[j] = 0;
			table[tmp].bitmap[BUCKET_SIZE - 1] = 1;

			i = tmp;
			dist = (SIZE + i - h) % SIZE;
		}

		// Обновляем bitmap бакета, в который поместили элемент
		table[h].bitmap[dist] = 1;
	}

	Item *find(const size_t key)
	{
		size_t h = calcHash(key);

		if (table[h].item == nullptr)
			return nullptr;

		for (size_t i = 0; i < BUCKET_SIZE; i++)
		{
			size_t offset = (h + i) % SIZE;
			if (table[h].bitmap[i] && table[offset].key == key)
				return table[offset].item;
		}
		return nullptr;
	}

	void remove(const size_t key)
	{
		size_t h = calcHash(key);

		if (table[h].item == nullptr)
			throw std::out_of_range("Указанный элемент не найден");

		for (size_t i = 0; i < BUCKET_SIZE; i++)
		{
			size_t offset = (h + i) % SIZE;
			if (table[h].bitmap[i] && table[offset].key == key)
			{
				table[offset].item = nullptr;
				table[h].bitmap[i] = 0;
				return;
			}
		}
		throw std::out_of_range("Указанный элемент не найден");
	}

}