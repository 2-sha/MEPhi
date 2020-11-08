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

		bool operator==(const Bone&) const;
		bool operator!=(const Bone&) const;
		bool operator<(const Bone&) const;
		bool operator>(const Bone&) const;
		bool operator<=(const Bone&) const;
		bool operator>=(const Bone&) const;

		friend std::ostream& operator<< (std::ostream&, const Bone&);
		friend std::istream& operator>> (std::istream&, Bone&);

		std::pair<int, int> get();
		void set(int, int);

		int getSum() const;

		Bone(int = 0, int = 0);
	};

	class Domino
	{
	public:
		static constexpr int MAX_BONES = 28;

		Domino(int = 0);
		Domino(const Bone&);
		Domino(const std::initializer_list<Bone>&);
		Domino(const Domino&);
		Domino(Domino&&);
		~Domino();

		int size() const;
		void sort();
		Domino findScore(int) const;

		friend std::ostream& operator<< (std::ostream&, const Domino&);
		friend std::istream& operator>> (std::istream&, Domino&);

		Domino& operator++();
		Domino operator++(int);
		Domino& operator-=(const Domino&);
		Domino& operator+=(const Domino&);

		Domino& operator= (const Domino&);

		const Bone& operator[](int) const;

	private:
		Bone *bones;
		unsigned int bonesNum;

		/*
		При помощи ГПСЧ генерирует Bone, которого ещё нет в массиве
		Если массив уже заполнен, то бросает overflow_error
		*/
		Bone generateBone();
	};
}