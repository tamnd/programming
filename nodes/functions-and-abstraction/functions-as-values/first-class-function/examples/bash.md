```bash run id=first-class-function-bash
double() {
  echo $(($1 * 2))
}

square() {
  echo $(($1 * $1))
}

add() {
  echo $(($1 + $2))
}

apply_twice() {
  local g=$1 x=$2
  "$g" "$("$g" "$x")"
}

f=double
"$f" 5

declare -A ops=([double]=double [square]=square)
"${ops[square]}" 4

apply_twice double 3

add5=(add 5)
"${add5[@]}" 10
```

```output for=first-class-function-bash
10
16
12
15
```

- Verdict: idiom. A Bash function is not a value. A variable holds the name of a function, and a command that starts with the variable calls it. The array `add5` holds a command name and its first argument, so it works as a function that takes one more argument.
