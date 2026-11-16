```bash run id=function-call-bash
rectangle_area() {
  echo "computing $1 by $2"
  area=$(( $1 * $2 ))
}

rectangle_area 3 4
echo "area $area"
```

```output for=function-call-bash
computing 3 by 4
area 12
```

- Verdict: native. A call is a command: the function name, then the arguments with spaces and no parentheses. The body reads them as `$1` and `$2`. A Bash function gives back only an exit status, so this one puts its result in the variable `area`.
