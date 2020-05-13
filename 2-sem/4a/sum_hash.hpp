#pragma once

#include <string>
#include <exception>
#include "table.hpp"

namespace sum_hash
{

	struct Bucket
	{
		size_t sumId, key;
		bool isDeleted;
		Item *item = nullptr;

		Bucket(const size_t key, Item *item) :
			key(key), item(item), isDeleted(false), sumId(0) {};
		Bucket() {};
	};

	Bucket table[SIZE];

	void add(const size_t key, Item *item)
	{
		size_t i, h, counter = 0;
		i = h = calcHash(key);

		while ((table[i].item != nullptr || table[i].isDeleted) && table[i].key != key)
		{
			i = (i + SUM_STEP) % SIZE;
			counter++;
			if (i == h)
				throw new std::overflow_error("Таблица заполнена!");
		}

		table[i] = Bucket(key, item);
	}

	Item *find(const size_t key)
	{
		size_t i, h;
		h = i = calcHash(key);

		while ((table[i].item != nullptr || table[i].isDeleted) && table[i].key != key)
		{
			i = (i + SUM_STEP) % SIZE;
			if (i == h)
				return nullptr;
		}

		return table[i].item;
	}

	void remove(const size_t key)
	{
		size_t i, h;
		h = i = calcHash(key);

		while ((table[i].item != nullptr || table[i].isDeleted) && table[i].key != key)
		{
			i = (i + SUM_STEP) % SIZE;
			if (i == h)
				throw std::out_of_range("Указанный элемент не найден!");
		}

		if (table[i].item == nullptr)
			throw std::out_of_range("Указанный элемент не найден!");
		table[i].item = nullptr;
		table[i].isDeleted = true;
	}

}