#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-command-line-arguments/.

# parse_args ARG... parses the arguments of "count [-v] [-n N] [--] [FILE...]".
# It sets the global variables verbose (true or false), limit, and the array
# files. On a usage error it prints a message on stderr and returns 2.
parse_args() {
  verbose=false
  limit=10
  files=()

  # getopts knows only one-letter options, so a first loop changes each long
  # option to its short form. It stops at the first operand or at "--".
  local -a short=()
  while (($# > 0)); do
    case $1 in
      --) break ;;
      --verbose) short+=(-v) ;;
      --limit)
        short+=(-n)
        (($# > 1)) && { short+=("$2"); shift; }
        ;;
      --limit=*) short+=(-n "${1#--limit=}") ;;
      --*)
        echo "count: unknown option $1" >&2
        return 2
        ;;
      -n)
        short+=(-n)
        (($# > 1)) && { short+=("$2"); shift; }
        ;;
      -?*) short+=("$1") ;;
      *) break ;;
    esac
    shift
  done
  set -- "${short[@]}" "$@"

  local opt OPTARG OPTIND=1
  while getopts ":vn:" opt; do
    case $opt in
      v) verbose=true ;;
      n)
        if [[ ! $OPTARG =~ ^[0-9]+$ ]]; then
          echo "count: limit '$OPTARG' is not a decimal number" >&2
          return 2
        fi
        limit=$((10#$OPTARG))
        ;;
      :)
        echo "count: option -$OPTARG needs a value" >&2
        return 2
        ;;
      *)
        echo "count: unknown option -$OPTARG" >&2
        return 2
        ;;
    esac
  done
  shift $((OPTIND - 1))
  files=("$@")
}
