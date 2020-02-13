#include "Campus.h"

vector<vector<bool>> Campus::readBMP(const std::string& file)
{
	static constexpr size_t HEADER_SIZE = 54;
	
	uint32_t fileSize;
	uint32_t dataOffset;
	int32_t width;
	int32_t height;
	uint32_t depth;
	long dataSize;
	
	
	ifstream bmp = readIntoImg1(file, fileSize, dataOffset, width, height, depth);
	
	std::vector<char> img(dataOffset - HEADER_SIZE);
	
	readIntoImg2(img, bmp, file, dataSize, height, width, depth);
	
	bmp.close();

	std::cout << "fileSize: " << fileSize << std::endl;
	std::cout << "dataOffset: " << dataOffset << std::endl;
	std::cout << "width: " << width << std::endl;
	std::cout << "height: " << height << std::endl;
	std::cout << "depth: " << depth << "-bit" << std::endl;

	int bytesPerPixel = depth / 8;
	int extraBit = (4 - width % 4) % 4;


	char temp = 0;

	vector<vector<unsigned char>> greyscaleMap(height, vector<unsigned char>(width, 0));

	
	
	
	cout << "Reading image into map" << endl;
	int i = 0;
	int j = width - 1;
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;

	int f = 0;
	double imageAverage = 0;
	long numPixelsLookedAt = 0;
	
	for (signed long x = dataSize - bytesPerPixel + extraBit; x >= 0; x -= bytesPerPixel)
	{
		// + extraBit
		temp = img[x];
		img[x] = img[x + 2];
		img[x + 2] = temp;

		red = (unsigned char)(img[x] & 0xff);
		green = (unsigned char)(img[x + 1] & 0xff);
		blue = (unsigned char)(img[x + 2] & 0xff);

		if (red == 255 && green == 0 && blue > 250){
			f++;
		}
		if (red < 10 && green > 245 && blue < 10) { //Green == start
			start[0] = i;
			start[1] = j;
		}
		if (red < 10 && green < 10 && blue > 245) { //Blue == end
			end[0] = i;
			end[1] = j;
		}
		
		greyscaleMap[i][j] = (unsigned char)(red * 0.299 + green * 0.587 + blue * 0.114);
		numPixelsLookedAt += 1;
		imageAverage += (greyscaleMap[i][j] - imageAverage) / numPixelsLookedAt;
		//cout << imageAverage << " ";
		
		j--;

		if (j == -1 ) {
			j = width - 1 + extraBit;
			i++;
			if (i % (height / 100 + 1) == 0) {
				cout << (dataSize - x) * 100 / dataSize << "%  ";
				if (i % (height / 10 + 1) == 0) {
					cout << endl;
				}
			}
		}
	}
	

	vector<vector<bool>> map(height + extraBit, vector<bool>(width + extraBit, 1));
	
	for(int row = 0; row < height; ++row){
		for(int col = 0; col < width; ++col){
			map[row][col] = (greyscaleMap[row][col] < imageAverage);
		}
	}
	
	if(start[0] == 0 && start[1] == 0){
		while(map[start[0]][start[1]]){
			start[0] += 1;
			start[1] += 1;
		}
	}
	
	if(end[0] == 0 && end[1] == 0){
		end[0] = height - 1;
		end[1] = width - 1;
		while(map[end[0]][end[1]]){
			end[0] -= 1;
			end[1] -= 1;
		}
	}
	
	map[start[0]][start[1]] = false;
	map[end[0]][end[1]] = false;
	
	found = f;
	greyMap = greyscaleMap;
	return map;
}

ifstream Campus::readIntoImg1(const std::string& file, uint32_t &fileSize, uint32_t &dataOffset, int32_t &width, int32_t &height, uint32_t &depth)
{
	static constexpr size_t HEADER_SIZE = 54;
	
	ifstream bmp(file, std::ios::binary);

	std::array<char, HEADER_SIZE> header;
	bmp.read(header.data(), header.size());
	
	
	fileSize = *reinterpret_cast<uint32_t*>(&header[2]);
	dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
	width = *reinterpret_cast<int32_t*>(&header[18]);
	height = *reinterpret_cast<int32_t*>(&header[22]);
	height = abs(height);
	depth = *reinterpret_cast<uint16_t*>(&header[28]);
	
	return bmp;
}

void Campus::readIntoImg2(std::vector<char> &img, ifstream &bmp, const std::string& file, long &dataSize, int32_t height, int32_t width, uint32_t depth)
{
	bmp.read(img.data(), img.size());

	dataSize = ((width * (depth/8) + 3) & (~3)) * height;
	img.resize(dataSize);
	bmp.read(img.data(), img.size());
	
}
