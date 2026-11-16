#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-stack/.

# balanced TEXT: exits with status 0 if the brackets of TEXT match and nest
# correctly, and with status 1 if they do not. An indexed array is the stack.
balanced() {
  local text=$1 c want i
  local -a stack=()
  for ((i = 0; i < ${#text}; i++)); do
    c=${text:i:1}
    case $c in
      '(' | '[' | '{') stack+=("$c") ;;
      ')' | ']' | '}')
        case $c in
          ')') want='(' ;;
          ']') want='[' ;;
          *) want='{' ;;
        esac
        ((${#stack[@]} > 0)) || return 1
        [[ ${stack[-1]} == "$want" ]] || return 1
        unset 'stack[-1]' # the quotes stop pathname expansion
        ;;
    esac
  done
  ((${#stack[@]} == 0))
}
