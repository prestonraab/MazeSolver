//
//  Path.hpp
//  MazeSolver
//
//  Created by Preston Raab on 2/10/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include <cmath>
#include <iostream>
#include "MajorBlock.h"
#include "Line.hpp"

using std::cout;
using std::endl;

class Path {
public:
	vector<MajorBlock> steps;
	double distance;

	Path() {
		steps = {};
		distance = 0;
	}

	Path(int d) {
		steps = { Corner(d,d,Corner::ALL) };
		distance = d;
	}
	
	void operator=(Path other){
		other.steps = this->steps;
		other.distance = this->distance;
	}

	unsigned long getSize() {
		return steps.size();
	}

	Path& add(MajorBlock m) {
		steps.push_back(m);

		if (steps.size() == 1)
			return *this;

		MajorBlock last = steps.at(steps.size() - 2);
		distance += sqrt(std::pow(last.i - m.i, 2) * 1.0 + std::pow(last.j - m.j, 2) * 1.0);
		return *this;
	}

	bool has(MajorBlock const &m) const {
		for (MajorBlock const &c : steps) {
			if (c.i == m.i && c.j == m.j) {
				return true;
			}
		}
		return false;
	}

	int find(MajorBlock const &m) {
		for (unsigned int x = 0; x < steps.size(); x++) {
			MajorBlock c = steps.at(x);
			if (c.i == m.i && c.j == m.j) {
				return x;
			}
		}
		return false;
	}

	Path reverse() {
		Path rev = {};
		for (unsigned i = (unsigned)steps.size() - 1; i != -1; i--) {
			rev.add(steps.at(i));
		}
		return rev;
	}

	void print() {
		for (MajorBlock m : steps) {
			cout << "(" << m.i << ", " << m.j << ")" << "\t";
		}
		cout << endl;
		cout << endl;
	}

	void drawOnMap(vector<vector<unsigned char>>& map, bool thick) {
		for (int x = 0; x < steps.size() - 1; ++x) {
			Line l = Line(steps[x].j, steps[x].i, steps[x + 1].j, steps[x + 1].i);
			l.print();
			l.drawOn(map, thick);
		}
		cout << "NumSteps: " << steps.size() << endl;
		cout << "Distance: " << distance << endl;
	}
};

#endif /* Path_hpp */
