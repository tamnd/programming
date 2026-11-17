```bash run id=stack-adt
# The operations. The representation is the array named in $1.
stack_push() { local -n s=$1; s+=("$2"); }
stack_pop() { local -n s=$1; REPLY=${s[-1]}; unset 's[-1]'; }
stack_is_empty() { local -n s=$1; (( ${#s[@]} == 0 )); }

nums=()
for v in 1 2 3; do stack_push nums "$v"; done
out=()
while ! stack_is_empty nums; do
  stack_pop nums
  out+=("$REPLY")
done
echo "ArrayStack: ${out[*]}"
```

```output for=stack-adt
ArrayStack: 3 2 1
```

- Verdict: idiom. A set of functions works on one array, and the code that uses the stack calls only the functions. `local -n` makes `s` a name for the array that the caller passes.
- Nothing hides the array. Any code can change `nums` directly, so the barrier is only a habit.
- `stack_pop` returns its value in `REPLY`, because `$(stack_pop nums)` would run in a subshell and its change to the array would be lost.
