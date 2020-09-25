#include "utils.h"

void createNewMatrix(const Matrix& matrix, Matrix& newMatrix)
{
	for (auto row : matrix)
	{
		std::map<int, ExistedNode> memory;
		std::map<int, int> newRow;
		for (auto col : row.second)
		{
			if (memory[col.second].count > 0)
			{
				newRow[col.first] = newRow[memory[col.second].firstPos] = col.second;
			}
			else
			{
				memory[col.second].firstPos = col.first;
			}
			memory[col.second].count++;
		}
		newMatrix[row.first] = newRow;
	}
}
