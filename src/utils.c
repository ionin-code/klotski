#include "utils.h"
#include "types.h"

#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

bool **
create_2d_bool_matrix(uint8_t rows, uint8_t cols) {
    assert(rows != 0);
    assert(cols != 0);

    bool **matrix = (bool **) malloc(rows * sizeof(*matrix));
    matrix[0] = (bool *) malloc(rows * cols * sizeof(*matrix[0]));
    for (uint8_t i = 1; i < rows; ++i) {
        matrix[i] = matrix[i - 1] + cols;
    }

    for (uint8_t i = 0; i < rows; ++i) {
        for (uint8_t j = 0; j < cols; ++j) {
            matrix[i][j] = false;
        }
    }

    return matrix;
}

void
destroy_2d_bool_matrix(bool **matrix) {
    if (matrix) {
        free(matrix[0]);
    }
    free(matrix);
}

bool
are_pairs_equal(uint8_pair a, uint8_pair b) {
    return a.first == b.first && a.second == b.second;
}

uint8_t *
create_uint8_array(uint8_t size) {
    assert(size != 0);

    uint8_t *array = (uint8_t *) malloc(size * sizeof(*array));
    for (uint8_t i = 0; i < size; ++i) {
        array[i] = 0;
    }
    return array;
}

void
destroy_uint8_array(uint8_t *array) {
    free(array);
}

uint8_pair *
create_uint8_pair_array(uint8_t size) {
    assert(size != 0);

    uint8_pair *array = (uint8_pair *) malloc(size * sizeof(*array));
    for (uint8_t i = 0; i < size; ++i) {
        array[i] = (uint8_pair) {
                .first = 0,
                .second = 0
        };
    }
    return array;
}

void
destroy_uint8_pair_array(uint8_pair *array) {
    free(array);
}

void
swap_pairs(uint8_pair *a, uint8_pair *b) {
    assert(a != NULL);
    assert(b != NULL);

    uint8_pair temp = *a;
    *a = *b;
    *b = temp;
}

bool
is_point_lesser(point a, point b) {
    if (a.first < b.first) {
        return true;
    }
    if (a.first == b.first) {
        return a.second < b.second;
    }
    return false;
}

void
sort_points(point *array, uint8_t size) {
    assert(array != NULL);
    assert(size != 0);

    for (uint8_t i = 0; i < size - 1; i++) {
        for (uint8_t j = 0; j < size - i - 1; j++) {
            if (is_point_lesser(array[j + 1], array[j])) {
                swap_pairs(&array[j + 1], &array[j]);
            }
        }
    }
}

bool
is_board_lesser(board *a, board *b) {
    assert(a != NULL);
    assert(b != NULL);

    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        if (is_point_lesser(a->blocks[i], b->blocks[i])) {
            return true;
        }
        if (!are_points_equal(a->blocks[i], b->blocks[i])) {
            return false;
        }
    }
    return false;
}

bool
are_points_equal(point a, point b) {
    return are_pairs_equal(a, b);
}

bool
are_boards_equal(board *a, board *b) {
    if (a == NULL) {
        return b == NULL;
    }
    if (b == NULL) {
        return a == NULL;
    }
    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        if (!are_points_equal(a->blocks[i], b->blocks[i])) {
            return false;
        }
    }
    return true;
}

void
print_unified_board(unification_pattern *pattern, board *b) {
    for (uint8_t i = 0; i < pattern->groups_amount; ++i) {
        printf(
                "Group #%d: %dx%d\n", i + 1,
                pattern->group_dimensions[i].first,
                pattern->group_dimensions[i].second
        );
        for (uint8_t j = pattern->group_begins[i]; j < pattern->group_begins[i] + pattern->group_sizes[i]; ++j) {
            printf("\t%d %d\n", b->blocks[j].first, b->blocks[j].second);
        }
    }
}

void
print_board(board *b) {
    int8_t mask[BOARD_HEIGHT][BOARD_WIDTH];
    for (uint8_t i = 0; i < BOARD_HEIGHT; ++i) {
        for (uint8_t j = 0; j < BOARD_WIDTH; ++j) {
            mask[i][j] = -1;
        }
    }

    for (uint8_t i = 0; i < BLOCKS_AMOUNT; ++i) {
        point coordinates = b->blocks[i];
        for (uint8_t x = coordinates.first; x < coordinates.first + block_dimensions[i].first; ++x) {
            for (uint8_t y = coordinates.second; y < coordinates.second + block_dimensions[i].second; ++y) {
                mask[y][x] = i;
            }
        }
    }

    for (uint8_t i = 0; i < BOARD_HEIGHT; ++i) {
        for (uint8_t j = 0; j < BOARD_WIDTH; ++j) {
            int8_t current = mask[i][j];
            current == -1 ? printf("*") : printf("%d", current + 1);
            printf("\t");
        }
        printf("\n");
    }
}

board **
create_board_ptr_array(uint8_t size) {
    board **array = (board **) malloc(size * sizeof(*array));
    for (uint8_t i = 0; i < size; ++i) {
        array[i] = NULL;
    }
    return array;
}

void
destroy_board_ptr_array(board **array, uint8_t size) {
    if (array) {
        for (uint8_t i = 0; i < size; ++i) {
            destroy_board(array[i]);
        }
    }
    free(array);
}

move *
create_move_array(uint8_t size) {
    move *array = (move *) malloc(size * sizeof(*array));
    return array;
}

void
destroy_move_array(move *array) {
    free(array);
}
