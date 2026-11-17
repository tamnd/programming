---
tag: ZJ87W9
kind: feature
owner: python
slug: broad-except
name: Broad except clauses
aliases: [bare except, except Exception, catch-all exception handler, try-except-pass, swallowed exception, E722, BLE001, S110, W0702, W0718, broad-exception-caught]
area: languages/python/pitfalls
level: core
summary: >-
  A bare except or except Exception catches more than the error you expect, so it hides typos and real bugs. Catch the specific exception, and log and re-raise in a catch-all handler.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#except-clause", locator: "8.4.1 except clause"}
proposals: []
see_also: [python/handling-exceptions, python/the-exception-hierarchy, python/raising-exceptions, python/exception-chaining, python/logging, python/clean-up-actions-with-finally, python/eafp-and-lbyl]
sources:
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: PEP 8, Style Guide for Python Code
    locator: Programming Recommendations
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/exceptions.html#exception-hierarchy
    title: Built-in Exceptions, Python 3.15 documentation
    locator: Base classes; SystemExit; KeyboardInterrupt
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/try-except-pass/
    title: try-except-pass (S110), Ruff documentation
    locator: Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/logging.html#logging.exception
    title: logging, Logging facility for Python, Python 3.15 documentation
    locator: logging.exception()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/bare-except/
    title: bare-except (E722), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/blind-except/
    title: blind-except (BLE001), Ruff documentation
    locator: What it does
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/bare-except.html
    title: bare-except / W0702, Pylint documentation
    locator: Message
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/broad-exception-caught.html
    title: broad-exception-caught / W0718, Pylint documentation
    locator: Message
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=typo-hidden
def parse_port(text):
    try:
        return int(txt)
    except:
        return 8080


print(parse_port("9000"))
```

```output for=typo-hidden
8080
```

The function has a typo, `txt`, so every call raises `NameError`. The bare `except:` catches that error too, and the function returns the default for every input.

## Semantics

**r1.** PEP 8 says: "When catching exceptions, mention specific exceptions whenever possible instead of using a bare except: clause".[^1] Example: [`specific`](#specific).

**r2.** A bare `except:` catches everything. "A bare except: clause will catch SystemExit and KeyboardInterrupt exceptions, making it harder to interrupt a program with Control-C, and can disguise other problems. If you want to catch all exceptions that signal program errors, use except Exception: (bare except is equivalent to except BaseException:)."[^1]

**r3.** The hierarchy keeps the exit signals out of `Exception`. `Exception` is the base of "All built-in, non-system-exiting exceptions".[^2] `KeyboardInterrupt` "inherits from BaseException so as to not be accidentally caught by code that catches Exception and thus prevent the interpreter from exiting."[^2] `SystemExit` "inherits from BaseException instead of Exception so that it is not accidentally caught by code that catches Exception."[^2] Example: [`exit-swallowed`](#exit-swallowed).

**r4.** `except Exception:` is still broad. It catches `NameError`, `AttributeError` and `TypeError`, which are usually bugs in the code, not conditions to handle. Example: [`typo-hidden`](#typo-hidden).

**r5.** An empty handler is the worst case. Ruff explains: "The try-except-pass pattern suppresses all exceptions. Suppressing exceptions may hide errors that could otherwise reveal unexpected behavior, security vulnerabilities, or malicious activity. Instead, consider logging the exception."[^3]

**r6.** A catch-all handler has a place at the top of a program or a worker loop, where one failure must not stop the others. There, record the error with its traceback. `logging.exception` "Logs a message with level ERROR on the root logger" and "Exception info is added to the logging message. This function should only be called from an exception handler."[^4] Then re-raise with a bare `raise` if the caller must know. Example: [`log-and-raise`](#log-and-raise).

**r7.** The linters:

- Ruff E722 "Checks for bare except catches in try-except statements."[^5] Its reason: "A bare except catches BaseException which includes KeyboardInterrupt, SystemExit, Exception, and others."[^5]
- Ruff BLE001 "Checks for except clauses that catch all exceptions. This includes except BaseException and except Exception."[^6]
- Ruff S110 reports `try`, `except` and `pass` (rule r5).[^3]
- Pylint W0702 `bare-except` has the message "No exception type(s) specified".[^7] Pylint W0718 `broad-exception-caught` has the message "Catching too general exception %s".[^8]

## Examples

```python run id=specific
def parse_port(text):
    try:
        return int(text)
    except ValueError:
        return 8080


print(parse_port("9000"))
print(parse_port("http"))
```

```output for=specific
9000
8080
```

The handler names `ValueError`, the error that `int` raises for bad text (rule r1). A typo in the body now raises `NameError` and shows the bug.

```python run id=log-and-raise
import logging

records = []


class Keep(logging.Handler):
    def emit(self, record):
        records.append(record)


logging.getLogger().addHandler(Keep())


def run_job(job):
    return 10 // job


for job in [5, 0]:
    try:
        print(run_job(job))
    except Exception:
        logging.exception("job %r failed", job)

for r in records:
    print(r.levelname, r.getMessage(), r.exc_info[0].__name__)
```

```output for=log-and-raise
2
ERROR job 0 failed ZeroDivisionError
```

The loop goes on after a failed job, and the log record holds the exception and its traceback, so the error is not lost (rule r6).

## Versions

The rules are the same in all Python 3 versions.

## Pitfalls

```python wrong id=exit-swallowed
import sys


def main():
    try:
        sys.exit(2)
    except:
        print("error ignored")
    return "still running"


print(main())
```

```output for=exit-swallowed
error ignored
still running
```

The bare `except:` caught `SystemExit`, so the program did not stop (rule r3). With `except Exception:`, the exit goes through.

```python fails id=reraise expect=exception:KeyError
config = {"host": "localhost"}

try:
    port = config["port"]
except KeyError:
    print("no port in the config")
    raise
```

A bare `raise` in a handler raises the same exception again, with its traceback. The handler adds a message, and the caller still sees the error (rule r6).

## Comparison

In Python, a handler that is too broad catches errors that are bugs in the code, such as `NameError`, because those are exceptions too. So the size of the handler matters as much as its class: keep the `try` block small, so it holds only the call that can fail. For the hierarchy, see [[python/the-exception-hierarchy]]. For the general topic, see [[exception-handling]].
