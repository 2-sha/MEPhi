#pragma once

#include <string>
#include <list>

const size_t SIZE = 32;

struct Version
{
	size_t ver;
	std::string info;
	Version(const size_t ver, const std::string &info) : ver(ver), info(info) {};
	Version() {};
};

struct Item
{
	size_t hopKey, sumKey;
	size_t sumId;
	std::list<Version> info;

	Item(const size_t hopKey, const size_t sumKey, const std::string &info)
		: hopKey(hopKey), sumKey(sumKey)
	{
		this->info.push_back(Version(1, info));
	};
	Item() {};

	void add(const std::string &info)
	{
		this->info.push_back(Version(this->info.back().ver + 1, info));
	}
};

int calcHash(const size_t key)
{
	return key % SIZE;
}