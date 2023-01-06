/* bouncing - DVD logo animation for terminals.
   Copyright (C) 2023 Williams Ochoa

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <string.h>

#include "animation.h"


int main() {
    if (setup_tty() == -1) {
        fprintf(stderr, "Error: could not configure the terminal.\n");
        return EXIT_FAILURE;
    }
    srand(getpid());

    const char *logo[] = {
        "X   X DDD ",
        " X X  D  D",
        "  X   D  D",
        " X X  D  D",
        "X   X DDD "
    };
    printf(CHCUR(l) SWBUF(h) CLRSCR MOVCUR(1, 1));
    if (animate_logo(logo, strlen(*logo),
                     sizeof(logo) / sizeof(*logo), 1) == -1) {
        printf(CHCUR(h) MOVCUR(1, 1) CLRSCR SWBUF(l));
        fprintf(stderr, "Error: terminal size is too low.\n");
        restore_tty();
    }

    return EXIT_FAILURE;
}
