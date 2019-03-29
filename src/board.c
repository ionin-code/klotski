#include "board.h"
#include "utils.h"
#include "types.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static void
fill_block(bool **occupied, uint8_pair coordinates, uint8_pair dimensions) {
    assert(coordinates.first + dimensions.first <= BOARD_WIDTH &&
           coordinates.second + dimensions.second <= BOARD_HEIGHT);
    for (uint8_t i = coordinates.first; i < coordinates.first + dimensions.first; ++i) {
        for (uint8_t j = coordinates.second; j < coordinates.second + dimensions.second; ++j) {
            occupied[i][j] = true;
        }
    }
}

static void
fill_free_cells(board *b) {
    assert(b != NULL);

    bool **occupied = create_2d_bool_matrix(BOARD_WIDTH, BOARD_HEIGHT);

    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        fill_block(occupied, block_coordinates[i], block_dimensions[i]);
    }

    uint8_t free_cells_found = 0;
    for (uint8_t i = 0; i < BOARD_WIDTH; ++i) {
        for (uint8_t j = 0; j < BOARD_HEIGHT; ++j) {
            if (!occupied[i][j]) {
                b->free_cells[free_cells_found] = (uint8_pair) {
                        .first = i,
                        .second = j
                };
                ++free_cells_found;
            }
        }
    }
    destroy_2d_bool_matrix(occupied);

    assert(free_cells_found == FREE_CELLS_AMOUNT);
}

board *
create_board() {
    board *b = (board *) malloc(sizeof(*b));
    b->parent = NULL;
    b->depth = 0;

    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        b->blocks[i] = block_coordinates[i];
    }
    fill_free_cells(b);

    return b;
}

void
destroy_board(board *b) {
    free(b);
}

board *
copy_board(board *b) {
    assert(b != NULL);

    board *copy = (board *) malloc(sizeof(*copy));
    *copy = *b;
    return copy;
}

unification_pattern *
create_unification_pattern() {
    unification_pattern *pattern = (unification_pattern *) malloc(sizeof(*pattern));
    pattern->groups_amount = 1;
    for (uint8_t i = 1; i < BLOCKS_AMOUNT; ++i) {
        bool new_group = true;
        for (uint8_t j = 0; j < i; ++j) {
            if (are_pairs_equal(block_dimensions[i], block_dimensions[j])) {
                new_group = false;
                break;
            }
        }
        if (new_group) {
            ++pattern->groups_amount;
        }
    }

    pattern->group_dimensions = create_uint8_pair_array(pattern->groups_amount);
    pattern->group_sizes = create_uint8_array(pattern->groups_amount);
    pattern->group_begins = create_uint8_array(pattern->groups_amount);

    uint8_t already_created_groups = 1;
    pattern->group_dimensions[0] = block_dimensions[0];
    pattern->group_sizes[0] = 1;

    pattern->block_groups[0] = 0;

    for (uint8_t i = 1; i < BLOCKS_AMOUNT; ++i) {
        bool group_found = false;
        for (uint8_t j = 0; j < already_created_groups; ++j) {
            if (are_pairs_equal(block_dimensions[i], pattern->group_dimensions[j])) {
                ++pattern->group_sizes[j];
                pattern->block_groups[i] = j;
                group_found = true;
                break;
            }
        }
        if (!group_found) {
            pattern->group_dimensions[already_created_groups] = block_dimensions[i];
            pattern->group_sizes[already_created_groups] = 1;
            pattern->block_groups[i] = already_created_groups;
            ++already_created_groups;
        }
    }

    pattern->group_begins[0] = 0;
    for (uint8_t i = 1; i < pattern->groups_amount; ++i) {
        pattern->group_begins[i] = pattern->group_begins[i - 1] + pattern->group_sizes[i - 1];
    }

    return pattern;
}

void
destroy_unification_pattern(unification_pattern *pattern) {
    if (pattern) {
        destroy_uint8_array(pattern->group_sizes);
        destroy_uint8_array(pattern->group_begins);
        destroy_uint8_pair_array(pattern->group_dimensions);
    }
    free(pattern);
}

board *
convert_board(unification_pattern *pattern, board *b) {
    assert(pattern != NULL);
    assert(b != NULL);

    board *temp = (board *) malloc(sizeof(*temp));
    temp->parent = b->parent;
    temp->depth = b->depth;

    for (uint8_t i = 0; i < FREE_CELLS_AMOUNT; ++i) {
        temp->free_cells[i] = b->free_cells[i];
    }

    uint8_t *groups_counter = create_uint8_array(pattern->groups_amount);
    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        temp->blocks[
                pattern->group_begins[pattern->block_groups[i]] + groups_counter[pattern->block_groups[i]]
        ] = b->blocks[i];
        ++groups_counter[pattern->block_groups[i]];
    }
    destroy_uint8_array(groups_counter);

    mend_board(pattern, temp);
    return temp;
}

void
mend_board(unification_pattern *pattern, board *b) {
    assert(b != NULL);

    if (pattern != NULL) {
        for (uint8_t i = 0; i < pattern->groups_amount; ++i) {
            sort_points(&b->blocks[pattern->group_begins[i]], pattern->group_sizes[i]);
        }
    }
    sort_points(b->free_cells, FREE_CELLS_AMOUNT);
}
