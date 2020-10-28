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
	: bonesNum(0)
{
	if (num > Domino::MAX_BONES)
		throw std::overflow_error("Превышено максимальное число костей");
	if (num < 0)
		throw std::invalid_argument("Число костей не может быть отрицательным");


	for (int i = 0; i < num; i++)
	{
		bones[i] = generateBone();
		bonesNum++;
	}
}

Domino::Domino(const Bone& bone)
{
	bones[0] = bone;
	bonesNum = 1;
}

Domino::Domino(const std::initializer_list<Bone>& list)
	: bonesNum(0)
{
	if (list.size() > Domino::MAX_BONES)
		throw std::overflow_error("Превышено максимальное число костей");

	for (const auto& it : list)
	{
		if (std::find(bones, bones + bonesNum, it) != bones + bonesNum)
			throw std::invalid_argument("Встречена повторяющяяся кость");
		bones[bonesNum] = it;
		bonesNum++;
	}
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

	for (Bone bone : bones)
		if (bone.getSum() == val)
			newDomino += bone;

	return newDomino;
}

Domino& Domino::operator++()
{
	if (size() == MAX_BONES)
		throw std::overflow_error("Превышено максимально число костей");

	bones[bonesNum++] = generateBone();

	return *this;
}

Domino Domino::operator++(int)
{
	if (size() == MAX_BONES)
		throw std::overflow_error("Превышено максимально число костей");

	Domino tmp = *this;
	bones[bonesNum++] = generateBone();

	return tmp;
}

Domino &Domino::operator-=(const Domino & domino)
{
	for (int i = 0; i < domino.size(); i++)
	{
		Bone* p = std::find(bones, bones + bonesNum, domino[i]);
		if (p == bones + bonesNum)
			throw std::out_of_range("Не найдена указанная кость");
		removeBone(p - bones);
	}

	return *this;
}

Domino &Domino::operator+=(const Domino & domino)
{
	if (this->size() + domino.size() > Domino::MAX_BONES)
		throw std::overflow_error("Превышено максимальное кол-во костей");
	for (int i = 0; i < domino.size(); i++)
	{
		if (std::find(bones, bones + bonesNum, domino[i]) != bones + bonesNum)
			throw std::out_of_range("Одна из костей уже существует");
	}

	for (int i = 0; i < domino.size(); i++)
		bones[bonesNum++] = domino[i];

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

void Domino::removeBone(int index)
{
	if (index < 0 || index > bonesNum - 1)
		throw std::out_of_range("Индекс находится за пределами массива");
	for (int i = index; i <= bonesNum; i++)
	{
		if (i == bonesNum)
			bones[i] = Bone();
		else
			bones[i] = bones[i + 1];
	}
	bonesNum--;
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
