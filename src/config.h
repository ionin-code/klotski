#pragma once

#include "types.h"

#include <inttypes.h>

#define BOARD_WIDTH 4
#define BOARD_HEIGHT 5

#define BLOCKS_AMOUNT 10

#define FREE_CELLS_AMOUNT 2

extern uint8_pair const block_dimensions[BLOCKS_AMOUNT];
extern point const block_coordinates[BLOCKS_AMOUNT];

extern uint8_t const target_block;
extern point const goal_cell;
