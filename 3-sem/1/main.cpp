#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include "utils.h"

/*
8
0 
0 
1
0 
1 
1
0 
2 
2
0 
3 
1
1 
0 
2
1 
1 
3
1 
2 
2
1 
3 
3
*/

int n, m;
Matrix matrix, newMatrix;

int getInt()
{
	int val;
	bool isCorrect = false;
	while (!isCorrect)
	{
		std::cin >> val;
		if (std::cin.good())
			isCorrect = true;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return val;
}

int main()
{
	setlocale(0, "");

	std::cout << "Кол-во значений: ";
	n = getInt();

	for (int i = 0; i < n; i++)
	{
		int col, row, val;
		std::cout << "Строка: ";
		row = getInt();
		std::cout << "Столбец: ";
		col = getInt();
		std::cout << "Значение: ";
		val = getInt();
		matrix[row][col] = val;
		std::cout << std::endl;
	}

	createNewMatrix(matrix, newMatrix);

	std::cout << std::endl;
	for (auto row : newMatrix)
	{
		for (auto col : row.second)
			std::cout << col.second << " ";
		std::cout << std::endl;
	}

	system("pause");
	return 0;
}