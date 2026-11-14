#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/merge-two-maps/.

# merge_maps KEY=VALUE... [-- KEY=VALUE...]
# The pairs before -- are the base map, and the pairs after it are the
# override map. Prints one line "KEY VALUE" for each key of either map, in
# ascending order of the key by its bytes. For a key in both maps, the value
# of the override map wins.
merge_maps() {
  local -A merged=()
  local arg
  for arg in "$@"; do
    if [[ $arg == -- ]]; then
      continue
    fi
    # The pairs after -- come later, so their values replace the base values.
    merged[${arg%%=*}]=${arg#*=}
  done
  for arg in "${!merged[@]}"; do
    printf '%s %s\n' "$arg" "${merged[$arg]}"
  done | LC_ALL=C sort
}
