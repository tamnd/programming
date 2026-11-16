#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-custom-error-type/.

# Bash has no error types. parse_port reports an error with its exit status
# and puts the fields of the error in two global variables. Call it without
# $( ), because a command substitution runs in a subshell, and the variables
# that it sets do not come back.

# parse_port TEXT
# Sets PORT on success and returns 0. Sets PORT_ERROR_TEXT and
# PORT_ERROR_REASON and returns 1 on an error.
parse_port() {
  local text=$1 value=0 i
  PORT_ERROR_TEXT=$text
  if [[ -z $text ]]; then
    PORT_ERROR_REASON="empty"
    return 1
  fi
  if [[ ! $text =~ ^[0-9]+$ ]]; then
    PORT_ERROR_REASON="not a number"
    return 1
  fi
  for ((i = 0; i < ${#text}; i++)); do
    value=$((value * 10 + ${text:i:1}))
    if ((value > 65535)); then
      PORT_ERROR_REASON="out of range"
      return 1
    fi
  done
  if ((value == 0)); then
    PORT_ERROR_REASON="out of range"
    return 1
  fi
  PORT=$value
  return 0
}

# check_port TEXT
# Prints "port N", or the message of the error.
check_port() {
  if parse_port "$1"; then
    echo "port $PORT"
  else
    echo "invalid port '$PORT_ERROR_TEXT': $PORT_ERROR_REASON"
  fi
}
