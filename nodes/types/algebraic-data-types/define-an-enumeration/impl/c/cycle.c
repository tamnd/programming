// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

#include "cycle.h"

#include <string.h>

// C keeps no names for enumeration constants at run time, so a table
// holds them.
static const char *const names[DAY_COUNT] = {
    [MONDAY] = "monday",     [TUESDAY] = "tuesday", [WEDNESDAY] = "wednesday",
    [THURSDAY] = "thursday", [FRIDAY] = "friday",   [SATURDAY] = "saturday",
    [SUNDAY] = "sunday",
};

const char *day_name(enum day d) {
    if ((int)d < 0 || d >= DAY_COUNT) {
        return NULL;
    }
    return names[d];
}

size_t cycle(const char *start, size_t steps, const char **out) {
    for (int d = MONDAY; d < DAY_COUNT; d++) {
        if (strcmp(names[d], start) == 0) {
            for (size_t k = 0; k < steps; k++) {
                out[k] = day_name((enum day)((d + k) % DAY_COUNT));
            }
            return steps;
        }
    }
    return 0;
}
