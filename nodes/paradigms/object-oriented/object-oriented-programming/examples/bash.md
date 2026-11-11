```bash run id=dispatch
circle_area() { echo $(( 3 * $1 * $1 )); }
square_area() { echo $(( $1 * $1 )); }

a_kind=circle a_size=1
b_kind=square b_size=2

for obj in a b; do
    k=${obj}_kind s=${obj}_size
    echo "${!k} $("${!k}_area" "${!s}")"
done
```

```output for=dispatch
circle 3
square 4
```

- Verdict: absent. Bash has no objects, classes or methods. A script can keep the fields of an object in variables with a common prefix and pick a function from a name, as above, but this is a convention with no help from the shell.
