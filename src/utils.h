#pragma once

#include "types.h"
#include "board.h"

#include <stdbool.h>
#include <inttypes.h>

bool **
create_2d_bool_matrix(uint8_t rows, uint8_t cols);

void
destroy_2d_bool_matrix(bool **matrix);

bool
are_pairs_equal(uint8_pair a, uint8_pair b);

uint8_t *
create_uint8_array(uint8_t size);

void
destroy_uint8_array(uint8_t *array);

uint8_pair *
create_uint8_pair_array(uint8_t size);

void
destroy_uint8_pair_array(uint8_pair *array);

board **
create_board_ptr_array(uint8_t size);

void
destroy_board_ptr_array(board **array, uint8_t size);

move *
create_move_array(uint8_t size);

void
destroy_move_array(move *array);

void
swap_pairs(uint8_pair *a, uint8_pair *b);

bool
is_point_lesser(point a, point b);

void
sort_points(point *array, uint8_t size);

bool
is_board_lesser(board *a, board *b);

bool
are_points_equal(point a, point b);

bool
are_boards_equal(board *a, board *b);

void
print_unified_board(unification_pattern *pattern, board *b);

void
print_board(board *b);
