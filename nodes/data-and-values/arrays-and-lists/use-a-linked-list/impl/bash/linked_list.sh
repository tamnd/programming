#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-linked-list/.

# Bash has no pointers, so the list keeps its nodes in three arrays. A node
# is an index: val[i] is its value, and prv[i] and nxt[i] are the indexes of
# the nodes before and after it, or -1 at an end.

# _ll_unlink I: takes node I out of the list. It uses the arrays and the
# head and tail of the caller, because Bash variables have dynamic scope.
_ll_unlink() {
  local i=$1 p=${prv[$1]} q=${nxt[$1]}
  if ((p < 0)); then head=$q; else nxt[p]=$q; fi
  if ((q < 0)); then tail=$p; else prv[q]=$p; fi
  unset 'val[i]' 'prv[i]' 'nxt[i]'
}

# run_ops OP...
# Runs the operations on a new linked list and prints its items on one line,
# separated by spaces. Returns 2 if an operation is not valid.
run_ops() {
  local -a val=() prv=() nxt=() out=()
  local head=-1 tail=-1 id=0 op name v i
  for op in "$@"; do
    read -r name v <<<"$op"
    case $name in
      push_front)
        val[id]=$v prv[id]=-1 nxt[id]=$head
        if ((head < 0)); then tail=$id; else prv[head]=$id; fi
        head=$id id=$((id + 1)) ;;
      push_back)
        val[id]=$v nxt[id]=-1 prv[id]=$tail
        if ((tail < 0)); then head=$id; else nxt[tail]=$id; fi
        tail=$id id=$((id + 1)) ;;
      pop_front) if ((head >= 0)); then _ll_unlink "$head"; fi ;;
      pop_back) if ((tail >= 0)); then _ll_unlink "$tail"; fi ;;
      remove)
        for ((i = head; i >= 0; i = nxt[i])); do
          if ((val[i] == v)); then
            _ll_unlink "$i"
            break
          fi
        done ;;
      *)
        printf 'bad operation: %s\n' "$op" >&2
        return 2 ;;
    esac
  done
  for ((i = head; i >= 0; i = nxt[i])); do
    out+=("${val[i]}")
  done
  local IFS=' '
  printf '%s\n' "${out[*]}"
}
