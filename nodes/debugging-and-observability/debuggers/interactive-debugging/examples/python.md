```python norun reason=interactive id=stop
def double(x):
    breakpoint()  # pdb stops here and waits for commands
    return x * 2


print(double(21))
```

The program above stops at `breakpoint()` and waits for a person to type commands. A page cannot record that session. The next program gives the same commands to pdb from a string. The subclass of `pdb.Pdb` echoes each command, so the output reads like a session at a terminal.

```python run id=session
import io
import os
import pdb
import sys


class Script(pdb.Pdb):
    """A Pdb that reads its commands from a string and echoes each one."""

    def __init__(self, commands):
        super().__init__(stdin=io.StringIO(commands), stdout=ShortPaths(), nosigint=True, readrc=False)
        self.use_rawinput = False

    def precmd(self, line):
        if not line.startswith("_"):  # skip the internal commands of pdb
            self.stdout.write(line + "\n")
        return super().precmd(line)


class ShortPaths(io.TextIOBase):
    """Writes to standard output and removes the directory from file paths."""

    def write(self, text):
        return sys.stdout.write(text.replace(os.path.dirname(os.path.abspath(__file__)) + os.sep, ""))


def total(prices):
    result = 0
    for price in prices:
        result += price
    return result


def checkout(cart):
    prices = [item["price"] for item in cart]
    return total(prices)


commands = """\
break total
continue
p prices
next
display result
next
next
break 30, price > 100
continue
p price, result
up
p cart[1]["name"]
continue
"""
cart = [{"name": "apple", "price": 30}, {"name": "bread", "price": 250}]
print("total:", Script(commands).runcall(checkout, cart))
```

```output for=session
> main.py(35)checkout()
-> prices = [item["price"] for item in cart]
(Pdb) break total
Breakpoint 1 at main.py:28
(Pdb) continue
> main.py(28)total()
-> result = 0
(Pdb) p prices
[30, 250]
(Pdb) next
> main.py(29)total()
-> for price in prices:
(Pdb) display result
display result: 0
(Pdb) next
> main.py(30)total()
-> result += price
(Pdb) next
> main.py(29)total()
-> for price in prices:
display result: 30  [old: 0]
(Pdb) break 30, price > 100
Breakpoint 2 at main.py:30
(Pdb) continue
> main.py(30)total()
-> result += price
(Pdb) p price, result
(250, 30)
(Pdb) up
> main.py(36)checkout()
-> return total(prices)
(Pdb) p cart[1]["name"]
'bread'
(Pdb) continue
total: 280
```

`break total` stops at the first line of `total`. `next` runs one line and stays in the function. `display result` prints `result` again each time the value changes. `break 30, price > 100` stops on line 30 only when the condition is true. `up` moves to the frame of the caller, where `cart` is visible.

Commands of pdb have short names. A variable with the same name as a command does not print. pdb runs the command.

```python wrong id=pdb-names
import io
import os
import pdb
import sys


class Script(pdb.Pdb):
    """A Pdb that reads its commands from a string and echoes each one."""

    def __init__(self, commands):
        super().__init__(stdin=io.StringIO(commands), stdout=ShortPaths(), nosigint=True, readrc=False)
        self.use_rawinput = False

    def precmd(self, line):
        if not line.startswith("_"):  # skip the internal commands of pdb
            self.stdout.write(line + "\n")
        return super().precmd(line)


class ShortPaths(io.TextIOBase):
    """Writes to standard output and removes the directory from file paths."""

    def write(self, text):
        return sys.stdout.write(text.replace(os.path.dirname(os.path.abspath(__file__)) + os.sep, ""))


def average(values):
    n = len(values)
    s = sum(values)
    return s / n


# The person wants to see the variables n and s. But n is the command next,
# and s is the command step: pdb runs them and prints no value.
commands = """\
next
next
n
s
p n, s
continue
"""
print("average:", Script(commands).runcall(average, [3, 4, 8]))
```

```output for=pdb-names
> main.py(28)average()
-> n = len(values)
(Pdb) next
> main.py(29)average()
-> s = sum(values)
(Pdb) next
> main.py(30)average()
-> return s / n
(Pdb) n
--Return--
> main.py(30)average()->5.0
-> return s / n
(Pdb) s
average: 5.0
```

`n` ran one more line and `s` stepped out of the function, so the program ended before `p n, s`. The fix is `p n`, or `!n`, which runs the text as a Python statement.

`breakpoint()` calls `sys.breakpointhook`. A program can put its own function in that hook. The next hook prints the stack and the local variables and then returns, so the program continues.

```python run id=hook
import sys
import traceback


def show_frame(*args, **kwargs):
    """A breakpoint hook that prints the stack and the locals, then returns."""
    caller = sys._getframe(1)
    for frame in traceback.extract_stack(caller):
        print(f"  {frame.name} line {frame.lineno}: {frame.line}")
    print("  locals:", caller.f_locals)


def total(prices):
    result = 0
    for price in prices:
        result += price
        if price > 100:
            breakpoint()  # calls sys.breakpointhook
    return result


sys.breakpointhook = show_frame
print("total:", total([30, 250]))
```

```output for=hook
  <module> line 23: print("total:", total([30, 250]))
  total line 18: breakpoint()  # calls sys.breakpointhook
  locals: {'prices': [30, 250], 'result': 280, 'price': 250}
total: 280
```

`faulthandler` prints the stack of Python frames with code in C. It works after a crash, when Python code can no longer run. `dump_traceback` prints the same stack on request.

```python run id=faulthandler
import faulthandler
import os

# faulthandler writes to a file descriptor without Python's I/O, so that it
# still works after a crash or in a deadlock. Here it writes to a pipe, and the
# program removes the directory from the file names before it prints them.


def dump():
    read_end, write_end = os.pipe()
    with os.fdopen(write_end, "w") as out:
        faulthandler.dump_traceback(file=out, all_threads=False)
    with os.fdopen(read_end) as pipe:
        text = pipe.read()
    print(text.replace(os.path.dirname(os.path.abspath(__file__)) + os.sep, ""), end="")


def load(path):
    dump()


def main():
    load("orders.csv")


main()
```

```output for=faulthandler
Stack (most recent call first):
  File "main.py", line 12 in dump
  File "main.py", line 19 in load
  File "main.py", line 23 in main
  File "main.py", line 26 in <module>
```

- Verdict: stdlib. pdb is a module of the standard library. `python -m pdb script.py` runs a script under it, and `breakpoint()` starts it from the code.
- `PYTHONBREAKPOINT=0` turns every `breakpoint()` call into a call that does nothing. Another dotted name selects another debugger.
- `python -X faulthandler` or `PYTHONFAULTHANDLER` turns on `faulthandler` for a crash: the program prints the stack on `SIGSEGV`, `SIGFPE`, `SIGABRT`, `SIGBUS` and `SIGILL`.
- `traceback.extract_stack` gives the same frames as Python objects. `traceback.print_stack` prints them.
