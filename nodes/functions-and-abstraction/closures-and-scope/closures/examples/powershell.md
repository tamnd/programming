```powershell run id=counter
function New-Counter {
    $count = 0
    {
        $script:count++
        $script:count
    }.GetNewClosure()
}

$first = New-Counter
$second = New-Counter
"$(& $first) $(& $first) $(& $first)"
& $second
```

```output for=counter
1 2 3
1
```

- Verdict: native, in a limited form. A script block is a function value, but by default it does not keep the variables of the scope where it was made.
- `GetNewClosure()` makes a fresh script block bound to a fresh module. It copies the local variables of the caller into that module.
- The script block sees the copy, not the original variable. Inside the script block, `$script:count` names the copy in the module scope, so the change persists from one call to the next.
- Each call of `New-Counter` makes a fresh module, so the two counters do not share state.
