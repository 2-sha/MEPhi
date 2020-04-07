#pragma once

#include <string>
#include <exception>
#include "table.hpp"

namespace sum_hash
{
	const size_t SUM_STEP = 3;

	Item *table[SIZE];

	void add(const size_t key, Item *item)
	{
		size_t i, h, counter = 0;
		i = h = calcHash(key);

		while (table[i] != nullptr && table[i]->sumKey != key)
		{
			i = (i + SUM_STEP) % SIZE;
			counter++;
			if (i == h)
				throw new std::overflow_error("Таблица заполнена!");
		}

		table[i] = item;
	}

	Item *find(const size_t key)
	{
		size_t i, h;
		h = i = calcHash(key);

		while (table[i] != nullptr && table[i]->sumKey != key)
		{
			i = (i + SUM_STEP) % SIZE;
			if (i == h)
				return nullptr;
		}

		return table[i];
	}

	void remove(const size_t key)
	{
		size_t i, h;
		h = i = calcHash(key);

		while (table[i] != nullptr && table[i]->sumKey != key)
		{
			i = (i + SUM_STEP) % SIZE;
			if (i == h)
				throw std::out_of_range("Указанный элемент не найден!");
		}

		if (table[i] == nullptr)
			throw std::out_of_range("Указанный элемент не найден!");
		table[i] = nullptr;
	}

}