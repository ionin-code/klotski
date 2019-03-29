#include "tests.h"
#include "config.h"
#include "board.h"
#include "utils.h"
#include "types.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

void
test_config() {
    assert(BLOCKS_AMOUNT > 0);
    assert(BOARD_HEIGHT > 0);
    assert(BOARD_WIDTH > 0);

    uint8_t occupied_cells = 0;

    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        assert(block_coordinates[i].first + block_dimensions[i].first <= BOARD_WIDTH);
        assert(block_coordinates[i].second + block_dimensions[i].second <= BOARD_HEIGHT);

        assert(block_dimensions[i].first == 1 || block_dimensions[i].first == 2);
        assert(block_dimensions[i].second == 1 || block_dimensions[i].second == 2);
        occupied_cells += block_dimensions[i].first * block_dimensions[i].second;
    }

    assert(goal_cell.first < BOARD_WIDTH);
    assert(goal_cell.second < BOARD_HEIGHT);

    assert(FREE_CELLS_AMOUNT == 2);
    assert(BOARD_WIDTH * BOARD_HEIGHT - occupied_cells == FREE_CELLS_AMOUNT);
}

static void
test_block(bool **occupied, uint8_pair coordinates, uint8_pair dimensions) {
    assert(coordinates.first + dimensions.first <= BOARD_WIDTH &&
           coordinates.second + dimensions.second <= BOARD_HEIGHT);
    for (uint8_t i = coordinates.first; i < coordinates.first + dimensions.first; ++i) {
        for (uint8_t j = coordinates.second; j < coordinates.second + dimensions.second; ++j) {
            assert(!occupied[i][j]);
            occupied[i][j] = true;
        }
    }
}

void
test_pattern(unification_pattern *pattern) {
    assert(pattern != NULL);
    assert(pattern->group_sizes[pattern->block_groups[target_block]] == 1);
    bool mask[BLOCKS_AMOUNT];
    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        mask[i] = false;
    }

    for (uint8_t i = 0; i < pattern->groups_amount; ++i) {
        for (uint8_t j = pattern->group_begins[i]; j < pattern->group_begins[i] + pattern->group_sizes[i]; ++j) {
            assert(j < BLOCKS_AMOUNT);
            assert(!mask[j]);
            mask[j] = true;
        }
    }
    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        assert(mask[i]);
    }
}

void
test_board(unification_pattern *pattern, board *b) {
    assert(pattern != NULL);
    assert(b != NULL);

    bool **occupied = create_2d_bool_matrix(BOARD_WIDTH, BOARD_HEIGHT);

    for (uint8_t i = 1; i < FREE_CELLS_AMOUNT; ++i) {
        assert(is_point_lesser(b->free_cells[i - 1], b->free_cells[i]));
        for (uint8_t j = 0; j < i; ++j) {
            assert(!are_points_equal(b->free_cells[j], b->free_cells[i]));
        }
    }

    for (uint8_t i = 0; i < pattern->groups_amount; ++i) {
        for (uint8_t j = 0; j < pattern->group_sizes[i]; ++j) {
            test_block(occupied, b->blocks[pattern->group_begins[i] + j], pattern->group_dimensions[i]);
            if (j != pattern->group_sizes[i] - 1) {
                assert(!is_point_lesser(
                        b->blocks[pattern->group_begins[i] + j + 1],
                        b->blocks[pattern->group_begins[i] + j]
                ));
            }
        }
    }

    uint8_t free_cells_amount = 0;
    for (uint8_t i = 0; i < BOARD_WIDTH; ++i) {
        for (uint8_t j = 0; j < BOARD_HEIGHT; ++j) {
            if (!occupied[i][j]) {
                bool cell_found = false;
                for (uint8_t k = 0; k < FREE_CELLS_AMOUNT; ++k) {
                    if (are_pairs_equal((uint8_pair) {.first = i, .second = j}, b->free_cells[k])) {
                        cell_found = true;
                        break;
                    }
                }
                assert(cell_found);
                ++free_cells_amount;
            }
        }
    }
    assert(free_cells_amount == FREE_CELLS_AMOUNT);

    destroy_2d_bool_matrix(occupied);

    uint8_t counter = 0;
    board *temp = b;
    while (temp->parent != NULL) {
        ++counter;
        temp = temp->parent;
    }
    assert(b->depth == counter);
}
