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

#ifndef BOUNCING_ANIMATION_H
#define BOUNCING_ANIMATION_H

#if __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>


#define CSI "\x1b["  /* Prefix for many virtual terminal commands. */
#define SWBUF(m) CSI "?1049" #m         /* Switch to the specified buffer
                                           (h = alternate, l = main). */
#define CLRSCR CSI "2J"                 /* Clear the entire screen buffer. */
#define MOVCUR(x, y) CSI #y ";" #x "H"  /* Set the cursor position. */
#define CHCUR(m) CSI "?25" #m           /* Change the cursor mode
                                           (h = enabled, l = disabled). */
#define CHFMT(c) CSI #c "m"             /* Change the format of the screen
                                           and text. */


/* Configure the terminal to show the animation (and be restored if an
   interrupt signal is received).
   Return 0 (on success) or -1 (on failure). */
int setup_tty(void);

/* Restore the terminal configuration to its initial state.
   Return 0 (on success) or -1 (on failure). */
int restore_tty(void);


/* Just start drawing the DVD logo endlessly.
   Return -1 on failure. */
int animate_logo(const char **logo, const int width, const int height,
                 const unsigned int delay);


#if __cplusplus
}
#endif

#endif  /* BOUNCING_ANIMATION_H */
