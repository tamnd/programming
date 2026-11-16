```bash run id=function-bash
rectangle_area() {
  echo $(( $1 * $2 ))
}

greeting() {
  echo "Hello, $1!"
}

rectangle_area 3 4
message=$(greeting "Ada")
echo "$message"
```

```output for=function-bash
12
Hello, Ada!
```

- Verdict: native. A Bash function reads its arguments as `$1` and `$2`, and gives back a result by printing it. The caller captures the output with `$( )`.
