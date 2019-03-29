#include "config.h"

#include <inttypes.h>

uint8_pair const block_dimensions[BLOCKS_AMOUNT] = {
        (uint8_pair) {
                .first = 1,
                .second = 2
        },
        (uint8_pair) {
                .first = 2,
                .second = 2
        },
        (uint8_pair) {
                .first = 1,
                .second = 2
        },
        (uint8_pair) {
                .first = 1,
                .second = 2
        },
        (uint8_pair) {
                .first = 2,
                .second = 1
        },
        (uint8_pair) {
                .first = 1,
                .second = 2
        },
        (uint8_pair) {
                .first = 1,
                .second = 1
        },
        (uint8_pair) {
                .first = 1,
                .second = 1
        },
        (uint8_pair) {
                .first = 1,
                .second = 1
        },
        (uint8_pair) {
                .first = 1,
                .second = 1
        }
};
point const block_coordinates[BLOCKS_AMOUNT] = {
        (point) {
                .first = 0,
                .second = 0
        },
        (point) {
                .first = 1,
                .second = 0
        },
        (point) {
                .first = 3,
                .second = 0
        },
        (point) {
                .first = 0,
                .second = 2
        },
        (point) {
                .first = 1,
                .second = 2
        },
        (point) {
                .first = 3,
                .second = 2
        },
        (point) {
                .first = 1,
                .second = 3
        },
        (point) {
                .first = 2,
                .second = 3
        },
        (point) {
                .first = 0,
                .second = 4
        },
        (point) {
                .first = 3,
                .second = 4
        }
};

uint8_t const target_block = 1;
uint8_pair const goal_cell = (uint8_pair) {
        .first = 1,
        .second = 3
};
