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
	const Path* previous;
	const MajorBlock* data;
	double distance;
	
	Path(MajorBlock const *block) : previous(NULL), data(block), distance(0){
	}
	
	Path(MajorBlock const *block, const Path* previous) : previous(previous), data(block), distance(0){
	}

	Path(MajorBlock const *block, int d) : previous(NULL), data(block), distance(d) {
	}
	
	Path* add(MajorBlock const *m) const{
		Path* next = new Path(m, this);
		next->distance = distance + sqrt(std::pow(data->i - m->i, 2) * 1.0 + std::pow(data->j - m->j, 2) * 1.0);
		return next;
	}

	bool has(MajorBlock const *m) const {
		if (data->i == m->i && data->j == m->j) {
			return true;
		}
		
		if(previous == NULL){
			return false;
		}
		else{
			return previous->has(m);
		}
	}

	const Path* find(MajorBlock const &m) const{
		if (data->i == m.i && data->j == m.j) {
			return this;
		}
		
		if(previous == NULL){
			return NULL;
		}
		
		return previous->find(m);
	}

//	Path reverse() {
//		Path rev = {};
//		for (unsigned i = (unsigned)steps.size() - 1; i != -1; i--) {
//			rev.add(steps.at(i));
//		}
//		return rev;
//	}

	void print() const{
		cout << "(" << data->i << ", " << data->j << ")" << "\t";
		
		if(previous == NULL){
			cout << endl;
			cout << endl;
		}
		else{
			previous->print();
		}
	}

	void drawOnMap(vector<vector<unsigned char>>& map, bool thick, int numSteps, double distance) const{
		if(previous != NULL){
			Line l = Line(data->j, data->i, previous->data->j, previous->data->i);
			l.print();
			l.drawOn(map, thick);
			previous->drawOnMap(map, thick, ++numSteps, distance);
		}
		else{
			cout << "NumSteps: " << numSteps << endl;
			cout << "Distance: " << distance << endl;
		}
	}
};

#endif /* Path_hpp */
