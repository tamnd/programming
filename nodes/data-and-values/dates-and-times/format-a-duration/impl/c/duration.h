// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

#ifndef FORMAT_DURATION_H
#define FORMAT_DURATION_H

// Room for a sign, the hours of any long long and ":MM:SS".
#define FORMAT_DURATION_SIZE 32

// Writes the duration as HH:MM:SS to out. The hours do not wrap at 24.
void format_duration(long long seconds, char out[FORMAT_DURATION_SIZE]);

#endif
