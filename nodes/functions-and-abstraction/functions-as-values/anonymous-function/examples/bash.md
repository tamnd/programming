```bash run id=anonymous-function-bash
square() {
  echo $(($1 * $1))
}

map() {
  local f=$1 x out=()
  shift
  for x in "$@"; do
    out+=("$("$f" "$x")")
  done
  echo "${out[@]}"
}

by_length() {
  local w
  for w in "$@"; do
    echo "${#w} $w"
  done | sort -n | cut -d' ' -f2 | paste -sd' ' -
}

by_length house car bike
map square 1 2 3

n=5
add() {
  echo $(($1 + n))
}
add 10
```

```output for=anonymous-function-bash
car bike house
1 4 9
15
```

- Verdict: absent. Every Bash function has a name. The program passes the name `square` to `map`, and `add` reads the global variable `n`.
