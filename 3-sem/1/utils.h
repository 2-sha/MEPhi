#pragma once
#include <list>
#include <map>
#include <utility>
#include <algorithm>

struct Node
{
	int i;
	int val;

	// Для тестов
	bool operator==(const Node& elem) const {
		return (elem.i == this->i && elem.val == this->val);
	}

	Node(int i, int val)
		: i(i), val(val) {};
};

struct ExistedNode
{
	int firstPos = -1, count = 0;
};

//typedef std::list<std::list<std::pair<int, int>>> Matrix;
typedef std::map<int, std::map<int, int>> Matrix;

void createNewMatrix(const Matrix& matrix, Matrix& newMatrix);