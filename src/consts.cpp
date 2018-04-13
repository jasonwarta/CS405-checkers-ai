#include "consts.h"

const std::string START_BOARD_STRING = "rrrrrrrrrrrr________bbbbbbbbbbbb";

std::shared_ptr<MoveTable> RED_MOVE_PTR = std::make_shared<MoveTable>(RED_MOVE_BOARD);
std::shared_ptr<MoveTable> RED_JUMP_PTR = std::make_shared<MoveTable>(RED_JUMP_BOARD);
std::shared_ptr<MoveTable> BLACK_MOVE_PTR = std::make_shared<MoveTable>(BLACK_MOVE_BOARD);
std::shared_ptr<MoveTable> BLACK_JUMP_PTR = std::make_shared<MoveTable>(BLACK_JUMP_BOARD);
