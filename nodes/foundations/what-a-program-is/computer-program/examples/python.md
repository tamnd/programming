```python run id=gcd-with-exit-status
import sys


def gcd(a, b):
    while b != 0:  # the state is the pair (a, b)
        a, b = b, a % b
        print(a, b)
    return a


def main():
    print(1071, 462)
    print("gcd", gcd(1071, 462))
    return 0  # the exit status


if __name__ == "__main__":
    sys.exit(main())
```

```output for=gcd-with-exit-status
1071 462
462 147
147 21
21 0
gcd 21
```

- Verdict: native. A Python program is a script: the interpreter runs the statements of the file from top to bottom, so a file with one `print` call is a complete program. The test `if __name__ == "__main__"` runs `main` only when the file runs as a program and not when another module imports it, and `sys.exit` passes its value to the operating system as the exit status.
