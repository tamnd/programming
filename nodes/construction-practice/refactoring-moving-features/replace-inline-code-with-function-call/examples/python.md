```python run id=say-it-by-name
states = ["NY", "MA", "CA"]
tags = ["sale", "tea", "gift"]

# Before: a loop that sets a flag, and string building by hand.
applies_before = False
for s in states:
    if s == "MA":
        applies_before = True
label_before = ""
for i, t in enumerate(tags):
    label_before += ("" if i == 0 else ", ") + t

# After: calls to built-ins that already do it.
applies = any(s == "MA" for s in states)  # or: "MA" in states
label = ", ".join(tags)

assert (applies, label) == (applies_before, label_before)
assert ", ".join([]) == "" and not any([])  # the edge cases agree too
print("replace inline code with function call: tests pass")
```

```output
replace inline code with function call: tests pass
```
