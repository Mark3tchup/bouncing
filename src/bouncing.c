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

#include <limits.h>
#include <string.h>

#include <getopt.h>

#include "animation.h"


#define USAGE_FMT "Usage: %s [-d delay] [-h] [-q] [-v]\n"
#define HELP_MSG USAGE_FMT "\n"                           \
    "Options:\n"                                          \
    "  -d delay  Seconds between each frame refresh\n"    \
    "            (one by default).\n"                     \
    "  -h        Display this help message.\n"            \
    "  -q        Do not display an informative message\n" \
    "            before the animation.\n"                 \
    "  -v        Display program version information.\n"
#define PROGRAM_VERSION "0.1.0"
#define VERSION_MSG "bouncing v" PROGRAM_VERSION "\n"                 \
    "Copyright (C) 2023 Williams Ochoa\n"                             \
    "This program comes with ABSOLUTELY NO WARRANTY.\n"               \
    "This is free software, and you are welcome to redistribute it\n" \
    "under certain conditions; see LICENSE for details.\n"

int main(int argc, char *argv[]) {
    /* Parse the arguments passed through the CLI. */
    unsigned long delay = 1;
    bool show_help = true;
    int opt;
    while ((opt = getopt(argc, argv, "d:hqv")) != -1) {
        switch (opt) {
            case 'd':
                if ((delay = strtoul(optarg, NULL, 10)) == 0
                    || delay > UINT_MAX) {
                    fprintf(stderr, "%s: invalid delay value\n", *argv);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                printf(HELP_MSG, *argv);
                exit(EXIT_SUCCESS);
                break;
            case 'q':
                show_help = false;
                break;
            case 'v':
                printf(VERSION_MSG);
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, USAGE_FMT, *argv);
                exit(EXIT_FAILURE);
                break;
        }
    }
    if (optind < argc) {
        fprintf(stderr, "%s: non-expected positional argument(s)\n" USAGE_FMT,
                *argv, *argv);
        return EXIT_FAILURE;
    }

    if (setup_tty() == -1) {
        fprintf(stderr, "%s: could not configure the terminal\n", *argv);
        return EXIT_FAILURE;
    }
    srand(getpid());
    printf(CHCUR(l) SWBUF(h) CLRSCR MOVCUR(1, 1));
    if (show_help) {
        printf("Press Ctrl+C to quit the program.");
        fflush(stdout);
        sleep(1);
    }

    const char *logo[] = {
        "  DDDDDDDVVV       VVVDDDDDDD ",
        " DDDDDDDDDVVV     VVVDDDDDDDDD",
        "      DDDDVVV     VVV     DDDD",
        " DDD   DDDVVV     VVVDDD   DDD",
        " DDD   DDD VVV   VVV DDD   DDD",
        "DDD   DDD   VVV VVV DDD   DDD ",
        "DDDDDDDD     VVVVV  DDDDDDDD  ",
        "DDDDDD        VVV   DDDDDD    ",
        "               V              ",
        "     V   V II DD  EEE OOO     ",
        "      V V  II D D E   O O     ",
        "       V   II DD  EEE OOO     "
    };
    if (animate_logo(logo, strlen(*logo), sizeof(logo) / sizeof(*logo),
                     (unsigned int)delay) == -1) {
        printf(CHFMT(0) CHCUR(h) MOVCUR(1, 1) CLRSCR SWBUF(l));
        restore_tty();
        fflush(stdout);
        fprintf(stderr, "%s: terminal size is too low\n", *argv);
    }

    return EXIT_FAILURE;
}
