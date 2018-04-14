#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED


#include <vector>
#include <string>
#include <memory>

const int8_t LOSS_VAL = -1;
const int8_t DRAW_VAL = 0;
const int8_t WIN_VAL = 2;

const int8_t NUM_THREADS = 3;
const int POPULATION_SIZE = 30;

const int8_t MATCHES = 5;

const std::vector<uint> NET_SIZE {32, 40, 10, 1};



typedef const std::vector<std::vector<int>> MoveTable;
extern const std::string START_BOARD_STRING;

extern std::shared_ptr<MoveTable> RED_MOVE_PTR;
extern std::shared_ptr<MoveTable> RED_JUMP_PTR;
extern std::shared_ptr<MoveTable> BLACK_MOVE_PTR;
extern std::shared_ptr<MoveTable> BLACK_JUMP_PTR;



const std::vector<std::vector<int>> RED_MOVE_BOARD { 
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

const std::vector<std::vector<int>> RED_JUMP_BOARD { 
	{-1, 9},	//0
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

const std::vector<std::vector<int>> BLACK_MOVE_BOARD { 
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

const std::vector<std::vector<int>> BLACK_JUMP_BOARD { 
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

const std::vector<std::vector<uint>> NN91_NODE_LOCATIONS {
	{0, 4, 5, 8},		//0, 3x3 tiles start
	{0, 1, 5, 8, 9},
	{1, 5, 6, 9},
	{1, 2, 6, 9, 10},
	{2, 6, 7, 10},
	{2, 3, 7, 10, 11},
	{4, 5, 8, 12, 13},
	{5, 8, 9, 13},
	{5, 6, 9, 13, 14},
	{6, 9, 10, 14},
	{6, 7, 10, 14, 15},
	{7, 10, 11, 15},
	{8, 12, 13, 16},
	{8, 9, 13, 16, 17},
	{9, 13, 14, 17},
	{9, 10, 14, 17, 18},
	{10, 14, 15, 18},
	{10, 11, 15, 18, 19},
	{12, 13, 16, 20, 21},
	{13, 16, 17, 21},
	{13, 14, 17, 21, 22},
	{14, 17, 18, 22},
	{15, 15, 18, 22, 23},
	{15, 18, 19, 23},
	{16, 20, 21, 24},
	{16, 17, 21, 24, 25},
	{17, 21, 22, 25},
	{17, 18, 22, 25, 26},
	{18, 22, 23, 26},
	{18, 19, 23, 26, 27},
	{21, 24, 25, 29},
	{20, 21, 24, 28, 29},
	{21, 22, 25, 29, 30},
	{22, 25, 26, 30},
	{22, 23, 26, 30, 31},
	{23, 26, 27, 31},		//36 total 3x3 tiles end

	{0, 1, 4, 5, 8, 9, 12, 13}, // 4x4 tiles start
	{0, 1, 5, 6, 8, 9, 13, 14},
	{1, 2, 5, 6, 9, 10, 13, 14},
	{1, 2, 6, 7, 9, 10, 14, 15},
	{2, 3, 6, 7, 10, 11, 14, 15},
	{4, 5, 8, 9, 12, 13, 16, 17},
	{5, 6, 8, 9, 13, 14, 16, 17},
	{5, 6, 9, 10, 13, 14, 17, 18},
	{6, 7, 9, 10, 14, 15, 17, 18},
	{6, 7, 10, 11, 14, 15, 18, 19},
	{8, 9, 12, 13, 16, 17, 20, 21},
	{8, 9, 13, 14, 16, 17, 21, 22},
	{9, 10, 13, 14, 17, 18, 21, 22},
	{9, 10, 14, 15, 17, 18, 22, 23},
	{10, 11, 14, 15, 18, 19, 22, 23},
	{12, 13, 16, 17, 20, 21, 24, 25},
	{13, 14, 16, 17, 21, 22, 24, 25},
	{13, 14, 17, 18, 21, 22, 25, 26},
	{14, 15, 17, 18, 22, 23, 25, 26},
	{14, 15, 18, 19, 22, 23, 26, 27},
	{16, 17, 20, 21, 24, 25, 28, 29},
	{16, 17, 21, 22, 24, 25, 29, 30},
	{17, 18, 21, 22, 25, 26, 29, 30},
	{17, 18, 22, 23, 25, 26, 30, 31},
	{18, 19, 22, 23, 26, 27, 30, 31}, //25 total 4x4 tiles end

	{0, 1, 4, 5, 6, 8, 9, 12, 13, 14, 16, 17},	// 5x5 tiles start
	{0, 1, 2, 5, 6, 8, 9, 10, 13, 14, 16, 17, 18},
	{1, 2, 5, 6, 7, 9, 10, 13, 14, 15, 17, 18},
	{1, 2, 3, 6, 7, 9, 10, 11, 14, 15, 17, 18, 19},
	{4, 5, 6, 8, 9, 12, 13, 14, 16, 17, 20, 21, 22},
	{5, 6, 8, 9, 10, 13, 14, 16, 17, 18, 21, 22},
	{5, 6, 7, 9, 10, 13, 14, 15, 17, 18, 21, 22, 23},
	{6, 7, 9, 10, 11, 14, 15, 17, 18, 19, 22, 23},
	{8, 9, 12, 13, 14, 16, 17, 20, 21, 22, 24, 25},
	{8, 9, 10, 13, 14, 16, 17, 18, 21, 22, 24, 25, 26},
	{9, 10, 13, 14, 15, 17, 18, 21, 22, 23, 25, 26},
	{9, 10, 11, 14, 15, 17, 18, 19, 22, 23, 25, 26, 27},
	{12, 13, 14, 16, 17, 20, 21, 22, 24, 25, 28, 29, 30},
	{13, 14, 16, 17, 18, 21, 22, 24, 25, 26, 29, 30},
	{13, 14, 15, 17, 18, 21, 22, 23, 25, 26, 29, 30, 31},
	{14, 15, 17, 18, 19, 22, 23, 25, 26, 17, 30, 31},	// 16 total 5x5 tiles end

	{0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22}, // 6x6 tiles start
	{0, 1, 2, 5, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 18, 21, 22, 23},
	{1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18, 19, 21, 22, 23},
	{4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22, 24, 25, 26},
	{5, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 18, 21, 22, 23, 24, 25, 26},
	{5, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18, 19, 21, 22, 23, 24, 25, 26},
	{8, 9, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22, 24, 25, 26, 28, 29, 30},
	{8, 9, 10, 13, 14, 15, 16, 17, 18, 21, 22, 23, 24, 25, 26, 29, 30, 31},
	{9, 10, 11, 13, 14, 15, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 31}, // 9 total 6x6 tiles end

	{0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26}, 	// 7x7 tiles start
	{0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27},
	{4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31},
	{5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 29, 30, 31},	// 7x7 end

	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31} // 8x8, all
	// 91 total
};




#endif