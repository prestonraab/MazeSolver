//
//  PathWriter.cpp
//  MazeSolver
//
//  Created by Preston Raab on 2/11/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#include "PathWriter.hpp"


void PathWriter::drawOnMap(vector<vector<unsigned char>>& mapToPaint, vector<MajorBlock> blocks, Path p, vector<int> start, vector<int> end, bool thick) {

	p.drawOnMap(mapToPaint, thick);

	for (unsigned int k = 0; k < blocks.size(); k++) {
		int i = blocks[k].i;
		int j = blocks[k].j;
		if(i == INT_MAX){
			continue;
		}
		mapToPaint[i][j] = 'C';
		if (p.has(blocks[k])) {
			mapToPaint[i][j] = 'P';
		}
	}
	
	mapToPaint.at(end[0]).at(end[1]) = 'O';
	mapToPaint.at(start[0]).at(start[1]) = 'O';
	
//	for(int r = -1; r < 2; ++r){
//		for(int c = -1; c < 2; ++c){
//			if((end[0] + r >= 0) && (end[0] + r < mapToPaint.size()) &&
//			   (end[1] + c >= 0) && (end[1] + c < mapToPaint[0].size())){
//				mapToPaint.at(end[0] + r).at(end[1] + c) = 'G';
//			}
//			if(start[0] + r >= 0 && start[0] + r < mapToPaint.size() &&
//			   start[1] + c >= 0 && start[1] + c < mapToPaint[0].size()){
//				mapToPaint[start[0] + r][start[1] + c] = 'G';
//			}
//
//		}
//	}
}

void PathWriter::drawOnMap(vector<vector<unsigned char>>& mapToPaint, Path p, bool thick) {
	//int pathCount = 0;

	p.drawOnMap(mapToPaint, thick);

	for (unsigned int k = 0; k < p.getSize(); k++) {
		int i = p.steps[k].i;
		int j = p.steps[k].j;
		if(i == INT_MAX){
			continue;
		}
		mapToPaint[i][j] = 'P';
	}
}

void PathWriter::encodeOneStep(vector<vector<unsigned char>>& paintedMap, vector<vector<unsigned char>> greyMap) {
	vector<unsigned char> image;
	unsigned long height = runner->getHeight();
	unsigned long width = runner->getWidth();
	image.reserve(width * height);


	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			switch (paintedMap[i][j])
			{
			case 'G':
				image.push_back(0);
				image.push_back(255);
				image.push_back(0);
				break;
			case 'P':
				image.push_back(0);
				image.push_back(0);
				image.push_back(255);
				break;
			case 'C':
				image.push_back(255);
				image.push_back(0);
				image.push_back(255);
				break;
			case 'O':
				image.push_back(255);
				image.push_back(165);
				image.push_back(0);
				break;
			default:
				char greyVal = greyMap[i][j];

				image.push_back(greyVal);
				image.push_back(greyVal);
				image.push_back(greyVal);
			}

			image.push_back(white);
		}
	}
	//Encode the image
	unsigned error = lodepng::encode(fileOut, image, (unsigned)width, (unsigned)height);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
