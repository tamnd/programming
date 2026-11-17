```python run id=trace
import inspect


def where():
    names = [f.function for f in inspect.stack()[1:]]  # innermost first
    return " <- ".join(n for n in names if n != "<module>")


def c():
    print("      stack:", where())


def b():
    print("    call c")
    c()
    print("    back in b")


def a():
    print("  call b")
    b()
    print("  back in a")


def main():
    print("call a")
    a()
    print("back in main")


main()
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. `inspect.stack()` returns one record for each frame, the caller first and the outermost call last. Each frame is an object, with `f_back` for the frame of its caller.
