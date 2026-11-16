#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wrap-an-error-with-context/.

# Bash has no error values. Each function returns a status and puts the
# message in the global variable ERROR_MESSAGE. Call the functions without
# $( ), because a command substitution runs in a subshell.

# parse_port TEXT
# Sets PORT and returns 0, or sets ERROR_MESSAGE and returns 1 or 2.
parse_port() {
  local text=$1 value=0 i
  if [[ ! $text =~ ^[0-9]+$ ]]; then
    ERROR_MESSAGE="not a number: '$text'"
    return 1
  fi
  for ((i = 0; i < ${#text}; i++)); do
    value=$((value * 10 + ${text:i:1}))
    if ((value > 65535)); then
      break
    fi
  done
  if ((value == 0 || value > 65535)); then
    ERROR_MESSAGE="out of range: $text"
    return 2
  fi
  PORT=$value
}

# read_port WHERE TEXT
# Like parse_port, but puts "WHERE: read port: " in front of the message and
# keeps the status of parse_port.
read_port() {
  local status
  parse_port "$2" && return 0
  status=$?
  ERROR_MESSAGE="$1: read port: $ERROR_MESSAGE"
  return "$status"
}

# check_setting WHERE TEXT
# Prints "port N", or "error: " and the message.
check_setting() {
  if read_port "$1" "$2"; then
    echo "port $PORT"
  else
    echo "error: $ERROR_MESSAGE"
  fi
}
