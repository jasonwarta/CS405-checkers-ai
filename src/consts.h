#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

#include <vector>
using std::vector;
#include <string>
using std::string;

const vector<char> START_BOARD { 
	'r','r','r','r',
	'r','r','r','r',
	'r','r','r','r',
	'_','_','_','_',
	'_','_','_','_',
	'b','b','b','b',
	'b','b','b','b',
	'b','b','b','b' };

string START_BOARD_STRING = "rrrrrrrrrrrr________bbbbbbbbbbbb";
// string START_BOARD_STRING = "rrrrr_r_rbrr_br___b_bb_b_b_bbbbb";
// string START_BOARD_STRING = "rrrrr_rrrrr__brrb____b_bbbbbbbbb";

std::vector<std::vector<int>> redMoveBoard { 
	{4, 5},		//0
	{5, 6},
	{6, 7},
	{7, -1},
	{-1, 8},
	{8, 9},		//5
	{9, 10},
	{10, 11},
	{12, 13},
	{13, 14},
	{14, 15},	//10
	{15, -1},
	{-1, 16},
	{16, 17},
	{17, 18},
	{18, 19},	//15
	{20, 21},
	{21, 22},
	{22, 23},
	{23, -1},
	{-1, 24},	//20
	{24, 25},
	{25, 26},
	{26, 27},
	{28, 29},
	{29, 30},	//25
	{30, 31},
	{31, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},	//30
	{-1, -1} };

std::vector<std::vector<int>> redJumpBoard { {-1, 9},	//0
	{8, 10},
	{9, 11},
	{10, -1},
	{-1, 13},
	{12, 14},	//5
	{13, 15},
	{14, -1},
	{-1, 17},
	{16, 18},
	{17, 19},	//10
	{18, -1},
	{-1, 21},
	{20, 22},
	{21, 23},
	{22, -1},	//15
	{-1, 25},
	{24, 26},
	{25, 27},
	{26, -1},
	{-1, 29},	//20
	{28, 30},
	{29, 31},
	{30, -1},
	{-1, -1},
	{-1, -1},	//25
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},	//30
	{-1, -1} };	 

std::vector<std::vector<int>> blackMoveBoard { 
	{-1, -1},	//0
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, 0},
	{0, 1},		//5
	{1, 2},
	{2, 3},
	{4, 5},
	{5, 6},
	{6, 7},		//10
	{7, -1},
	{-1, 8},
	{8, 9},
	{9, 10},
	{10, 11},	//15
	{12, 13},
	{13, 14},
	{14, 15},
	{15, -1},
	{-1, 16},	//20
	{16, 17},
	{17, 18},
	{18, 19},
	{20, 21},
	{21, 22},	//25
	{22, 23},
	{23, -1},
	{-1, 24},
	{24, 25},
	{25, 26},	//30
	{26, 27} };

std::vector<std::vector<int>> blackJumpBoard { 
	{-1, -1},	//0
	{1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},	//5
	{-1, -1},
	{-1, -1},
	{-1, 1},
	{0, 2},
	{1, 3},		//10
	{2, -1},
	{-1, 5},
	{4, 6},
	{5, 7},
	{6, -1},	//15
	{-1, 9},
	{8, 10},
	{9, 11},
	{10, -1},
	{-1, 13},	//20
	{12, 14},
	{13, 15},
	{14, -1},
	{-1, 17},
	{16, 18},	//25
	{17, 19},
	{18, -1},
	{-1, 21},
	{20, 22},
	{21, 23},	//30
	{22, -1} };

#endif