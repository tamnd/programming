```bash run id=loop
data=(1 2 3 4 5)
i=0
j=$(( ${#data[@]} - 1 ))
while (( i < j )); do
    t=${data[i]}
    data[i]=${data[j]}
    data[j]=$t
    (( i++, j-- ))
done
echo "${data[@]}"

count=0
word=imperative
for (( k = 0; k < ${#word}; k++ )); do
    case ${word:k:1} in
        [aeiou]) (( count++ )) ;;
    esac
done
echo "$count"
```

```output for=loop
5 4 3 2 1
5
```

- Verdict: native. A shell script is a sequence of commands with variables, loops and conditionals. Variables are global unless a function declares them `local`.
