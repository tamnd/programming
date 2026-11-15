```python run id=tiny-build
# A tiny build system: rules, dependencies first, and a skip when a target is up to date.
RULES = {
    "app": (["main.o", "util.o"], "link"),
    "main.o": (["main.c", "util.h"], "compile"),
    "util.o": (["util.c", "util.h"], "compile"),
}
store = {}  # key -> (value, time)
clock = 0


def write(key, value):
    global clock
    clock += 1
    store[key] = (value, clock)


def build(target, done, ran):
    if target in done:
        return
    done.add(target)
    if target not in RULES:
        if target not in store:
            raise KeyError("no rule to make " + target)
        return
    deps, action = RULES[target]
    for dep in deps:
        build(dep, done, ran)
    old = store.get(target)
    if old is None or any(store[dep][1] > old[1] for dep in deps):
        write(target, action + "(" + ", ".join(deps) + ")")
        ran.append(action + " " + target)


def run(label):
    ran = []
    build("app", set(), ran)
    print(label + ": " + (", ".join(ran) if ran else "nothing to do"))


for name in ["main.c", "util.c", "util.h"]:
    write(name, "source of " + name)
run("build 1")
run("build 2")
print("edit util.h")
write("util.h", "edited")
run("build 3")
print("edit main.c")
write("main.c", "edited")
run("build 4")
print("delete util.c")
del store["util.c"]
try:
    run("build 5")
except KeyError as e:
    print("build 5: error: " + e.args[0])
```

```output
build 1: compile main.o, compile util.o, link app
build 2: nothing to do
edit util.h
build 3: compile main.o, compile util.o, link app
edit main.c
build 4: compile main.o, link app
delete util.c
build 5: error: no rule to make util.c
```

- Verdict: idiom. A dict maps each target to its dependencies and its action, and a second dict is the store, with a value and a time for each key.
- `build` is recursive, so the dependencies of a target are built before the target. The set `done` makes each target run at most once in one build.
- The check `store[dep][1] > old[1]` is the Make rule: a target is out of date when a dependency is newer.
