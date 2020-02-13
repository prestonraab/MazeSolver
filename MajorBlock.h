//
//  MajorBlock.h
//  MazeSolver
//
//  Created by Preston Raab on 2/10/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#ifndef MajorBlock_h
#define MajorBlock_h

#include "Corner.h"
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class MajorBlock {
public:
	int i;
	int j;
	vector<MajorBlock> connections;

	MajorBlock(Corner c) {
		if (c.d == Corner::TL || c.d == Corner::TR || c.d == Corner::ALL)
			i = c.i;
		else
			i = c.i + 1;
		if (c.d == Corner::TL || c.d == Corner::BL || c.d == Corner::ALL)
			j = c.j;
		else
			j = c.j + 1;
		connections = {};
	}
	
	MajorBlock(const MajorBlock &other): i(other.i), j(other.j), connections(other.connections){
		
	}
	
	MajorBlock operator=(const MajorBlock &other){
		return other;
	}
	
	static vector<MajorBlock> findMajorBlocks(vector<Corner> corners, Corner start, Corner finish) {
		vector<MajorBlock> blocks;
		blocks.push_back(start);
		blocks.push_back(finish);
		blocks[0].connections.push_back(finish);

		unsigned long size = corners.size();
		unsigned percentile = (unsigned)size / 100 + 1;

		for (unsigned int i = 0; i < size; i++) {
			Corner c = corners[i];
			MajorBlock m = MajorBlock(c);
			blocks.at(0).connections.push_back(m);
			m.connections.push_back(finish);

			getPotentialCorners(m.connections, c.i, c.j, c.d, corners);
			
			if (m.connections.size() > 0) {
				blocks.push_back(m);
			}

			if (i % percentile == 0){
				cout << i * 100.0 / size << "%" << endl;
			}
		}
		cout << "Removing Repeats" << endl;
		//vector<MajorBlock> remov = removeRepeats(blocks);
		return blocks;
	}
	
	
	static vector<MajorBlock> removeRepeats(vector<MajorBlock> repList) {
		return removeRepeats(repList, -1, -1);
	}
	
	static vector<MajorBlock> removeRepeats(vector<MajorBlock> repList, int i, int j)
	{
		vector<MajorBlock> newList = {}; // create a new list to store values for keeping
		for (MajorBlock& m : repList) {    // iterate through the given list
			bool foundRepeat = false;
			if (m.i == i && m.j == j) { //if we match i and j, definitely don't add it to anything
				foundRepeat = true;
			}
			else
			{
				vector<MajorBlock>::iterator it = lower_bound(newList.begin(), newList.end(), m);  //check what's already added to the list
				if (it != newList.end() && *it == m) {  //if we found it
					foundRepeat = true;          //we found a repeat
					for (MajorBlock &c : m.connections) {  //add the given one's connections to the new list one's connections
						if (!(c == *it) && !(c.i == i && c.j == j))
						{
							it->connections.push_back(c);
						}
					}
					//vector<MajorBlock> removed = removeRepeats(n.connections, n.i, n.j);
					//n.connections.assign(removed.begin(), removed.end());
				}
			}
			if (!foundRepeat) {
//				vector<MajorBlock> removed = removeRepeats(m.connections, m.i, m.j);
//				m.connections.assign(removed.begin(), removed.end());

				newList.insert(lower_bound(newList.begin(), newList.end(), m), m);
			}
		}
		return newList;
	}
	
	static void getPotentialCorners(vector<MajorBlock> &potentialCorners, int &i, int &j, Corner::direction const &d, vector<Corner> const &corners)
	{
		for (Corner c : corners)
		{
			if (c.d == Corner::ALL)
			{
				potentialCorners.push_back(c);
				continue;
			}
			switch (Corner::getRelation(d, c.d))
			{
			case Corner::adj:
				switch (d)
				{
				case Corner::TL:
					if (i == c.i && c.d == Corner::TR)
						potentialCorners.push_back(c);
					else if (j == c.j && c.d == Corner::BL)
						potentialCorners.push_back(c);
					break;
				case Corner::TR:
					if (i == c.i && c.d == Corner::TL)
						potentialCorners.push_back(c);
					else if (j == c.j && c.d == Corner::BR)
						potentialCorners.push_back(c);
					break;
				case Corner::BL:
					if (i == c.i && c.d == Corner::BR)
						potentialCorners.push_back(c);
					else if (j == c.j && c.d == Corner::TL)
						potentialCorners.push_back(c);
					break;
				case Corner::BR:
					if (i == c.i && c.d == Corner::BL)
						potentialCorners.push_back(c);
					else if (j == c.j && c.d == Corner::TR)
						potentialCorners.push_back(c);
					break;
				default:
					break;
				}
			default:
				switch (d)
				{
					case Corner::TL:
						if (Corner::searchDirection(i, j, Corner::TR, c) || Corner::searchDirection(i, j, Corner::BL, c))
							potentialCorners.push_back(c);
						break;
					case Corner::TR:
						if (Corner::searchDirection(i, j, Corner::BR, c) || Corner::searchDirection(i, j, Corner::TL, c))
							potentialCorners.push_back(c);
						break;
					case Corner::BL:
						if (Corner::searchDirection(i, j, Corner::TL, c) || Corner::searchDirection(i, j, Corner::BR, c))
							potentialCorners.push_back(c);
						break;
					case Corner::BR:
						if (Corner::searchDirection(i, j, Corner::BL, c) || Corner::searchDirection(i, j, Corner::TR, c))
							potentialCorners.push_back(c);
						break;
					default:
						break;
				}
			}
		}
	}

	bool operator==(MajorBlock other) {
		return other.i == i && other.j == j;
	}
	bool operator<(MajorBlock other) {
		return other.i < i || (other.i == i && other.j < j);
	}
	bool operator<(const MajorBlock other) const {
		return other.i < i || (other.i == i && other.j < j);
	}
};



#endif /* MajorBlock_h */
