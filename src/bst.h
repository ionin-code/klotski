#pragma once

#include "board.h"

#include <inttypes.h>

typedef struct bst_node bst_node;
struct bst_node {
    board *key;

    bst_node *left;
    bst_node *right;
};


bst_node *
create_bst(board *value);

bst_node *
insert(bst_node *root, board *value);

bst_node *
find(bst_node *root, board *value);

void
destroy_bst(bst_node *root);
