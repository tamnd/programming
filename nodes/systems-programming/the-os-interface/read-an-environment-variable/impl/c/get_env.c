// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

#include "get_env.h"

#include <stdlib.h>

const char *get_env(const char *name, const char *default_value) {
    // getenv returns a null pointer when the variable is not set.
    const char *value = getenv(name);
    return value != NULL ? value : default_value;
}
