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


/* Configure the terminal to show the animation (and be restored if an
   interrupt signal is received).
   Return 0 (on success) or -1 (on failure). */
int setup_tty();

/* Restore the terminal configuration to its initial state.
   Return 0 (on success) or -1 (on failure). */
int restore_tty();


#if __cplusplus
}
#endif

#endif  /* BOUNCING_ANIMATION_H */
