```python run id=tiny-package-manager
# A tiny package manager: install with dependencies, refuse to break them, and remove unused ones.
REGISTRY = {
    "app": ["web", "log"],
    "web": ["http", "log"],
    "http": [],
    "log": [],
    "cli": ["log"],
}
installed = {}  # name -> True if the user asked for it, False if it came as a dependency


def install(name, asked=True):
    if name in installed:
        installed[name] = installed[name] or asked
        return
    for dep in REGISTRY[name]:
        install(dep, asked=False)
    installed[name] = asked
    print("  install", name, "(asked)" if asked else "(dependency)")


def needed_by(name):
    return sorted(p for p in installed if name in REGISTRY[p])


def remove(name):
    users = needed_by(name)
    if users:
        print("  error:", name, "is needed by", ", ".join(users))
        return
    del installed[name]
    print("  remove", name)


def autoremove():
    changed = True
    while changed:
        changed = False
        for name in sorted(installed):
            if not installed[name] and not needed_by(name):
                remove(name)
                changed = True


for command, arg in [("install", "app"), ("install", "cli"), ("remove", "log"),
                     ("remove", "app"), ("autoremove", None), ("list", None)]:
    print("$", command if arg is None else command + " " + arg)
    if command == "install":
        install(arg)
    elif command == "remove":
        remove(arg)
    elif command == "autoremove":
        autoremove()
    else:
        for name in sorted(installed):
            print(" ", name, "(asked)" if installed[name] else "(dependency)")
```

```output
$ install app
  install http (dependency)
  install log (dependency)
  install web (dependency)
  install app (asked)
$ install cli
  install cli (asked)
$ remove log
  error: log is needed by app, cli, web
$ remove app
  remove app
$ autoremove
  remove web
  remove http
$ list
  cli (asked)
  log (dependency)
```

- Verdict: idiom. A dict maps each package to its dependencies, and a second dict maps each installed package to its flag.
- `install` is recursive, so the dependencies are installed before the package. A package that is already installed returns at once, so `log` is installed one time.
- `autoremove` loops until a pass removes nothing, because removing `web` makes `http` unused.
