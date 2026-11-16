#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-text-template/.

# substitute template [name value]...: print template with $$ changed to $,
# and $name and ${name} changed to the value of name. Each pass of the loop
# takes the text before the first $ and one placeholder, so a value is not
# read as a template. It does not use eval, which would run the commands in
# a template. An unknown name or a $ without a name returns 1.
substitute() {
  local rest=$1 out='' name
  shift
  local -A table=()
  while (($# >= 2)); do
    table[$1]=$2
    shift 2
  done
  local re='^([^$]*)\$(\$|\{([A-Za-z_][A-Za-z0-9_]*)\}|([A-Za-z_][A-Za-z0-9_]*))(.*)$'
  while [[ $rest == *'$'* ]]; do
    if ! [[ $rest =~ $re ]]; then
      printf 'a $ without a name\n' >&2
      return 1
    fi
    out+=${BASH_REMATCH[1]}
    name=${BASH_REMATCH[3]}${BASH_REMATCH[4]}
    rest=${BASH_REMATCH[5]}
    if [[ ${BASH_REMATCH[2]} == '$' ]]; then
      out+='$'
    elif [[ -v table[$name] ]]; then
      out+=${table[$name]}
    else
      printf 'unknown name: %s\n' "$name" >&2
      return 1
    fi
  done
  printf '%s\n' "$out$rest"
}
