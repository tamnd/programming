```python run id=backtracking-resolver
# A backtracking dependency resolver: try the newest version first, and go back on a conflict.
# A requirement is (package, lowest allowed version, first version not allowed).
VERSIONS = {
    "web": {"2.0": [("log", "2.0", "3.0")],
            "1.1": [("log", "1.1", "2.0")],
            "1.0": [("log", "1.0", "2.0")]},
    "log": {"2.0": [], "1.2": [], "1.1": [], "1.0": []},
}


def key(version):
    return tuple(int(part) for part in version.split("."))


def fits(version, lo, hi):
    return key(lo) <= key(version) < key(hi)


def solve(todo, chosen):
    if not todo:
        return chosen
    (name, lo, hi, who), rest = todo[0], todo[1:]
    if name in chosen:
        if fits(chosen[name], lo, hi):
            return solve(rest, chosen)
        print(f"  conflict: {who} needs {name} >={lo},<{hi}, but {name} {chosen[name]} is chosen")
        return None
    for version in sorted(VERSIONS[name], key=key, reverse=True):
        if fits(version, lo, hi):
            print(f"try {name} {version}")
            deps = [(dep, dlo, dhi, f"{name} {version}") for dep, dlo, dhi in VERSIONS[name][version]]
            result = solve(rest + deps, {**chosen, name: version})
            if result is not None:
                return result
    return None


for root in ([("web", "1.0", "3.0", "app"), ("log", "1.0", "2.0", "app")],
             [("web", "2.0", "3.0", "app"), ("log", "1.0", "2.0", "app")]):
    print("app needs", ", ".join(f"{n} >={lo},<{hi}" for n, lo, hi, _ in root))
    result = solve(root, {})
    if result is None:
        print("result: no solution")
    else:
        print("result:", ", ".join(f"{n} {v}" for n, v in sorted(result.items())))
```

```output
app needs web >=1.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
try web 1.1
try log 1.2
result: log 1.2, web 1.1
app needs web >=2.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
result: no solution
```

- Verdict: idiom. A dict maps each package to its versions, and each version to its requirements.
- `solve` is recursive. `{**chosen, name: version}` makes a new dict for each choice, so a failed branch leaves the caller's dict as it was.
- `sorted(..., key=key, reverse=True)` tries the newest version first. The key turns "1.10" into a tuple of numbers, so that it sorts after "1.9".
