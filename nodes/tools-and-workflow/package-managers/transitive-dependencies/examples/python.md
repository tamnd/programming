```python run id=dependency-tree
# Print a dependency tree, then find the packages that are shared (a diamond) and the packages with two versions.
DEPS = {
    "app": ["cli 1.0", "web 2.0"],
    "cli 1.0": ["json 0.9", "log 1.2"],
    "web 2.0": ["http 1.4", "json 1.1", "log 1.2"],
    "http 1.4": ["log 1.2"],
    "json 0.9": [],
    "json 1.1": [],
    "log 1.2": [],
}
seen = set()


def show(package, prefix=""):
    children = DEPS[package]
    for i, child in enumerate(children):
        last = i == len(children) - 1
        mark = " (*)" if child in seen else ""
        print(prefix + ("`-- " if last else "+-- ") + child + mark)
        if not mark:
            seen.add(child)
            show(child, prefix + ("    " if last else "|   "))


print("app")
show("app")
transitive = sorted(seen - set(DEPS["app"]))
print("direct:", ", ".join(DEPS["app"]))
print("transitive:", ", ".join(transitive))

users, versions = {}, {}
for package, children in DEPS.items():
    for child in children:
        users.setdefault(child, []).append(package)
for package in sorted(seen):
    name, version = package.split()
    versions.setdefault(name, []).append(version)
print("needed by more than one package:")
for package in sorted(users):
    if len(users[package]) > 1:
        print(f"  {package}: {', '.join(sorted(users[package]))}")
print("more than one version:")
for name in sorted(versions):
    if len(versions[name]) > 1:
        print(f"  {name}: {', '.join(versions[name])}")
```

```output
app
+-- cli 1.0
|   +-- json 0.9
|   `-- log 1.2
`-- web 2.0
    +-- http 1.4
    |   `-- log 1.2 (*)
    +-- json 1.1
    `-- log 1.2 (*)
direct: cli 1.0, web 2.0
transitive: http 1.4, json 0.9, json 1.1, log 1.2
needed by more than one package:
  log 1.2: cli 1.0, http 1.4, web 2.0
more than one version:
  json: 0.9, 1.1
```

- Verdict: idiom. A dict maps each package to a list of the packages that it needs, and a set records the packages that `show` has printed.
- The set difference `seen - set(DEPS["app"])` gives the transitive dependencies.
- `setdefault` builds the maps from a package to its users and from a name to its versions in one pass each.
