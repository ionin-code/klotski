#pragma once

#include <inttypes.h>

typedef struct {
    uint8_t block;
    uint8_t action;
} move;

#define ACTIONS_AMOUNT 4

#define MOVE_UP 0
#define MOVE_RIGHT 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3

typedef struct {
    uint8_t first;
    uint8_t second;
} uint8_pair;

typedef uint8_pair point;
