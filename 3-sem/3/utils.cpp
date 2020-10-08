#include "utils.h"

Bone Domino::generateBone()
{
	if (bonesNum == MAX_BONES)
		throw std::overflow_error("���������� ������������ ���-�� ������");

	Bone bone = Bone::genetate();
	while (std::find(bones, bones + bonesNum, bone) != bones + bonesNum)
		bone = Bone::genetate();

	return bone;
}

Domino::Domino(int num)
	: bonesNum(0)
{
	if (num > Domino::MAX_BONES)
		throw std::overflow_error("��������� ������������ ����� ������");

	for (int i = 0; i < num; i++)
	{
		bones[i] = generateBone();
		bonesNum++;
	}
}

Domino::Domino(const std::initializer_list<Bone>& list)
	: bonesNum(0)
{
	if (list.size() > Domino::MAX_BONES)
		throw std::overflow_error("��������� ������������ ����� ������");

	for (const auto& it : list)
	{
		if (std::find(bones, bones + bonesNum, it) != bones + bonesNum)
			throw std::invalid_argument("��������� ������������� �����");
		bones[bonesNum] = it;
		bonesNum++;
	}
}

int Domino::size() const
{
	return bonesNum;
}

Domino& Domino::operator++()
{
	if (size() == MAX_BONES)
		throw std::overflow_error("��������� ����������� ����� ������");

	bones[bonesNum++] = generateBone();
	return *this;
}

Domino Domino::operator++(int)
{
	if (size() == MAX_BONES)
		throw std::overflow_error("��������� ����������� ����� ������");

	Domino tmp = *this;
	bones[bonesNum++] = generateBone();

	return tmp;
}

Domino& Domino::operator-=(const Bone& bone)
{
	Bone* p = std::find(bones, bones + bonesNum, bone);
	if (p == bones + bonesNum)
		throw std::out_of_range("�� ������� ��������� �����");

	int index = p - bones;
	for (int i = index; i <= bonesNum; i++)
	{
		if (i == bonesNum)
			bones[i] = Bone();
		else
			bones[i] = bones[i + 1];
	}
	bonesNum--;

	return *this;
}

const Bone& Domino::operator[](int n) const
{
	if (n > bonesNum - 1)
		throw std::out_of_range("������ �� ��������� �������");
	return bones[n];
}

Bone& Domino::operator[](int n)
{
	if (n > bonesNum - 1)
		throw std::out_of_range("������ �� ��������� �������");
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

std::pair<int, int> Bone::get()
{
	return std::make_pair(left, right);
}

void Bone::set(int left, int right)
{
	if (left > MAX_POINTS || right > MAX_POINTS)
		throw std::invalid_argument("������������ �������� ������� ����� - " + std::to_string(MAX_POINTS));
	this->left = left;
	this->right = right;
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
