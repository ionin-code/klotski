#include "bst.h"
#include "board.h"
#include "utils.h"

#include <assert.h>
#include <stdlib.h>

bst_node *
create_bst(board *value) {
    bst_node *node = (bst_node *) malloc(sizeof(*node));
    node->key = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

bst_node *
insert(bst_node *root, board *value) {
    if (root == NULL) {
        return create_bst(value);
    }
    if (is_board_lesser(value, root->key)) {
        root->left = insert(root->left, value);
    } else if (is_board_lesser(root->key, value)) {
        root->right = insert(root->right, value);
    }
    return root;
}

bst_node *
find(bst_node *root, board *value) {
    if (root == NULL || are_boards_equal(root->key, value)) {
        return root;
    }
    if (is_board_lesser(root->key, value)) {
        return find(root->right, value);
    }
    return find(root->left, value);
}

void
destroy_bst(bst_node *root) {
    if (root) {
        destroy_board(root->key);
        destroy_bst(root->left);
        destroy_bst(root->right);
    }
    free(root);
}
