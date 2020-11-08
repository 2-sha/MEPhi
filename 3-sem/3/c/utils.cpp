#include "utils.h"
using namespace lab3;

Bone Domino::generateBone()
{
	if (bonesNum == MAX_BONES)
		throw std::overflow_error("Достигнуто максимальное кол-во костей");

	Domino variants;
	for (int i = 0; i <= Bone::MAX_POINTS; i++)
		for (int j = i; j <= Bone::MAX_POINTS; j++)
			variants += Bone(i, j);
	variants -= *this;

	std::random_device rd;
	std::mt19937 mersenne(rd());
	return variants[mersenne() % variants.size()];
}

Domino::Domino(int num)
	: bonesNum(0), bones(nullptr)
{
	if (num > Domino::MAX_BONES)
		throw std::overflow_error("Превышено максимальное число костей");
	if (num < 0)
		throw std::invalid_argument("Число костей не может быть отрицательным");

	for (int i = 0; i < num; i++)
		*this += generateBone();
}

Domino::Domino(const Bone& bone)
{
	/*
	*this += bone;
	Здесь нельзя это использовать, потому что получается рекурсия
	*/
	bones = new Bone[1];
	bonesNum = 1;
	bones[0] = bone;
}

Domino::Domino(const std::initializer_list<Bone>& list)
	: bonesNum(0), bones(nullptr)
{
	if (list.size() > Domino::MAX_BONES)
		throw std::overflow_error("Превышено максимальное число костей");

	for (const auto& it : list)
	{
		if (std::find(bones, bones + bonesNum, it) != bones + bonesNum)
			throw std::invalid_argument("Встречена повторяющяяся кость");
		*this += it;
	}
}

Domino::Domino(const Domino& domino)
{
	bonesNum = domino.bonesNum;
	bones = new Bone[bonesNum];
	bones = domino.bones;
}

Domino::Domino(Domino&& domino)
	: bones(domino.bones), bonesNum(domino.size())
{
	domino.bones = nullptr;
}

Domino::~Domino()
{
	delete[] bones;
}

int Domino::size() const
{
	return bonesNum;
}

void Domino::sort()
{
	std::sort(bones, bones + bonesNum);
}

Domino Domino::findScore(int val) const
{
	Domino newDomino;
	for (int i = 0; i < bonesNum; i++)
	{
		if (bones[i].getSum() == val)
			newDomino += bones[i];
	}

	return newDomino;
}

Domino& Domino::operator++()
{
	if (size() == MAX_BONES)
		throw std::overflow_error("Превышено максимально число костей");

	*this += generateBone();

	return *this;
}

Domino Domino::operator++(int)
{
	if (size() == MAX_BONES)
		throw std::overflow_error("Превышено максимально число костей");

	Domino tmp = *this;
	*this += generateBone();

	return tmp;
}

Domino &Domino::operator-=(const Domino& domino)
{
	int newBonesNum = bonesNum;
	for (int i = 0; i < domino.size(); i++)
	{
		Bone* item = std::find(bones, bones + bonesNum, domino[i]);
		if (item == bones + bonesNum)
			continue;
		for (int j = item - bones; j < bonesNum - 1; j++)
			bones[j] = bones[j + 1];
		newBonesNum--;
	}

	Bone *newBones = new Bone[newBonesNum];
	for (int i = 0; i < newBonesNum; i++)
		newBones[i] = bones[i];
	delete[] bones;
	bones = newBones;
	bonesNum = newBonesNum;

	return *this;
}

Domino &Domino::operator+=(const Domino& domino)
{
	// Удаляем повторяющеися кости
	Bone* bonesToAdd = new Bone[domino.size()];
	int bonesToAddNum = 0;
	for (int i = 0; i < domino.size(); i++)
		if (std::find(bones, bones + bonesNum, domino[i]) == bones + bonesNum)
			bonesToAdd[bonesToAddNum++] = domino[i];

	int newBonesNum = bonesNum + bonesToAddNum;
	Bone *newBones = new Bone[newBonesNum];
	int counter = 0;
	for (int i = 0; i < newBonesNum; i++)
	{
		if (bonesNum > i)
			newBones[i] = bones[i];
		else
			newBones[i] = bonesToAdd[i - bonesNum];
	}
	delete[] bonesToAdd;
	delete[] bones;
	bones = newBones;
	bonesNum = newBonesNum;

	return *this;
}

Domino& Domino::operator=(const Domino& domino)
{
	if (&domino == this)
		return *this;

	delete[] bones;

	bonesNum = domino.bonesNum;
	bones = new Bone[bonesNum];
	bones = domino.bones;

	return *this;
}

const Bone& Domino::operator[](int n) const
{
	if (n >= bonesNum || n < 0)
		throw std::out_of_range("Индекс за пределами массива");
	return bones[n];
}

bool Bone::operator==(const Bone& bone) const
{
	return (this->left == bone.left && this->right == bone.right);
}

bool Bone::operator!=(const Bone& bone) const
{
	return !(*this == bone);
}

bool Bone::operator<(const Bone& bone) const
{
	return (bone.getSum() > this->getSum());
}

bool Bone::operator>(const Bone& bone) const
{
	return (bone.getSum() < this->getSum());
}

bool Bone::operator<=(const Bone& bone) const
{
	return (bone.getSum() >= this->getSum());
}

bool Bone::operator>=(const Bone& bone) const
{
	return (bone.getSum() <= this->getSum());
}

std::pair<int, int> Bone::get()
{
	return std::make_pair(left, right);
}

void Bone::set(int first, int second)
{
	if (first > MAX_POINTS || second > MAX_POINTS)
		throw std::invalid_argument("Максимальное значение стороны кости - " + std::to_string(MAX_POINTS));
	if (first < 0 || second < 0)
		throw std::invalid_argument("Значение кости не может быть меньше 0");
	if (first < second)
	{
		this->left = first;
		this->right = second;
	}
	else
	{
		this->left = second;
		this->right = first;
	}
}

int Bone::getSum() const
{
	return left + right;
}

Bone::Bone(int left, int right)
{
	set(left, right);
}
