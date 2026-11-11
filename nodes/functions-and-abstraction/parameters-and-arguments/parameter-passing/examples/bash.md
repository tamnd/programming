```bash run id=positional
count_args() {
  echo "$# arguments, first: $1"
}

set_result() {
  printf -v "$1" 'done at step %s' "$2"
}

files=("a.txt" "my notes.txt")
count_args "${files[@]}"
set_result status 3
echo "$status"
```

```output for=positional
2 arguments, first: a.txt
done at step 3
```

- Verdict: native. A function gets its arguments as the positional parameters `$1`, `$2` and so on, and `$#` is their count. The arguments are strings, and they are passed by value.
- A function can only return an exit status. To return a string, it prints it, or it assigns to a variable whose name the caller passes. `printf -v` assigns to a variable by name. Bash 4.3 and later also have `local -n`, a name reference.
- There are no named parameters and no declared parameter list.

```bash wrong id=unquoted
count_args() {
  echo "$# arguments, first: $1"
}

files=("a.txt" "my notes.txt")
count_args ${files[@]}
```

```output for=unquoted
3 arguments, first: a.txt
```

Without quotes, the shell splits `my notes.txt` into two words. Write `"${files[@]}"` and `"$@"` to pass each element as one argument.
