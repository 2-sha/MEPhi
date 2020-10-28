#include "utils.h"
using namespace lab3;

Bone Domino::generateBone()
{
	if (bonesNum == MAX_BONES)
		throw std::overflow_error("Достигнуто максимальное кол-во костей");

	Bone bone = Bone::genetate();
	while (std::find(bones, bones + bonesNum, bone) != bones + bonesNum)
		bone = Bone::genetate();

	return bone;
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
	*this = domino;
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

Domino &Domino::operator-=(const Domino & domino)
{
	// Нельзя ничего менять, пока не убедимся, что всё будет корректно
	for (int i = 0; i < domino.size(); i++)
		if (std::find(bones, bones + bonesNum, domino[i]) == bones + bonesNum)
			throw std::out_of_range("Не найдена указанная кость");

	int newBonesNum = bonesNum - domino.size();
	Bone *newBones = new Bone[newBonesNum];
	int counter = 0;
	for (int i = 0; i < bonesNum; i++)
	{
		bool isFound = false;
		for (int j = 0; j < domino.size(); j++)
		{
			if (domino[j] == bones[i])
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
			newBones[counter++] = bones[i];
	}
	delete[] bones;
	bones = newBones;
	bonesNum = newBonesNum;

	return *this;
}

Domino &Domino::operator+=(const Domino & domino)
{
	// Нельзя ничего менять, пока не убедимся, что всё будет корректно
	if (this->size() + domino.size() > Domino::MAX_BONES)
		throw std::overflow_error("Превышено максимальное кол-во костей");
	for (int i = 0; i < domino.size(); i++)
		if (std::find(bones, bones + bonesNum, domino[i]) != bones + bonesNum)
			throw std::out_of_range("Одна из костей уже существует");

	int newBonesNum = bonesNum + domino.size();
	Bone *newBones = new Bone[newBonesNum];
	for (int i = 0; i < newBonesNum; i++)
	{
		if (bonesNum > i)
			newBones[i] = bones[i];
		else
			newBones[i] = domino[i - this->size()];
	}
	delete[] bones;
	bones = newBones;
	bonesNum = newBonesNum;

	return *this;
}

Domino& Domino::operator=(const Domino& domino)
{
	bonesNum = domino.size();
	bones = new Bone[bonesNum];
	for (int i = 0; i < bonesNum; i++)
		bones[i] = domino[i];
	return *this;
}

const Bone& Domino::operator[](int n) const
{
	if (n > bonesNum - 1 || n < 0)
		throw std::out_of_range("Индекс за пределами массива");
	return bones[n];
}

Bone& Domino::operator[](int n)
{
	if (n > bonesNum - 1 || n < 0)
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

std::pair<int, int> Bone::get()
{
	return std::make_pair(left, right);
}

void Bone::set(int left, int right)
{
	if (left > MAX_POINTS || right > MAX_POINTS)
		throw std::invalid_argument("Максимальное значение стороны кости - " + std::to_string(MAX_POINTS));
	if (left < 0 || right < 0)
		throw std::invalid_argument("Значение кости не может быть меньше 0");
	this->left = left;
	this->right = right;
}

int Bone::getSum() const
{
	return left + right;
}

Bone Bone::genetate()
{
	std::random_device rd;
	std::mt19937 mersenne(rd());

	return Bone(mersenne() % Bone::MAX_POINTS, mersenne() % Bone::MAX_POINTS);
}

Bone::Bone(int left, int right)
{
	set(left, right);
}
