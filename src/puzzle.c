#include "puzzle.h"
#include "board.h"
#include "utils.h"
#include "tests.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static board *
validate(unification_pattern *pattern, board *b, uint8_t block, uint8_t action, uint8_pair dims) {
    assert(block < BLOCKS_AMOUNT);
    assert(action < ACTIONS_AMOUNT);
    assert(b != NULL);

    board *temp = NULL;

    uint8_pair old_block = b->blocks[block];

    switch (action) {
        case MOVE_UP: {
            if (old_block.second == 0) {
                break;
            }
            if (dims.first == 1) {
                if (b->free_cells[0].first == old_block.first &&
                    b->free_cells[0].second == old_block.second - 1) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    --new_block->second;
                    temp->free_cells[0].second += dims.second;
                } else if (b->free_cells[1].first == old_block.first &&
                           b->free_cells[1].second == old_block.second - 1) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    --new_block->second;
                    temp->free_cells[1].second += dims.second;
                }
            } else { // dims.first == 2
                if (b->free_cells[0].first == old_block.first &&
                    b->free_cells[0].second == old_block.second - 1 &&
                    b->free_cells[1].first == old_block.first + 1 &&
                    b->free_cells[1].second == old_block.second - 1) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    --new_block->second;
                    temp->free_cells[0].second += dims.second;
                    temp->free_cells[1].second += dims.second;
                }
            }
            break;
        }
        case MOVE_RIGHT: {
            if (old_block.first == BOARD_WIDTH - dims.first) {
                break;
            }
            if (dims.second == 1) {
                if (b->free_cells[0].second == old_block.second &&
                    b->free_cells[0].first == old_block.first + dims.first) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    ++new_block->first;
                    temp->free_cells[0].first -= dims.first;
                } else if (b->free_cells[1].second == old_block.second &&
                           b->free_cells[1].first == old_block.first + dims.first) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    ++new_block->first;
                    temp->free_cells[1].first -= dims.first;
                }
            } else { // dims.second == 2
                if (b->free_cells[0].second == old_block.second &&
                    b->free_cells[0].first == old_block.first + dims.first &&
                    b->free_cells[1].second == old_block.second + 1 &&
                    b->free_cells[1].first == old_block.first + dims.first) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    ++new_block->first;
                    temp->free_cells[0].first -= dims.first;
                    temp->free_cells[1].first -= dims.first;
                }
            }
            break;
        }
        case MOVE_DOWN: {
            if (old_block.second == BOARD_HEIGHT - dims.second) {
                break;
            }
            if (dims.first == 1) {
                if (b->free_cells[0].first == old_block.first &&
                    b->free_cells[0].second == old_block.second + dims.second) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    ++new_block->second;
                    temp->free_cells[0].second -= dims.second;
                } else if (b->free_cells[1].first == old_block.first &&
                           b->free_cells[1].second == old_block.second + dims.second) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    ++new_block->second;
                    temp->free_cells[1].second -= dims.second;
                }
            } else { //dims.first == 2
                if (b->free_cells[0].first == old_block.first &&
                    b->free_cells[0].second == old_block.second + dims.second &&
                    b->free_cells[1].first == old_block.first + 1 &&
                    b->free_cells[1].second == old_block.second + dims.second) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    ++new_block->second;
                    temp->free_cells[0].second -= dims.second;
                    temp->free_cells[1].second -= dims.second;
                }
            }
            break;
        }
        case MOVE_LEFT: {
            if (old_block.first == 0) {
                break;
            }
            if (dims.second == 1) {
                if (b->free_cells[0].second == old_block.second &&
                    b->free_cells[0].first == old_block.first - 1) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    --new_block->first;
                    temp->free_cells[0].first += dims.first;
                } else if (b->free_cells[1].second == old_block.second &&
                           b->free_cells[1].first == old_block.first - 1) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    --new_block->first;
                    temp->free_cells[1].first += dims.first;
                }
            } else { // dims.second == 2
                if (b->free_cells[0].second == old_block.second &&
                    b->free_cells[0].first == old_block.first - 1 &&
                    b->free_cells[1].second == old_block.second + 1 &&
                    b->free_cells[1].first == old_block.first - 1) {
                    temp = copy_board(b);
                    point *new_block = &temp->blocks[block];
                    --new_block->first;
                    temp->free_cells[0].first += dims.first;
                    temp->free_cells[1].first += dims.first;
                }
            }
            break;
        }
        default: {
            fprintf(stderr, "Unknown action in validate()\n");
            exit(EXIT_FAILURE);
        }
    }

    if (temp != NULL) {
        temp->parent = b;
        temp->depth = (uint8_t) (b->depth + 1);
        mend_board(pattern, temp);
    }
    return temp;
}

