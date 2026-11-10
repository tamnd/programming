#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/run-a-subprocess/.

# run_program STATUS_VAR OUT_VAR ERR_VAR PROGRAM [ARG...]
# Runs PROGRAM with the arguments and stores its exit status, its standard
# output and its standard error in the three variables. It returns 0, or 1
# with a message on standard error when PROGRAM is not found.
run_program() {
  local __status_var=$1 __out_var=$2 __err_var=$3 __path __dir __status __out __err
  shift 3
  # type -P searches PATH for a file. It skips functions and builtins, so
  # "printf" runs /usr/bin/printf and not the builtin.
  if ! __path=$(type -P -- "$1"); then
    printf 'run_program: %s: not found\n' "$1" >&2
    return 1
  fi
  __dir=$(mktemp -d) || return 1
  # The || keeps a status that is not 0 from stopping a caller with set -e.
  __status=0
  "$__path" "${@:2}" </dev/null >"$__dir/out" 2>"$__dir/err" || __status=$?
  # $(...) deletes trailing newlines, so add a character and remove it.
  __out=$(cat "$__dir/out" && printf x) && __out=${__out%x}
  __err=$(cat "$__dir/err" && printf x) && __err=${__err%x}
  rm -rf "$__dir"
  printf -v "$__status_var" '%s' "$__status"
  printf -v "$__out_var" '%s' "$__out"
  printf -v "$__err_var" '%s' "$__err"
}
