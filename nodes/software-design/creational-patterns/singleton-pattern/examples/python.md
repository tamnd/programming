```python norun reason=fragment file=settings.py group=modules
print("creating the settings")

level = "info"
```

```python run file=main.py group=modules id=modules
import threading

import settings

seen = []


def worker():
    # A second import finds the module in sys.modules and does not run it again.
    import settings as again

    print("worker sets debug")
    again.level = "debug"
    seen.append(again)


print(f"main: {settings.level}")
thread = threading.Thread(target=worker)
thread.start()
thread.join()
print(f"main: {settings.level}")
print(f"same instance: {seen[0] is settings}")
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: True
```

- Verdict: native. A module is the singleton. Python runs a module once and stores it in `sys.modules`. Every later import returns the same module object.
- The import lock makes the first import safe when two threads import the module at the same time.
- The module runs when it is first imported. To create an expensive object on first use, the module can hold a function that creates and stores it, protected by a lock.
- Code that runs `settings.py` as a script gets a second copy, named `__main__`, separate from the copy that `import settings` makes.
