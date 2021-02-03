#pragma once

#include <string>
#include <math.h>
#include <iostream>

#include <QDebug>

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
			return abs((int)x - (int)to.x) + abs((int)y - (int)to.y);
		}

		template<class T>
		inline friend T& operator<< (T& out, const Coord& point)
		{
			out << "(" << point.x << "; " << point.y << ")";
			return out;
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