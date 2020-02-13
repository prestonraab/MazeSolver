#include "MazeRunner.h"


bool MazeRunner::removeUnlikelies(vector<vector<bool>>& m) {
	bool removedOne = false;
	unsigned long height = m.size();
	unsigned long percentile = height * 20 / 100 + 1;
	unsigned long width = m[0].size();
	bool center = false;
	bool right = false;
	bool lastCenterWasFalse = false;
	for (unsigned int i = 1; i + 1 < height; ++i) {
		center = m[i][0];
		right = m[i][1];
		for (unsigned int j = 1; j + 1 < width; ++j) {
			lastCenterWasFalse = !center;
			center = right;
			right = m[i][j + 1];
			if (!center && !(i == start.i && j == start.j) && !(i == finish.i && j == finish.j)) {
				int countFalse = lastCenterWasFalse ? 1 : 0;

				if (!right)
					if (++countFalse == 2)
						continue;

				if (!m[i + 1][j])
					if (++countFalse == 2)
						continue;

				if (!m[i - 1][j])
					if (++countFalse == 2)
						continue;
				
				m[i][j] = true;
				center = true;
				removedOne = true;
			}
		}
		if (i % percentile == 0)
			cout << i * 100 / height << fixed << setprecision(0) << "%  ";
	}
	cout << endl;
	return removedOne;
}



void MazeRunner::findConnections(vector<MajorBlock>& blocks) {
	unsigned long size = blocks.size();
	unsigned percentile = (unsigned)(size / 100 + 1);
	for (unsigned int x = 0; x < size; ++x) {
		MajorBlock& m = blocks.at(x);
		
		vector<MajorBlock> newConnections;
		
		for (unsigned int y = 0; y < m.connections.size(); ++y) {
			MajorBlock& t = m.connections.at(y);
			bool collided = false;
			
			if(t.i == 3 && t.j == 3){
				cout << "T: (" << t.i << ", " << t.j << ")" << endl;
				;
			}
			
			if (m.i == t.i && m.j == t.j) {
				collided = true;
			}
			else if (m.i == t.i) {  //horizontal
				int delJ = t.j - m.j;
				int sgnJ = sgn(delJ);
				for (int j = m.j + sgnJ; j != t.j; j += sgnJ) {
					if (map[m.i][j])
						collided = true;
				}
			}
			else if (m.j == t.j) {  //vertical
				int delI = t.i - m.i;
				int sgnI =  sgn(delI);
				for (int i = m.i + sgnI; i != t.i; i += sgnI) {
					if (map[i][m.j])
						collided = true;
				}
			}
			else {  //some diagonal
				Line line = Line(t.i, t.j, m.i, m.j);
				int down = (t.i > m.i) ? 1 : -1;
				int right = (t.j > m.j) ? 1 : -1;

				int i = m.i;
				int j = m.j + right;

				int originalPoint[] = { i,j };

				if (map[i][j] || map[i + down][j - right]) { //collide near m
					collided = true;
				}
				else if (map[t.i][t.j - right] || map[t.i - down][t.j]) {  //collide near j
					collided = true;
				}
				else { // m and j are far apart
					while (i != (t.i - down) || j != (t.j)) { //diagonal doesn't match t's diagonal
						int rightPoint[] = { i       , j + right };
						int downPoint[]  = { i + down, j };
						
						if (line.onPoint(downPoint)) {
							i += down;
							
							if (map[i][j]) {  //if square hits
								collided = true;
								break;
							}
						}
						else if (line.onSameSide(downPoint, originalPoint)) {    //try down
							i += down;
							
							if (map[i][j] || map[i + down][j - right]) {  //if diagonal hits
								collided = true;
								break;
							}
						}
						else if (line.onSameSide(rightPoint, originalPoint)) {   //try right
							j += right;
							
							if (map[i][j] || map[i + down][j - right]) {  //if diagonal hits
								collided = true;
								break;
							}
						}
					}
				}
			}
			if (collided == false) {
				newConnections.push_back(m.connections[y]);
//				m.connections.erase(m.connections.begin() + y);
//				y--;
			}
		}
		m.connections.clear();
		m.connections = newConnections;
		
		if (x % percentile == 0) {
			cout << x * 100 / size << " %" << endl;
		}
	}
}



Path MazeRunner::findBestPath(MajorBlock const &m, Path const &p) {
	vector<Path> paths = {};
	
	unsigned long findBlock = foundShortBlocks.size();
	for (unsigned x = 0; x < findBlock; x++) {
		if (foundShortBlocks[x] == m) {
			findBlock = x;
			break;
		}
	}
	if (findBlock != foundShortBlocks.size()) { //If we've gotten to this block already
		if(p.distance > foundShortPaths[findBlock].distance){ //A faster way
			return maxPath;
		}
		else if(p.distance < foundShortPaths[findBlock].distance){ //A slower way
			foundShortPaths.at(findBlock) = p;
		}
	}
	else{ //If this is the first time we've reached this block
		foundShortBlocks.push_back(m);
		foundShortPaths.push_back(p);
	}
	
	
	
	for (MajorBlock const &n : m.connections) {
		if (p.has(n))   //Don't step on a tile already stepped on
			continue;
//		if(m.i == 3 && m.j == 3){
//			cout << "N: (" << n.i << ", " << n.j << ")" << endl;
//			;
//		}
		
		if (mfinish == n) {  //If a connection hits the end, form a new path
			Path newPath = Path();
			newPath.add(n);
			newPath.add(m);
			//paths.push_back(newPath);
			cout << "Hit end" << endl;
			return newPath;
			
		}
		else {   //Otherwise continue search from the new connection
			Path trod = p;
			trod.add(m);
			Path bestPath = findBestPath(getBlock(n), trod);
			paths.push_back(bestPath);
		}
	}
	for (Path& path : paths) {
		path.add(m);
	}
	if(m.i == 3 && m.j == 3){
		;
	}
	Path shortest = shortestPath(paths);

	return shortest;
}

void MazeRunner::identifyCorners(vector<vector<bool>> m, vector<Corner>& c) {
	int percentile = height / 100 + 1;

	bool lastTopLeft = false;
	bool lastBottomLeft = false;

	for (int i = 0; i + 1 < height; i++) {
		lastTopLeft = m[i][0];
		lastBottomLeft = m[i + 1][0];
		for (int j = 0; j + 1 < width; j++) {
			int countTrue = 0;
			Corner::direction d = Corner::BR;
			if (lastTopLeft) {
				countTrue++;
			}
			lastTopLeft = m[i][j + 1];

			if (lastBottomLeft) {
				lastBottomLeft = m[i + 1][j + 1];
				if (++countTrue == 2)
					continue;
				d = Corner::TR;
			}

			lastBottomLeft = m[i + 1][j + 1];

			if (lastTopLeft) {
				if (++countTrue == 2)
					continue;
				d = Corner::BL;
			}
			if (lastBottomLeft) {
				if (++countTrue == 2)
					continue;
				d = Corner::TL;
			}
			if (countTrue == 1) {
				Corner corner = Corner(i, j, d);
				c.push_back(corner); //Add corner to the end of the list
			}
		}
		if (i % percentile == 0)
			cout << i * 100.0 / height << "%" << endl;
	}
}

