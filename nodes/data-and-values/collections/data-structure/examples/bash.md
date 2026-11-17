```bash run id=two-layouts
array=(10 20 30 40 50)

# A linked list in two arrays: value[k] and next[k]; -1 ends the list.
value=() next=()
head=-1
for (( i = ${#array[@]} - 1; i >= 0; i-- )); do
  k=${#value[@]}
  value[k]=${array[i]}
  next[k]=$head
  head=$k
done

echo "array[3] = ${array[3]} (0 steps)"
n=$head steps=0
for (( i = 3; i > 0; i-- )); do
  n=${next[n]}
  steps=$(( steps + 1 ))
done
echo "linked[3] = ${value[n]} ($steps steps)"

k=${#value[@]}
value[k]=5 next[k]=$head head=$k
out=()
for (( n = head; n != -1; n = next[n] )); do
  out+=("${value[n]}")
done
echo "after push_front: ${out[*]}"
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: idiom. Bash has indexed and associative arrays and nothing else. A linked structure is built from arrays, with indices in place of pointers.
- The node at index `k` has its value in `value[k]` and the index of the next node in `next[k]`.
