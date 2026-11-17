---
tag: 16WFFS
kind: feature
owner: python
slug: tab-completion-and-history-editing
name: Tab Completion and History Editing
aliases: [tab completion, rlcompleter, rlcompleter.Completer, readline.set_completer, completer function, completion delimiters, sys.__interactivehook__, attribute completion]
area: languages/python/tooling
level: core
summary: >-
  How Tab completes names at the interactive prompt: rlcompleter, the state protocol of a completer function, dotted names that run __getattr__, and how site turns it all on.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/rlcompleter.html", locator: "Completer objects"}
see_also: [python/interactive-input-editing-and-history-substitution, python/the-interactive-startup-file, python/the-customization-modules, python/interactive-mode, python/the-dir-function, python/attribute-access-customization]
sources:
  - url: https://docs.python.org/3.15/tutorial/interactive.html
    title: Interactive Input Editing and History Substitution, The Python Tutorial 3.15
    locator: 14.1. Tab Completion and History Editing; 14.2. Alternatives to the Interactive Interpreter
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/site.html
    title: site, Python 3.15 documentation
    locator: Readline configuration
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/readline.html
    title: readline, Python 3.15 documentation
    locator: Completion
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/rlcompleter.html
    title: rlcompleter, Python 3.15 documentation
    locator: module overview; Completer.complete
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.__interactivehook__
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Show documentation for package or symbol
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import math
import rlcompleter

namespace = {'value': 1, 'values': [1, 2], 'math': math}
completer = rlcompleter.Completer(namespace)


def completions(text):
    found = []
    state = 0
    while (match := completer.complete(text, state)) is not None:
        found.append(match)
        state += 1
    return found


for text in ['val', 'math.fl', 'pri', 'imp', 'nosuch.a']:
    print(f'{text!r:11}', completions(text))
```

```output for=synopsis
'val'       ['value', 'values']
'math.fl'   ['math.floor(']
'pri'       ['print(']
'imp'       ['import ']
'nosuch.a'  []
```

This is what Tab does at the prompt, without a terminal. The loop asks the completer for match 0, 1, 2 and so on until it gets `None`, which is how readline calls it (r2). Plain words are completed from the namespace, the builtins and the keywords. `math.fl` is completed from the attributes of `math`. The completer adds `(` after a callable and a space after a keyword, as the output shows. `nosuch` is not defined, so there is nothing to complete.

## Semantics

**r1.** At the prompt, completion is on by default. "Completion of variable and module names is automatically enabled at interpreter startup so that the Tab key invokes the completion function; it looks at Python statement names, the current local variables, and the available module names."[^1] `site` does this: it "will also import and configure the rlcompleter module, if Python is started in interactive mode and without the -S option".[^2]

**r2.** A completer is a function of `(text, state)`. It "is called as function(text, state) , for state in 0 , 1 , 2 , …, until it returns a non-string value. It should return the next possible completion starting with text ."[^3] `readline.set_completer(function)` installs it, and `get_completer` returns it.[^3]

**r3.** `rlcompleter` provides the standard one. "When this module is imported on a Unix platform with the readline module available, an instance of the Completer class is automatically created and its complete() method is set as the readline completer ."[^4] "On platforms without readline , the Completer class defined by this module can still be used for custom purposes."[^4]

**r4.** For a word without a dot, `Completer.complete` "will complete from names currently defined in __main__ , builtins and keywords (as defined by the keyword module)".[^4] Passing a dict to `Completer` uses that dict in place of `__main__`, as in the synopsis.

**r5.** For a dotted name, "it will try to evaluate anything without obvious side-effects (functions will not be evaluated, but it can generate calls to __getattr__() ) up to the last part, and find matches for the rest via the dir() function. Any exception raised during the evaluation of the expression is caught, silenced and None is returned."[^4] The tutorial warns: "Note that this may execute application-defined code if an object with a __getattr__() method is part of the expression."[^1]

**r6.** The word to complete is found with delimiter characters. `set_completer_delims` and `get_completer_delims` "determine the start of the word to be considered for completion (the completion scope)".[^3] "If the readline module is to be used with a custom completer, a different set of word delimiters should be set."[^3]

**r7.** The setup runs from a hook. `sys.__interactivehook__` "is automatically called (with no arguments) when the interpreter is launched in interactive mode . This is done after the PYTHONSTARTUP file is read, so that you can set this hook there. The site module sets this ."[^5]

## Examples

### Completion can run your code

```python run id=getattr
import rlcompleter


class Remote:
    def __dir__(self):
        return ['fetch', 'free']

    def __getattr__(self, name):
        print(f'  __getattr__({name!r}) ran')
        return 0


completer = rlcompleter.Completer({'server': Remote()})
print(completer.complete('server.f', 0))
print(completer.complete('server.f', 1))
```

```output for=getattr
  __getattr__('free') ran
  __getattr__('fetch') ran
server.fetch
server.free
```

`dir(server)` lists `fetch` and `free`, but neither is a real attribute, so looking them up runs `__getattr__` (r5). Pressing Tab after `server.f` would do the same. If `__getattr__` made a network call, so would Tab. The first call works out all the matches; the second returns the next one from that list.

### A completer of your own

```python run id=custom
COMMANDS = ['start', 'status', 'stop', 'help']


def complete(text, state):
    matches = [c for c in COMMANDS if c.startswith(text)]
    return matches[state] if state < len(matches) else None


for text in ['st', 'h', 'x']:
    state = 0
    found = []
    while (match := complete(text, state)) is not None:
        found.append(match)
        state += 1
    print(repr(text), found)
```

```output for=custom
'st' ['start', 'status', 'stop']
'h' ['help']
'x' []
```

This function follows the protocol in r2, so `readline.set_completer(complete)` would make Tab complete these command names, for example at an `input()` prompt in a small command shell. The loop here plays the part of readline.

## Pitfalls

- Tab after a dotted name evaluates the part before the last dot, and can call `__getattr__` (r5). With objects that talk to a database or a server, completion can be slow or have side effects.
- A failing evaluation is silent (r5): no completions appear, and no error is shown. A missing completion is not proof that the attribute does not exist.
- A custom completer with the default delimiters may get only part of the word. Set delimiters that suit the input (r6).
- With `-S` there is no tab completion at the prompt, because `site` sets it up (r1).
- The tutorial lists what the built-in prompt lacks, such as suggested indentation on continuation lines, and points to "IPython , which features tab completion, object exploration and advanced history management" and to bpython.[^1]

## Comparison

Go's toolchain has no prompt to complete at. Its nearest tool for listing what a package holds is `go doc`: it "prints the documentation comments associated with the item identified by its arguments (a package, const, func, type, var, method, or struct field) followed by a one-line summary of each of the first-level items "under" that item".[^6] So `go doc math` lists the names in `math`, much as completing `math.` does in Python, but as a separate command rather than at a prompt.

[^1]: Interactive Input Editing and History Substitution, The Python Tutorial 3.15.
[^4]: rlcompleter, Python 3.15 documentation.
[^3]: readline, Python 3.15 documentation.
[^2]: site, Python 3.15 documentation.
[^5]: sys, Python 3.15 documentation.
[^6]: Command go, Go documentation.
