//
//  Corner.h
//  MazeSolver
//
//  Created by Preston Raab on 2/10/20.
//  Copyright © 2020 Preston Raab. All rights reserved.
//

#ifndef Corner_h
#define Corner_h

#include <vector>

using std::vector;

#define SEARCH_RANGE 500

struct Corner {
	enum relation { same, adj, opp };

	enum direction { TL, TR, BR, BL, ALL };
	
	static relation getRelation(direction a, direction b) {
		if (a == b)
			return same;

		if (a - b == 2 || b - a == 2)
			return opp;
		else
			return adj;
	}
	
	Corner(int y, int x, direction dir): i(y), j(x), d(dir) {
	}
	
	
	
	static bool searchDirection(int i, int j, direction d, Corner const &cornerToSearch) {
		if ( (i - cornerToSearch.i > SEARCH_RANGE) || (cornerToSearch.i - i > SEARCH_RANGE))
			return false;
		if ( (j - cornerToSearch.j > SEARCH_RANGE) || (cornerToSearch.j - j > SEARCH_RANGE))
			return false;


		int iMult, jMult;
		if (d == ALL)
			return true;
		if (d == BL || d == BR)
			iMult = 1;
		else
			iMult = -1;
		if (d == TR || d == BR)
			jMult = 1;
		else
			jMult = -1;

		bool found = (cornerToSearch.i - i) * iMult >= 0 && (cornerToSearch.j - j) * jMult >= 0;
		if (i == cornerToSearch.i && j == cornerToSearch.j)
			found = false;
		return found;
	}
	
	int i;
	int j;
	direction d;
};

#endif /* Corner_h */
