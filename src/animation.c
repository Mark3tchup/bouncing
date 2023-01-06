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


#define RAND_DISP (rand() % 2 ? 1 : -1)

/* Just start drawing the DVD logo endlessly.
   Return -1 on failure. */
int animate_logo(const char **logo, const int width, const int height,
                 const unsigned int delay) {
    struct winsize tty_size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &tty_size);

    int x = 2 + rand() % (tty_size.ws_col - width - 1),
        y = 2 + rand() % (tty_size.ws_row - height - 1),
        dispx = RAND_DISP, dispy = RAND_DISP;
    for (; tty_size.ws_col > width && tty_size.ws_row > width;
         x += dispx, y += dispy) {
        printf(CLRSCR);
        for (int i = 0; i < height; ++i) {
            printf(MOVCUR(%d, %d) "%s", y + i, x, logo[i]);
        }
        fflush(stdout);
        sleep(delay);
        if (x == 1 || x + width == tty_size.ws_col + 1) {
            dispx = -dispx;
        }
        if (y == 1 || y + height == tty_size.ws_row) {
            dispy = -dispy;
        }
    }

    return -1;
}
