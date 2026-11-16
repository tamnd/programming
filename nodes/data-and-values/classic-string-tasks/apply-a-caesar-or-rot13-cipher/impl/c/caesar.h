// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

#ifndef CAESAR_H
#define CAESAR_H

// Moves each letter A to Z and a to z of text shift places along the
// alphabet, from z back to a, in place. Other bytes do not change, so UTF-8
// text keeps its other characters. A negative shift decodes, and
// caesar(text, 13) is ROT13.
void caesar(char *text, long long shift);

#endif
