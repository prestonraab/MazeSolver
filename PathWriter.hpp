//
//  PathWriter.hpp
//  MazeSolver
//
//  Created by Preston Raab on 2/11/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#ifndef PathWriter_hpp
#define PathWriter_hpp

#include <stdio.h>
#include "MazeRunner.h"

class PathWriter{
public:
	PathWriter(MazeRunner* runner, const char* fileOut):runner(runner), fileOut(fileOut){
		
	}
	
	void drawOnMap(vector<vector<unsigned char>>& mapToPaint, vector<MajorBlock> blocks, Path p, vector<int> start, vector<int> end, bool thick);
	void drawOnMap(vector<vector<unsigned char>>& map, Path p, bool thick);
	
	void encodeOneStep(vector<vector<unsigned char>>& paintedMap, vector<vector<unsigned char>> greyMap);
	
private:
	MazeRunner* runner;
	const char* fileOut;
};

#endif /* PathWriter_hpp */
