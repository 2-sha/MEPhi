#pragma once

#include <string>
#include <math.h>

namespace robots
{
	struct Coord
	{
		unsigned x, y;
		Coord(unsigned x = 0, unsigned y = 0) : x(x), y(y) {};

		inline bool operator==(const Coord& coord) const { return this->x == coord.x && this->y == coord.y; };
		inline bool operator!=(const Coord& coord) const { return this->x != coord.x || this->y != coord.y; };
		//inline bool operator<(const Coord& coord) const { return this->y < coord.y || this->x < coord.x; };
		//inline bool operator>(const Coord& coord) const { return this->y > coord.y || this->x > coord.x; };
		//inline bool operator<=(const Coord& coord) const { return *this == coord || this->y < coord.y || this->x < coord.x; };
		//inline bool operator>=(const Coord& coord) const { return *this == coord || this->y >= coord.y || this->x >= coord.x; };

		inline unsigned calcDist(const Coord& to) const
		{
			return ceil(sqrt(pow((int)x - (int)to.x, 2) + pow((int)y - (int)to.y, 2)));
		}
	};
}

namespace std {
    template <>
    struct hash<robots::Coord>
    {
		inline size_t operator()(const robots::Coord& k) const { return stoi(to_string(k.x) + to_string(k.y)); };
    };
}