#include "puzzle.h"

#include <stdlib.h>

int
main() {
    solver *s = create_solver();
    solve(s);
    destroy_solver(s);

    return EXIT_SUCCESS;
}
