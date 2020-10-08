﻿#pragma once
#include <utility>
#include <ostream>
#include <initializer_list>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <string>

class Bone
{
private:
	int left, right;

public:
	static constexpr int MAX_POINTS = 6;

	bool operator==(const Bone& bone) const;
	bool operator!=(const Bone& bone) const;

	friend std::ostream& operator<< (std::ostream& out, const Bone& bone);
	friend std::istream& operator>> (std::istream& in, Bone& bone);

	std::pair<int, int> get();
	void set(int left, int right);

	static Bone genetate();
	Bone(int left = 0, int right = 0);
};

class Domino
{
public:
	static constexpr int MAX_BONES = 28;

	Domino(int num = 0);
	Domino(const std::initializer_list<Bone>& list);

	int size() const;

	friend std::ostream& operator<< (std::ostream& out, const Domino& domino);
	friend std::istream& operator>> (std::istream& in, Domino& point);

	Domino& operator++();
	Domino operator++(int);
	Domino& operator-=(const Bone& bone);

	const Bone& operator[](int n) const;
	Bone& operator[](int n);

private:
	Bone bones[MAX_BONES];
	int bonesNum;

	/*
	При помощи ГСПЧ генерирует Bone, которого ещё нет в массиве
	Если массив уже заполнен, то бросает overflow_error
	*/
	Bone generateBone();
};