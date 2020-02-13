//
//  Line.hpp
//  MazeSolver
//
//  Created by Preston Raab on 2/10/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

class Line {
private:
	int x1;
	int y1;
	int x2;
	int y2;
	double slope = 0.0;

	bool isZero;
	bool isInfinite;
public:
	Line(int fx, int fy, int sx, int sy);

	bool onPoint(int point[2]);

	bool onSameSide(int point3[2], int point4[2]);

	void print() {
		cout << "i1: " << y1 << " j1: " << x1 << " i2: " << y2 << " j2: " << x2 << endl;
	}

	void drawOn(vector<vector<unsigned char>>& map, bool thick);
};

#endif /* Line_hpp */
