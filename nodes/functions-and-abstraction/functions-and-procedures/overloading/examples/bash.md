```bash run id=overloading-bash
show() {
  if [ "$#" -eq 2 ]; then
    echo "two ints $1 $2"
  elif [[ $1 =~ ^-?[0-9]+$ ]]; then
    echo "int $1"
  else
    echo "string $1"
  fi
}

show 42
show hi
show 3 4
```

```output for=overloading-bash
int 42
string hi
two ints 3 4
```

- Verdict: absent. Bash functions have no parameter list, so they cannot be overloaded. One function checks `$#` and the form of its first argument.
