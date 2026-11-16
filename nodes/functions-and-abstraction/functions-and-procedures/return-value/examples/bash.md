```bash run id=return-value-bash
absolute() {
  if (( $1 < 0 )); then
    echo $(( 0 - $1 ))
    return
  fi
  echo "$1"
}

absolute -7
echo $(( $(absolute 5) + 1 ))
```

```output for=return-value-bash
7
6
```

- Verdict: idiom. `return` gives only an exit status, so the function prints the value and the caller captures it with `$( )`.
