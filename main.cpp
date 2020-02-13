//  main.cpp: This file contains the 'main' function. Program execution begins and ends there.
//  MazeSolver
//
//  Created by Preston Raab on 2/10/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#include <stdio.h>

#include "Campus.h"
#include "MazeRunner.h"
#include "PathWriter.hpp"

int main()
{
	//const char* fileIn = "/Users/Preston/Downloads/maze.bmp";
	const char* fileIn = "/Users/Preston/Desktop/simpleMaze.bmp";
	const char* fileOut = "/Users/Preston/Desktop/new.bmp";
	//"/Users/Preston/MazeSolver/MazeSolver/BigMaze.bmp";
	//Disp::display(fileIn);
	
	bool useFileIn = true;
	
	MazeRunner* runner;
	
	vector<vector<unsigned char>> greyMap;
	vector<int> start;
	vector<int> end;
	
	if(useFileIn){
		Campus campus(fileIn);
		greyMap = campus.greyMap;
		runner = new MazeRunner(campus);
		start = vector<int>{campus.start[0], campus.start[1]};
		end = vector<int>{campus.end[0], campus.end[1]};
	}
	else{
		int mazeSize = 70;
		Maze maze = Maze(mazeSize, mazeSize);
		
		vector<vector<bool>> map = maze.getMap();
		vector<vector<int>> startEnd = maze.getStartEnd();
		
		start = startEnd.at(0);
		end = startEnd.at(1);
		
		
		runner = new MazeRunner(map, start, end);
		
		greyMap.reserve(map.size());
	
		for(int row = 0; row < map.size(); ++row){
			greyMap.push_back(vector<unsigned char>() );
			for(int col = 0; col < map[0].size(); ++col){
				greyMap.at(row).push_back( (unsigned char)(map[row][col]) - 1);
			}
		}
	}
	
	cout << "Start: (" << start.at(0) << ", " << start.at(1) << ")" << endl;
	cout << "End: (" << end.at(0) << ", " << end.at(1) << ")" << endl;
	
	bool solved = runner->solve();
	
	Path shortPath;
	if(solved){
		shortPath = runner->getShortestPath();
		
		PathWriter writer(runner, fileOut);
		
		vector<vector<unsigned char>> mapToPaint;
		mapToPaint.reserve(runner->getHeight());
		
		for(long row = 0; row < runner->getHeight(); ++row){
			mapToPaint.push_back(vector<unsigned char>(runner->getWidth()));
		}
		bool thick = false;//useFileIn;
		writer.drawOnMap(mapToPaint, runner->allBlocks, runner->getShortestPath(), start, end, thick);
		writer.encodeOneStep(mapToPaint, greyMap);
	}

	return 0;
}
