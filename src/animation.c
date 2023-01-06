/* bouncing - DVD logo animation for terminals.
   Copyright (C) 2023 Williams Ochoa

   This file is part of bouncing.

   bouncing is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <signal.h>

#include <sys/ioctl.h>
#include <termios.h>

#include "animation.h"


/* Safely exit if an interrupt signal is received. */
void sigint_handler(int sig) {
    restore_tty();
    /* Restore the screen format.
       write() is signal-safe (according to the manpage 'signal-safety')
       so let's use it instead of printf(). */
    write(STDOUT_FILENO,
          MOVCUR(1, 1)  /* 6 bytes. */
          CLRSCR        /* 4 bytes. */
          SWBUF(l)      /* 8 bytes. */
          CHCUR(h),     /* 6 bytes. */
          24);          /* 6 + 4 + 8 + 6 = 24 bytes. */
    /* Unlike exit(), _Exit() is signal-safe :-) */
    _Exit(sig == SIGINT ? EXIT_SUCCESS : sig);
}


/* Configure the terminal to show the animation (and be restored if an
   interrupt signal is received).
   Return 0 (on success) or -1 (on failure). */
int setup_tty() {
    struct termios tty;
    if (tcgetattr(STDIN_FILENO, &tty) == -1
        || signal(SIGINT, sigint_handler) == SIG_ERR) {
        return -1;
    }
    tty.c_lflag &= ~ECHO & ~ICANON;
    return tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

/* Restore the terminal configuration to its initial state.
   Return 0 (on success) or -1 (on failure). */
int restore_tty() {
    struct termios tty;
    if (tcgetattr(STDIN_FILENO, &tty)) {
        return -1;
    }
    tty.c_lflag ^= ECHO ^ ICANON;
    return tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}


static volatile sig_atomic_t must_resize;

/* What to do if the terminal window is resized. */
void sigwinch_handler(int sig) {
    must_resize = true;
    signal(sig, sigwinch_handler);
}

#define RAND_DISP (rand() % 2 ? 1 : -1)
#define START_POS(tmp_var, limit) \
    2 + rand() % ((tmp_var = limit - 1) != 0 ? tmp_var : 2)
#define RAND_FG 31 + rand() % 7

/* Just start drawing the DVD logo endlessly.
   Return -1 on failure. */
int animate_logo(const char **logo, const int width, const int height,
                 const unsigned int delay) {
    struct winsize tty_size;
    signal(SIGWINCH, sigwinch_handler);
    int x = 0, y = 0, dispx = RAND_DISP, dispy = RAND_DISP;
    for (must_resize = true;; x += dispx, y += dispy) {
        if (must_resize) {
            ioctl(STDIN_FILENO, TIOCGWINSZ, &tty_size);
            if (tty_size.ws_col - 2 < width || tty_size.ws_row - 2 < height) {
                return -1;
            }
            must_resize = false;
            printf(CHFMT(%d), RAND_FG);
        }
        if (x == 0 || y == 0 || x + width > tty_size.ws_col + 1
            || y + height > tty_size.ws_row + 1) {
            int size_tmp;
            x = START_POS(size_tmp, tty_size.ws_col - width);
            y = START_POS(size_tmp, tty_size.ws_row - height);
        }
        printf(CLRSCR);
        for (int i = 0; i < height; ++i) {
            printf(MOVCUR(%d, %d) "%s", y + i, x, logo[i]);
        }
        fflush(stdout);
        sleep(delay);
        if (x == 1 || x + width == tty_size.ws_col + 1) {
            dispx = -dispx;
            printf(CHFMT(%d), RAND_FG);
        }
        if (y == 1 || y + height == tty_size.ws_row + 1) {
            dispy = -dispy;
            printf(CHFMT(%d), RAND_FG);
        }
    }
}
