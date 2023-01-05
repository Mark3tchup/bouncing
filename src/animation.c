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

#include <termios.h>
#include <unistd.h>

#include "animation.h"


/* Safely exit if an interrupt signal is received. */
void sigint_handler(int sig) {
    restore_tty();
    _Exit(sig == SIGINT ? 0 : sig);
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
