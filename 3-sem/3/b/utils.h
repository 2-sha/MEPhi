#pragma once
#include <utility>
#include <ostream>
#include <initializer_list>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <string>

namespace lab3
{
	class Bone
	{
	private:
		int left, right;

	public:
		static constexpr int MAX_POINTS = 6;

		bool operator==(const Bone& bone) const;
		bool operator!=(const Bone& bone) const;
		bool operator<(const Bone& bone) const;

		friend std::ostream& operator<< (std::ostream& out, const Bone& bone);
		friend std::istream& operator>> (std::istream& in, Bone& bone);

		std::pair<int, int> get();
		void set(int left, int right);

		int getSum() const;

		static Bone genetate();
		Bone(int left = 0, int right = 0);
	};

	class Domino
	{
	public:
		static constexpr int MAX_BONES = 28;

		Domino(int num = 0);
		Domino(const Bone& bone);
		Domino(const std::initializer_list<Bone>& list);

		int size() const;
		void sort();
		Domino findScore(int val) const;

		friend std::ostream& operator<< (std::ostream& out, const Domino& domino);
		friend std::istream& operator>> (std::istream& in, Domino& point);

		Domino& operator++();
		Domino operator++(int);
		Domino& operator-=(const Domino& domino);
		Domino& operator+=(const Domino& domino);

		const Bone& operator[](int n) const;
		Bone& operator[](int n);

	private:
		Bone bones[MAX_BONES];
		int bonesNum;

		void removeBone(int i);

		/*
		При помощи ГПСЧ генерирует Bone, которого ещё нет в массиве
		Если массив уже заполнен, то бросает overflow_error
		*/
		Bone generateBone();
	};
}