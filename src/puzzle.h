#pragma once

#include "board.h"
#include "bst.h"
#include "queue.h"
#include "types.h"

#include <inttypes.h>

typedef struct {
    board **data;
    move *moves;
    uint8_t size;
} path;

path *
create_path(unification_pattern *pattern, board *vertex);

void
destroy_path(path *p);

typedef struct {
    path *answer;
    unification_pattern *pattern;
    bst_node *state_space;
    queue *pool;
} solver;

solver *
create_solver();

void
destroy_solver(solver *s);

void
solve(solver *s);
