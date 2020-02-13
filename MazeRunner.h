
#ifndef MAZERUNNER_H
#define MAZERUNNER_H

#include<iostream> 
#include<vector> 
#include<string>
#include<iomanip>
#include <cmath>
#include<algorithm>
#include "Maze.h"
#include "Line.hpp"
#include "MajorBlock.h"
#include "Path.hpp"
#include "Campus.h"
#include "lodepng.h"
#include <algorithm>

//#include "Disp.h"

using namespace std;

static unsigned char black = 0x00;
static unsigned char white = 0xFF;


class MazeRunner{
private:
	vector<vector<bool>> map;
	
	unsigned int height;
	unsigned int width;
	
	vector<vector<bool>> likelyMap;
	vector<vector<unsigned char>> finalMap;
	
	Corner start;// = Corner(1, 1, Corner::TL);
	Corner finish;// = Corner(3, 9, Corner::ALL);

	vector<Corner> allCorners;// = { start, finish };

	MajorBlock mfinish;// = Corner(finish);


	Path shortPath = {};
	vector<MajorBlock> foundShortBlocks = {};
	vector<Path> foundShortPaths = {};
	

	const char* fileIn;
	
	bool removeUnlikelies(vector<vector<bool>>& m);
	
	void identifyCorners(vector<vector<bool>> m, vector<Corner>& c);
	
	void getPotentialCorners(int i, int j, Corner::direction d, vector<MajorBlock> corners);
	
	
	void findConnections(vector<MajorBlock>& blocks);
	
public:
	vector<MajorBlock> allBlocks;
	
	MazeRunner(Campus& campus): map(campus.getMap()),
									height((unsigned)map.size()),
									width((unsigned)map[0].size()),
									start(Corner(campus.start[0], campus.start[1], Corner::TL)),
									finish(Corner(campus.end[0], campus.end[1], Corner::ALL)),
									mfinish(finish),
									likelyMap(map)
	{
		cout << "Found: " << campus.found << endl;
	}
	
	MazeRunner(vector<vector<bool>> map, vector<int> startPos, vector<int> endPos):
									map(map),
									height((unsigned)map.size()),
									width((unsigned)map[0].size()),
									start(Corner(startPos.at(0), startPos.at(1), Corner::ALL)),
									finish(Corner(endPos.at(0), endPos.at(1), Corner::ALL)),
									mfinish(finish),
									likelyMap(map)
	{
		print(map);

		allCorners = { start,finish };
	}
	
	Path getShortestPath(){
		return shortPath;
	}
	
	unsigned int getWidth(){
		return width;
	}
	
	unsigned int getHeight(){
		return height;
	}
	
	bool solve(){
		//print(likelyMap);
//		int numIterations = 0;
		
//		while (removeUnlikelies(likelyMap)) {
//			cout << ++numIterations << endl;
//		}
		
		//print(likelyMap);
		cout << "Identifying Corners" << endl;
		identifyCorners(likelyMap, allCorners);
		cout << "Corner size: " << allCorners.size();
		//print(allCorners);
		cout << "Finding Major Blocks" << endl;
		allBlocks = MajorBlock::findMajorBlocks(allCorners, start, finish);
		cout << "Finding Connections" << endl;
		findConnections(allBlocks);
		print(allBlocks);
		cout << "Printed All Blocks" << endl;
		MajorBlock startBlock = getBlock(start);
		cout << "Start block connections size: " << startBlock.connections.size() << endl;
		cout << "Start block first connection: (" << startBlock.connections[0].i << ", " << startBlock.connections[0].j << ")" << endl;
		cout << "Finding Best Path" << endl;
		Path initialPath;
		shortPath = findBestPath(startBlock, initialPath).reverse();//.add(startBlock).reverse();
		shortPath.print();
		cout << "Printed Shortest Path" << endl;
		//print(allBlocks);
		cout << "Printed All Blocks" << endl;

		char default_value = 0;
		vector<vector<unsigned char>> fin(map.size(), vector<unsigned char>(map[0].size(), default_value));
		
		return shortPath.distance != INT_MAX;
	}
	
	



	



	MajorBlock getBlock(MajorBlock m) {
		for (MajorBlock& c : allBlocks) {
			if (m.i == c.i && m.j == c.j) {
				return *&c;
			}
		}
		return MajorBlock(Corner(-1, -1, Corner::ALL));
	}

	Path maxPath = Path(INT_MAX);

	Path shortestPath(vector<Path> paths) {
		double min = INT_MAX;
		Path shortest = maxPath;
		for (Path p : paths) {
			if (p.distance < min && p.has(mfinish)) {
				shortest = p;
				min = p.distance;
			}
		}
		return shortest;
	}

	vector<Path> alreadySolvedPaths = {};

	Path findBestPath(MajorBlock const &m, Path const &p);



	void print(vector<vector<bool>> m) {
		for (unsigned int i = 0; i < m.size(); i++) {
			for (unsigned int j = 0; j < m.at(i).size(); j++) {
				std::cout << (m.at(i).at(j) ? "X" : " ");//(unsigned char)(219) : (unsigned char)(32));
				//std::cout << (m.at(i).at(j) ? (unsigned char)(219) : (unsigned char)(32));
			}
			std::cout << std::endl;
		}
		cout << endl;
	}

	void print(vector<Corner> corners) {
		for (unsigned int i = 0; i + 1 < map.size(); i++) {
			for (unsigned int j = 0; j + 1 < map.at(i).size(); j++) {
				bool isInCorners = false;
				for (unsigned int k = 0; k < corners.size(); k++) {
					if (i == corners[k].i && j == corners[k].j)
						isInCorners = true;
				}
				std::cout << (isInCorners ? "X" : " ");
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void print(vector<MajorBlock> blocks) {
		int pathCount = 0;
		for (unsigned int i = 0; i < map.size(); i++) {
			for (unsigned int j = 0; j < map.at(i).size(); j++) {
				bool isInCorners = false;
				long size = 0;
				bool isInPath = false;
				for (unsigned int k = 0; k < blocks.size(); k++) {
					if (i == blocks[k].i && j == blocks[k].j) {
						isInCorners = true;
						if (shortPath.has(blocks[k])) {
							isInPath = true;
							pathCount = shortPath.find(blocks[k]);
						}
						else
							size = blocks[k].connections.size();
					}
				}
				if (isInPath) {
					if (i == start.i && j == start.j)
						cout << "SS";
					else if (i == finish.i && j == finish.j)
						cout << "FF";
					else {
						cout << fixed << setw(2) << pathCount % 100;
					}
					pathCount++;
				}
				else if (isInCorners) {
					std::cout << setw(2) << size;
				}
				else if (map[i][j]) {
					std::cout << "XX";//char(219) << char(219);
				}
				else {
					std::cout << "  ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}




	
};


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#endif
