```bash run id=regions
heap="" stack=""
while read -r range perms _ _ _ path; do
  start=$((16#${range%%-*}))
  case $path in
    "[heap]")  [[ -z $heap ]] && heap=$start && echo "$path $perms" ;;
    "[stack]") [[ -z $stack ]] && stack=$start && echo "$path $perms" ;;
    "[vdso]")  echo "$path $perms" ;;
  esac
done < /proc/$$/maps # $$ is the process ID of this shell
if (( heap < stack )); then
  echo "the heap is below the stack: true"
fi
```

```output for=regions
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

- Verdict: native. `$$` is the process ID of the shell, so `/proc/$$/maps` lists the regions of the shell itself.
