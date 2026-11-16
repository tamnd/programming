---
tag: XYTX6Y
kind: feature
owner: python
slug: bytecode-and-dis
name: Bytecode and dis
aliases: [dis, dis.dis, dis.get_instructions, dis.Bytecode, dis.Instruction, CPython bytecode, opcode, co_code, disassembler, python -m dis]
area: languages/python/metaprogramming
level: advanced
summary: >-
  CPython compiles each function to bytecode, a list of instructions for its stack machine. The dis module shows the instructions. The bytecode is an implementation detail and changes between releases.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/dis.html", locator: "dis — Disassembler for Python bytecode"}
proposals: []
see_also: [bytecode, python/code-and-frame-objects, python/the-ast-module, python/eval-and-exec, python/tracing-and-monitoring]
sources:
  - url: https://docs.python.org/3.15/library/dis.html
    title: dis — Disassembler for Python bytecode, Python 3.15 documentation
    locator: Bytecode analysis; Analysis functions; Python Bytecode Instructions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#code-objects
    title: The Python Language Reference, Python 3.15
    locator: 3.2.13.1 Code objects
    accessed: 2026-11-16
---

## Synopsis

```python run id=instructions
import dis

def add_tax(price):
    return price * 1.2

for ins in dis.get_instructions(add_tax):
    if ins.opname in ("LOAD_CONST", "BINARY_OP"):
        print(ins.opname, ins.argrepr)
```

```output for=instructions
LOAD_CONST 1.2
BINARY_OP *
```

## Semantics

**r1.** "The dis module supports the analysis of CPython bytecode by disassembling it."[^1] A code object holds the bytecode in `co_code`, "A string representing the sequence of bytecode instructions in the function".[^2]

**r2.** "Bytecode is an implementation detail of the CPython interpreter. No guarantees are made that bytecode will not be added, removed, or changed between versions of Python."[^1] Example: [`fast-name`](#fast-name).

**r3.** `dis.get_instructions` returns "an iterator over the instructions in the supplied function, method, source code string or code object."[^1] Each item is an `Instruction` with an `opname`, the "human readable name for operation", an `argval`, the "resolved arg value (if any)", and an `argrepr`.[^1] Example: [`instructions`](#instructions).

**r4.** `dis.Bytecode` wraps a function or code object, and "iterating over a Bytecode instance yields the bytecode operations as Instruction instances."[^1] Example: [`calls`](#calls).

**r5.** Each instruction has `positions`, a "dis.Positions object holding the start and end locations that are covered by this instruction."[^1] Example: [`calls`](#calls).

**r6.** `dis.stack_effect` computes "the stack effect of opcode with argument oparg."[^1] Example: [`stack-effect`](#stack-effect).

**r7.** The interpreter "adapts the bytecode to specialize it for different runtime conditions. The adaptive bytecode can be shown by passing adaptive=True."[^1]

## Examples

```python run id=listing
import dis

def myfunc(alist):
    return len(alist)

dis.dis(myfunc)
```

```output for=listing
  3           RESUME                   0

  4           LOAD_GLOBAL              1 (len + NULL)
              LOAD_FAST_BORROW         0 (alist)
              CALL                     1
              RETURN_VALUE
```

The numbers on the left are source lines. Each line after them is one instruction and its argument. `RESUME` is "A no-op" that "Performs internal tracing, debugging and optimization checks."[^1] This is the output of CPython 3.14 (rule r2).

```python run id=folding
import dis

code = compile("seconds = 60 * 60 * 24", "<s>", "exec")
print([(i.opname, i.argval) for i in dis.get_instructions(code) if i.opname.startswith("LOAD")])
```

```output for=folding
[('LOAD_CONST', 86400), ('LOAD_CONST', None)]
```

The compiler calculated `60 * 60 * 24` before the code ran, so the bytecode loads one constant. The second constant is the `None` that the module code returns.

```python run id=calls
import dis

def report(items):
    print(len(items))
    return sorted(items)

calls = [i for i in dis.Bytecode(report) if i.opname == "CALL"]
print(len(calls), [i.positions.lineno - report.__code__.co_firstlineno for i in calls])
```

```output for=calls
3 [1, 1, 2]
```

There are three calls: `len` and `print` on the first line of the body, and `sorted` on the second (rules r4 and r5).

```python run id=stack-effect
import dis

print(dis.stack_effect(dis.opmap["POP_TOP"]))
print(dis.stack_effect(dis.opmap["BUILD_LIST"], 3))
```

```output for=stack-effect
-1
-2
```

`POP_TOP` removes one value. `BUILD_LIST` with argument 3 removes three values and pushes one list (rule r6).

## Versions

> [!since v="3.11"]
> "the interpreter now adapts the bytecode to specialize it for different runtime conditions."[^1] Instructions can also have inline `CACHE` entries.[^1]

> [!since v="3.14"]
> `LOAD_FAST_BORROW` and `LOAD_SMALL_INT` are new instructions.[^1]

## Pitfalls

```python wrong id=fast-name
import dis

def f(x):
    return x

names = {i.opname for i in dis.get_instructions(f)}
print("LOAD_FAST" in names)
print(sorted(n for n in names if n.startswith("LOAD_FAST")))
```

```output for=fast-name
False
['LOAD_FAST_BORROW']
```

The test looks for one instruction name, but CPython 3.14 loads the local with a different instruction (rule r2). Code that reads bytecode must follow each release.

```python fails id=builtin expect=exception:TypeError
import dis

dis.Bytecode(len)
```

`len` is written in C, so it has no code object and no bytecode to show.

## Comparison

Bytecode is the step after the [[python/the-ast-module]] tree. See [[bytecode]] for the general idea and [[python/code-and-frame-objects]] for the object that holds it. Other Python implementations can use a different format.
