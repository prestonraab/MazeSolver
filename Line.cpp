//
//  Line.cpp
//  MazeSolver
//
//  Created by Preston Raab on 2/10/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#include "Line.hpp"

Line::Line(int fx, int fy, int sx, int sy) {
	x1 = fx;
	x2 = sx;
	y1 = fy;
	y2 = sy;
	slope = 0;

	isZero = y2 == y1;
	isInfinite = x2 == x1;
	if (!isZero && !isInfinite) {
		slope = (1.0 * y2 - 1.0 * y1) / (double)(1.0 * x2 - 1.0 * x1);
	}
}

bool Line::onPoint(int point[2])
{
	int x = point[0];
	int y = point[1];
	if (isZero) {
		return y == y1;
	}
	if (isInfinite) {
		return x == x1;
	}
	return (y1 - y2) * (x1 - x) == (y1 - y) * (x1 - x2);
}

bool Line::onSameSide(int point3[2], int point4[2])
{
	int x3 = point3[0];
	int y3 = point3[1];
	int x4 = point4[0];
	int y4 = point4[1];
	if (isZero) {
		return (y3 > y1) == (y4 > y1);
	}
	if (isInfinite) {
		return (x3 > x1) == (x4 > x1);
	}
	return	(slope * (static_cast<double>(x3) - static_cast<double>(x1)) + y1 > y3) ==
		(slope * (static_cast<double>(x4) - static_cast<double>(x1)) + y1 > y4);
}

void Line::drawOn(vector<vector<unsigned char>>& map, bool thick) {
	char value = 'G';
	
	if (isInfinite) { //plug in y
		for (int y = y1; y != y2; y += sgn(y2 - y1)) {
			if(thick) map[y][x1 - 1] = value;
			map[y][x1] = value;
			if(thick) map[y][x1 + 1] = value;
		}
	}
	else if (std::abs(static_cast<double>(slope)) > 1) { // plug in y
		for (int y = y1; y != y2; y += sgn(y2 - y1)) {
			int x = x1 + (static_cast<double>(y) - y1) / slope;
			if(thick) map[y][x - 1] = value;
			map[y][x] = value;
			if(thick) map[y][x + 1] = value;
		}
	}
	else { //plug in x
		for (unsigned int x = x1; x != x2; x += sgn(x2 - x1)) {
			unsigned int y = slope * (static_cast<double>(x) - x1) + y1;
			if(thick) map[y - 1][x] = value;
			map[y][x] = value;
			if (y + 1 < map.size())
				if(thick) map[y + 1][x] = value;
		}
	}
}
