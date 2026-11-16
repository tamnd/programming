```bash run id=refutable-bash
for line in width=80 oops height=24; do
  case $line in
    *=*) echo "${line%%=*} -> ${line#*=}" ;;
    *) echo "skip: $line" ;;
  esac
done
```

```output for=refutable-bash
width -> 80
skip: oops
height -> 24
```

- Verdict: absent. A `case` glob such as `*=*` can fail, but it binds no names. The clause takes the parts with parameter expansion. The last pattern `*` matches each string.
