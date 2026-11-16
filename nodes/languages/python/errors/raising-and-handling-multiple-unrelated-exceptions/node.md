---
tag: BZ0G95
kind: feature
owner: python
slug: raising-and-handling-multiple-unrelated-exceptions
name: Raising and Handling Multiple Unrelated Exceptions
aliases: [ExceptionGroup, BaseExceptionGroup, except star, exception groups, PEP 654, split and subgroup]
area: languages/python/errors
level: advanced
summary: >-
  An ExceptionGroup raises several exceptions together. The except* clause handles the members of a group by type: each clause takes the members that match and the rest go on.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#except-star", locator: "8.4.2. except* clause"}
see_also: [python/handling-exceptions, python/the-exception-hierarchy, python/enriching-exceptions-with-notes]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#raising-and-handling-multiple-unrelated-exceptions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.9. Raising and Handling Multiple Unrelated Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#except-star
    title: The Python Language Reference, 8. Compound statements
    locator: 8.4.2. except* clause
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#exception-groups
    title: Built-in Exceptions
    locator: Exception groups
    accessed: 2026-11-16
  - url: https://pkg.go.dev/errors#Join
    title: errors package
    locator: func Join
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
def check(record):
    errors = []
    if not record.get('name'):
        errors.append(ValueError('name is empty'))
    if not isinstance(record.get('age'), int):
        errors.append(TypeError('age must be an int'))
    if errors:
        raise ExceptionGroup('invalid record', errors)

try:
    check({'name': '', 'age': '40'})
except* ValueError as group:
    print('values:', group.exceptions)
except* TypeError as group:
    print('types:', group.exceptions)
```

```output for=basics
values: (ValueError('name is empty'),)
types: (TypeError('age must be an int'),)
```

The function reports both problems at once. Each except* clause runs for the members of its type.

## Semantics

**r1.** "The builtin ExceptionGroup wraps a list of exception instances so that they can be raised together. It is an exception itself, so it can be caught like any other exception."[^1] "the exceptions nested in an exception group must be instances, not types."[^1]

**r2.** "BaseExceptionGroup extends BaseException and it can wrap any exception, while ExceptionGroup extends Exception and it can only wrap subclasses of Exception. This design is so that except Exception catches an ExceptionGroup but not BaseExceptionGroup."[^3]

**r3.** "When an exception group is raised in the try block, each except* clause splits (see split()) it into the subgroups of matching and non-matching exceptions. If the matching subgroup is not empty, it becomes the handled exception (the value returned from sys.exception()) and assigned to the target of the except* clause (if there is one)."[^2] "Then, the body of the except* clause executes. If the non-matching subgroup is not empty, it is processed by the next except* in the same manner."[^2]

**r4.** "After all except* clauses execute, the group of unhandled exceptions is merged with any exceptions that were raised or re-raised from within except* clauses. This merged exception group propagates on".[^2] Example: [`rest`](#rest).

**r5.** "If the exception raised from the try block is not an exception group and its type matches one of the except* clauses, it is caught and wrapped by an exception group with an empty message string."[^2] Example: [`wrap`](#wrap).

**r6.** "A try statement can have either except or except* clauses, but not both."[^2] "break, continue and return cannot appear in an except* clause."[^2] Example: [`mixed`](#mixed).

**r7.** `subgroup(condition)` "Returns an exception group that contains only the exceptions from the current group that match condition, or None if the result is empty."[^3] `split` is "Like subgroup(), but returns the pair (match, rest)".[^3] Example: [`split`](#split).

## Examples

```python run id=rest
try:
    try:
        raise ExceptionGroup('eg', [ValueError(1), TypeError(2), OSError(3), OSError(4)])
    except* TypeError as e:
        print('caught', e.exceptions)
    except* OSError as e:
        print('caught', e.exceptions)
except ExceptionGroup as rest:
    print('left over:', repr(rest))
```

```output for=rest
caught (TypeError(2),)
caught (OSError(3), OSError(4))
left over: ExceptionGroup('eg', [ValueError(1)])
```

No clause matches the ValueError, so a group with it goes on to the outer handler (rule r4).

```python run id=wrap
try:
    raise BlockingIOError
except* BlockingIOError as e:
    print(repr(e))
```

```output for=wrap
ExceptionGroup('', (BlockingIOError(),))
```

A plain exception is put in a group, so the target of except* is always a group (rule r5).

```python run id=split
group = ExceptionGroup('batch', [ValueError('a'), KeyError('b'), ValueError('c')])
match, rest = group.split(ValueError)
print(repr(match))
print(repr(rest))
print(group.subgroup(OSError))
```

```output for=split
ExceptionGroup('batch', [ValueError('a'), ValueError('c')])
ExceptionGroup('batch', [KeyError('b')])
None
```

`split` gives the matching and the other members. `subgroup` gives None when nothing matches (rule r7).

```python run id=collect
tasks = {'a': '1', 'b': 'x', 'c': '3', 'd': 'y'}
results, errors = {}, []
for name, text in tasks.items():
    try:
        results[name] = int(text)
    except ValueError as e:
        errors.append(e)
print(results)
try:
    if errors:
        raise ExceptionGroup('some tasks failed', errors)
except ExceptionGroup as eg:
    print(eg)
    print(len(eg.exceptions))
```

```output for=collect
{'a': 1, 'c': 3}
some tasks failed (2 sub-exceptions)
2
```

This is the pattern from the tutorial: collect the exceptions that were caught, then raise them together (rule r1).

## Pitfalls

A plain except clause does not look inside a group:

```python fails id=plain expect=exception:ExceptionGroup
try:
    raise ExceptionGroup('eg', [OSError('disk full')])
except OSError:
    print('handled the OSError')
```

An ExceptionGroup is not an OSError, so the clause does not match, and the group is not handled. Use `except* OSError`, or catch the group and look at its members (rule r3).

except and except* cannot be in the same try statement:

```python fails id=mixed expect=exception:SyntaxError
try:
    pass
except* ValueError:
    pass
except TypeError:
    pass
```

Python rejects this code before it runs (rule r6). Nest one try statement in another if you need both kinds of clause.

## Comparison

Go joins errors into one value. "Join returns an error that wraps the given errors."[^4] "The errors may be inspected with Is and As."[^4] An ExceptionGroup also holds several exceptions, and except* or `split` takes them out by type (rules r3 and r7).
