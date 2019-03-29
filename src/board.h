#pragma once

#include "config.h"
#include "types.h"

#include <inttypes.h>

typedef struct board board;
struct board {
    board *parent;
    uint8_t depth;
    point blocks[BLOCKS_AMOUNT];
    point free_cells[FREE_CELLS_AMOUNT];
};

board *
create_board();

void
destroy_board(board *b);

board *
copy_board(board *b);

typedef struct {
    uint8_t block_groups[BLOCKS_AMOUNT];
    uint8_pair *group_dimensions;
    uint8_t *group_sizes;
    uint8_t groups_amount;
    uint8_t *group_begins;
} unification_pattern;

unification_pattern *
create_unification_pattern();

void
destroy_unification_pattern(unification_pattern *pattern);

board *
convert_board(unification_pattern *pattern, board *b);

void
mend_board(unification_pattern *pattern, board *b);
