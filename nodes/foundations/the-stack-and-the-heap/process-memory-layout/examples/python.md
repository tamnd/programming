```python run id=regions
def regions():
    """Yield (start, perms, path) for each region in /proc/self/maps."""
    with open("/proc/self/maps") as f:
        for line in f:
            fields = line.split(maxsplit=5)
            start = int(fields[0].split("-")[0], 16)
            path = fields[5].strip() if len(fields) > 5 else ""
            yield start, fields[1], path


found = {}
for start, perms, path in regions():
    if path in ("[heap]", "[stack]", "[vdso]") and path not in found:
        found[path] = start
        print(path, perms)

print("the heap is below the stack:", str(found["[heap]"] < found["[stack]"]).lower())
```

```output for=regions
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

- Verdict: native. Python does not give the address of a region, but the program runs in a process with the usual layout, and it can read `/proc/self/maps` as a text file.