path *
create_path(unification_pattern *pattern, board *vertex) {
    test_board(pattern, vertex);

    path *p = (path *) malloc(sizeof(*p));
    p->size = vertex->depth;
    p->data = create_board_ptr_array((uint8_t) (p->size + 1));
    p->moves = create_move_array(p->size);

    board *current = vertex;
    for (uint8_t i = 0; i < p->size; ++i) {
        p->data[p->size - i] = current;
        assert(current->parent != NULL);
        current = current->parent;
    }
    p->data[0] = create_board();

    for (uint8_t i = 0; i < vertex->depth; ++i) {
        bool completed = false;
        board *prev = p->data[i];

        for (uint8_t block = 0; block < BLOCKS_AMOUNT && !completed; ++block) {
            for (uint8_t action = 0; action < ACTIONS_AMOUNT && !completed; ++action) {
                board *candidate = validate(NULL, prev, block, action, block_dimensions[block]);

                if (candidate != NULL) {
                    board *temp = convert_board(pattern, candidate);
                    test_board(pattern, temp);

                    if (are_boards_equal(temp, p->data[i + 1])) {
                        completed = true;
                        p->data[i + 1] = candidate;
                        p->moves[i] = (move) {.action = action, .block = block};
                    } else {
                        destroy_board(candidate);
                    }
                    destroy_board(temp);
                }
            }
        }
        assert(completed);
    }

    return p;
}

void
destroy_path(path *p) {
    if (p) {
        destroy_move_array(p->moves);
        destroy_board_ptr_array(p->data, (uint8_t) (p->size + 1));
    }
    free(p);
}

solver *
create_solver() {
    test_config();

    solver *s = (solver *) malloc(sizeof(*s));

    s->pattern = create_unification_pattern();
    test_pattern(s->pattern);

    s->pool = create_queue();

    s->state_space = NULL;

    return s;
}

void
destroy_solver(solver *s) {
    if (s) {
        destroy_queue(s->pool);
        destroy_bst(s->state_space);
        destroy_unification_pattern(s->pattern);
        destroy_path(s->answer);
    }
    free(s);
}

static board *
fill_state_space(solver *s, bool full, uint16_t *states) {
    assert(s != NULL);
    assert(states != NULL);

    *states = 0;

    board *res = NULL;
    while (s->pool->front != NULL) {
        board *current = dequeue(s->pool)->key;
        if (res == NULL && are_pairs_equal(
                current->blocks[s->pattern->group_begins[s->pattern->block_groups[target_block]]],
                goal_cell
        )) {
            res = current;
            if (!full) {
                break;
            }
        }

        for (uint8_t i = 0; i < s->pattern->groups_amount; ++i) {
            for (uint8_t j = 0; j < s->pattern->group_sizes[i]; ++j) {
                for (uint8_t k = 0; k < ACTIONS_AMOUNT; ++k) {
                    board *candidate = validate(s->pattern, current, s->pattern->group_begins[i] + j, k,
                                                s->pattern->group_dimensions[i]);
                    if (candidate != NULL) {
                        if (find(s->state_space, candidate) == NULL) {
                            enqueue(s->pool, candidate);
                            insert(s->state_space, candidate);
                            ++(*states);
                        } else {
                            destroy_board(candidate);
                        }
                    }
                }
            }
        }
    }

    return res;
}

void
solve(solver *s) {
    assert(s != NULL);

    board *temp = create_board();
    board *converted = convert_board(s->pattern, temp);

    test_board(s->pattern, converted);
    destroy_board(temp);

    s->state_space = insert(s->state_space, converted);
    enqueue(s->pool, converted);

    uint16_t states = 0;
    board *res = fill_state_space(s, true, &states);
    printf("%d states have been analysed.\n", states);
    if (res != NULL) {
        printf("Puzzle has been solved in %d steps.\n", res->depth);
        path *p = create_path(s->pattern, res);
        for (uint8_t i = 0; i <= p->size; ++i) {
            printf("Step #%d", i);
            if (i != 0) {
                printf(": ");
                switch (p->moves[i - 1].action) {
                    case MOVE_UP: {
                        printf("↑");
                        break;
                    }
                    case MOVE_RIGHT: {
                        printf("→");
                        break;
                    }
                    case MOVE_DOWN: {
                        printf("↓");
                        break;
                    }
                    case MOVE_LEFT: {
                        printf("←");
                        break;
                    }
                    default: {
                        fprintf(stderr, "Unknown action in main()\n");
                        exit(EXIT_FAILURE);
                    }
                }
                printf(" %d", p->moves[i - 1].block + 1);
            }
            printf("\n");
            print_board(p->data[i]);
            printf("\n");
        }
    } else {
        printf("Puzzle is unsolvable.\n");
    }
}
