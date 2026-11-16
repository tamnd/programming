```bash run id=function-pointer-and-delegate-bash
add() {
  echo $(($1 + $2))
}

mul() {
  echo $(($1 * $2))
}

sub() {
  echo $(($1 - $2))
}

apply() {
  local f=$1
  shift
  "$f" "$@"
}

op=add
"$op" 6 7
op=mul
"$op" 6 7
apply sub 6 7
```

```output for=function-pointer-and-delegate-bash
13
42
-1
```

- Verdict: idiom. Bash has no function pointers. The variable `op` holds the name of a function, and `apply` runs the name that it gets as its first argument.
