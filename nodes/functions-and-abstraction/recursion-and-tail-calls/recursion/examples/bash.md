```bash run id=hanoi
hanoi() {
    local n=$1 from=$2 to=$3 via=$4
    (( n == 0 )) && return
    hanoi $(( n - 1 )) "$from" "$via" "$to"
    echo "move disk $n from $from to $to"
    hanoi $(( n - 1 )) "$via" "$to" "$from"
}

factorial() {
    local n=$1 rest
    if (( n <= 1 )); then
        echo 1
        return
    fi
    rest=$(factorial $(( n - 1 )))
    echo $(( n * rest ))
}

hanoi 3 A C B
factorial 10
```

```output for=hanoi
move disk 1 from A to C
move disk 2 from A to B
move disk 1 from C to B
move disk 3 from A to C
move disk 1 from B to A
move disk 2 from B to C
move disk 1 from A to C
3628800
```

- Verdict: native. A shell function may call itself, and `local` gives each call its own variables.
- A function returns only an exit status. A value comes back through standard output and a command substitution, which runs the function in a subshell, or through a global variable.
- By default, Bash places no limit on the depth of recursion. The variable `FUNCNEST` sets a maximum nesting level, and a call beyond it aborts the command.
