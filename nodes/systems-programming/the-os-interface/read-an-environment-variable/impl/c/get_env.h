// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

#ifndef GET_ENV_H
#define GET_ENV_H

// get_env returns the value of the environment variable name, or
// default_value when it is not set. An empty value is a value, so it does not
// give default_value. The result points into the environment: do not change
// it, and copy it before a call to setenv, unsetenv or putenv.
const char *get_env(const char *name, const char *default_value);

#endif
