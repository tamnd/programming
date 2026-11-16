#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/log-an-error/.

# read_ports prints the good ports on the standard output, one on each line,
# and writes the log lines on the standard error.

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

# quote VALUE
# Prints VALUE in double quotes, and escapes the characters that could end
# the line or the value.
quote() {
  local s=$1
  s=${s//'\'/'\\'}
  s=${s//'"'/'\"'}
  s=${s//$'\n'/'\n'}
  s=${s//$'\r'/'\r'}
  s=${s//$'\t'/'\t'}
  printf '"%s"' "$s"
}

# read_ports TEXT...
read_ports() {
  local line=0 ok=0 text
  for text in "$@"; do
    line=$((line + 1))
    if parse_port "$text"; then
      echo "$PORT"
      ok=$((ok + 1))
    else
      # The error is handled here: it is logged once, and the status of
      # read_ports does not show it.
      printf 'level=ERROR msg="read port failed" line=%d err=%s\n' "$line" "$(quote "$ERROR_MESSAGE")" >&2
    fi
  done
  printf 'level=INFO msg="ports read" ok=%d total=%d\n' "$ok" "$#" >&2
}
