#pragma once
#ifndef CAMPUS_H
#define CAMPUS_H

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <fstream>
using std::ifstream;
using std::vector;
using std::cout;
using std::endl;

class Campus
{
private:
	ifstream readIntoImg1(const std::string& file, uint32_t &fileSize, uint32_t &dataOffset, int32_t &width, int32_t &height, uint32_t &depth);
	void readIntoImg2(std::vector<char> &img, ifstream &bmp, const std::string& file, long &dataSize, int32_t height, int32_t width, uint32_t depth);
public:
	vector<vector<bool>> campusMap;
	vector<vector<unsigned char>> greyMap;
	int found = 0;
	int start[2] = {0,0};
	int end[2] = {0,0};

	Campus(const char* fileIn) {
		//campusMap = readBMP("C:\\Users\\Preston Raab\\Desktop\\CampusMap.bmp");
		campusMap = readBMP(fileIn);
	}

	vector<vector<bool>> getMap() {
		return campusMap;
	}

	int getFound() {
		return found;
	}

	vector<vector<bool>> readBMP(const std::string& file);

};

#endif
