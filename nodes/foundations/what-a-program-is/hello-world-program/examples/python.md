```python run id=main-guard-and-exit-status
import sys


def main(argv):
    name = argv[1] if len(argv) > 1 else "world"
    print(f"Hello, {name}!")
    print(f"{name} has {len(name)} letters")
    print("arguments:", argv[1:])
    return 0  # the exit status: 0 means success


if __name__ == "__main__":  # true when run as a program, false when imported
    sys.exit(main(sys.argv))
```

```output for=main-guard-and-exit-status
Hello, world!
world has 5 letters
arguments: []
```

```python wrong id=exit-skips-flush
import os

print("start", flush=True)
print("Hello, world!")  # waits in the output buffer when stdout is a pipe
os._exit(0)  # ends the process at once, so nothing flushes the buffer
```

```output for=exit-skips-flush
start
```

- Verdict: native. A Python program is a file of statements that run from top to bottom, so `print("Hello, world!")` alone is a complete program. `print` writes the text and a newline to standard output. The test `if __name__ == "__main__"` runs the main code only when the file runs as a program, not when another module imports it, and `sys.exit` sets the exit status.
- The wrong block prints a line and then ends the process with `os._exit`. When standard output is a pipe or a file, Python buffers it, and `os._exit` ends the process without the flush that a normal exit does, so the line is lost. It appears on a terminal, where output is flushed at each newline. Return from the program normally, or flush before a hard exit.
