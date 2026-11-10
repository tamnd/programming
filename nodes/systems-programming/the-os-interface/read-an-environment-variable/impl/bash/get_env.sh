#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-an-environment-variable/.

# get_env NAME DEFAULT
# Prints the value of the variable NAME, or DEFAULT when NAME is not set.
# An empty value is a value, so it does not give DEFAULT.
get_env() {
  local name=$1 default=$2
  # ${!name} reads the variable whose name is in $name.
  # "-" without ":" gives the default only for an unset variable.
  printf '%s' "${!name-$default}"
}
