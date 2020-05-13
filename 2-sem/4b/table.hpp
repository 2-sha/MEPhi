#pragma once

#include <string>
#include <fstream>
#include <list>

const int SIZE = 32;
const int SUM_STEP = 3;
const int BUCKET_SIZE = 4;
const char* SUM_FILE = "sum_hash";
const char* HOP_FILE = "hop_hash";

struct Version
{
	int ver;
	std::string info;
	Version(const int ver, const std::string& info) : ver(ver), info(info) {};
	Version() {};

	void read(std::fstream& file)
	{
		file.read((char*)&this->ver, sizeof this->ver);
		int infoSize = 0;
		file.read((char*)&infoSize, sizeof infoSize);
		char* buffer = new char[infoSize + 1];
		file.read(buffer, infoSize);
		buffer[infoSize] = 0;
		this->info = buffer;
		delete[] buffer;
	}

	void write(std::fstream& file)
	{
		file.write((char*)&this->ver, sizeof this->ver);
		int infoSize = this->info.size();
		file.write((char*)&infoSize, sizeof infoSize);
		const char* info = this->info.c_str();
		file.write(info, infoSize);
	}
};

struct Item
{
	int hopKey, sumKey;
	std::list<Version> info;

	Item(const int hopKey, const int sumKey, const std::string& info)
		: hopKey(hopKey), sumKey(sumKey)
	{
		this->info.push_back(Version(1, info));
	};
	Item() {};

	void add(const std::string& info)
	{
		this->info.push_back(Version(this->info.back().ver + 1, info));
	}

	void read(std::fstream& file)
	{
		file.read((char*)&this->hopKey, sizeof this->hopKey);
		file.read((char*)&this->sumKey, sizeof this->sumKey);
		int infoLen = 0;
		file.read((char*)&infoLen, sizeof infoLen);
		this->info.clear();
		for (int i = 0; i < infoLen; i++)
		{
			Version ver;
			ver.read(file);
			this->info.push_back(ver);
		}
	}
	
	void write(std::fstream& file)
	{
		file.write((char*)&this->hopKey, sizeof this->hopKey);
		file.write((char*)&this->sumKey, sizeof this->sumKey);
		int infoLen = this->info.size();
		file.write((char*)&infoLen, sizeof infoLen);
		for (auto it : this->info)
			it.write(file);
	}
};

int calcHash(const int key)
{
	return key % SIZE;
}

template <class Bucket>
std::fstream openFile(const char* name)
{
	std::fstream file(name, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open() || file.peek() == EOF)
	{
		file.close();
		file.open(name, std::ios::binary | std::ios::app);
		
		file.seekg(0, std::ios::beg);
			Bucket tmp;
			for (int i = 0; i < SIZE; i++)
			{
				tmp.id = i;
				tmp.write(file);
			}
			file.clear();
			file.seekg(0, std::ios::beg);

		file.close();
		file.open(name, std::ios::binary | std::ios::in | std::ios::out);
	}
	if (!file.is_open())
		throw std::invalid_argument("Не удаётся открыть файл");

	return file;
}

template <class Bucket>
void updateFile(std::fstream& file, const int pos, Bucket elem)
{
	int ppos = file.tellg();
	file.seekp(0, std::ios::end);
	int len = -ppos + file.tellg();
	char* buffer = 0;
	if (len > 0)
	{
		file.seekg(ppos);
		buffer = new char[len];
		file.read(buffer, len);
	}

	file.seekg(pos);
	elem.write(file);

	if (len > 0)
		file.write(buffer, len);
}