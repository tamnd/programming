// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stddef.h>

// Writes template into out with $$ changed to $, and $name and ${name}
// changed to values[i] where names[i] is name. count is the length of names
// and values. Returns the length of the full result, as snprintf does: if it
// is size or more, the result was cut to fit. Returns -1 for an unknown name
// or a $ without a name.
int substitute(char *out, size_t size, const char *template, const char *const names[],
               const char *const values[], size_t count);

#endif
