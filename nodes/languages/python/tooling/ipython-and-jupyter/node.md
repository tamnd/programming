---
tag: Y6EDHV
kind: feature
owner: python
slug: ipython-and-jupyter
name: IPython and Jupyter
aliases: [IPython, Jupyter, Jupyter Notebook, JupyterLab, notebook, .ipynb, nbformat, magic commands, "%timeit", line magic, cell magic, kernel]
area: languages/python/tooling
level: intro
summary: >-
  IPython is an enhanced interactive shell with magic commands, shell escapes and numbered input and output. Jupyter notebooks keep cells and their outputs in an .ipynb JSON file, run by a kernel.
grammar: null
spec: {standard: null, url: "https://nbformat.readthedocs.io/en/latest/format_description.html", locator: "Top-level structure"}
see_also: [python/interactive-mode, python/interactive-input-editing-and-history-substitution, python/tab-completion-and-history-editing, python/the-interactive-startup-file, python/performance-measurement, python/editors-and-language-servers]
sources:
  - url: https://ipython.readthedocs.io/en/stable/interactive/tutorial.html
    title: Introducing IPython, IPython documentation
    locator: The four most helpful commands; Magic functions; History; System shell commands
    accessed: 2026-11-18
  - url: https://ipython.readthedocs.io/en/stable/interactive/magics.html
    title: Built-in magic commands, IPython documentation
    locator: "%time; %timeit; %who"
    accessed: 2026-11-18
  - url: https://docs.jupyter.org/en/latest/what_is_jupyter.html
    title: What is Jupyter?, Jupyter documentation
    locator: kernels; the .ipynb file format
    accessed: 2026-11-18
  - url: https://nbformat.readthedocs.io/en/latest/format_description.html
    title: The Notebook file format, nbformat documentation
    locator: Top-level structure; Code cells; execute_result
    accessed: 2026-11-18
  - url: https://nbformat.readthedocs.io/en/latest/api.html
    title: Python API for working with notebook files, nbformat documentation
    locator: validate(); Constructing notebooks programmatically
    accessed: 2026-11-18
  - url: https://go.dev/play/
    title: The Go Playground
    locator: About the Playground
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import os
import subprocess
import sys
import tempfile

session = '''\
x = [3, 1, 2]
sorted(x)
_[0] + 10
Out[2]
%who
words = !echo red green
words.s
exit
'''
with tempfile.TemporaryDirectory() as tmp:
    env = dict(os.environ, IPYTHONDIR=tmp)
    result = subprocess.run([sys.executable, '-m', 'IPython', '--simple-prompt', '--no-banner',
                             '--colors=nocolor', '--HistoryManager.enabled=False'],
                            input=session, env=env, cwd=tmp, capture_output=True, text=True)
print(subprocess.run([sys.executable, '-m', 'IPython', '--version'], capture_output=True, text=True).stdout.strip())
for line in result.stdout.splitlines():
    if line.strip():
        print(line.rstrip())
print('stderr:', result.stderr.strip() or None)
```

```output for=synopsis
9.18.1
In [1]:
In [2]: Out[2]: [1, 2, 3]
In [3]: Out[3]: 11
In [4]: Out[4]: [1, 2, 3]
In [5]: x
In [6]:
In [7]: Out[7]: 'red green'
In [8]:
stderr: None
```

The lines go to IPython, a third-party interactive shell, through a pipe; the first output line is its version. Each input gets a number, and each result is kept as `Out[n]`. `_` is the last result, so `_[0] + 10` is 11, and `Out[2]` brings back the result of line 2. `%who` is a magic command, not Python: it lists the variables that the session made. `!echo red green` runs a shell command, and `words.s` joins its output lines with spaces. `IPYTHONDIR` points IPython at a temporary directory for its profile, so the run leaves no files behind.

## Semantics

**r1.** IPython adds commands to the Python syntax. "IPython has a set of predefined ‘magic functions’ that you can call with a command line style syntax. There are two kinds of magics, line-oriented and cell-oriented. Line magics are prefixed with the % character and work much like OS command-line calls: they get as an argument the rest of the line, where arguments are passed without parentheses or quotes."[^1] "Cell magics are prefixed with a double %% , and they are functions that get as an argument not only the rest of the line, but also the lines below it in a separate argument."[^1]

**r2.** Some magics measure time. `%time` will "Time execution of a Python statement or expression. The CPU and wall clock times are printed, and the value of the expression (if any) is returned."[^2] `%who` will "Print all interactive variables, with some minimal formatting."[^2]

**r3.** The session keeps its inputs and results. "Input and output history are kept in variables called In and Out , keyed by the prompt numbers, e.g. In[4] . The last three objects in output history are also kept in variables named _ , __ and ___ ."[^1]

**r4.** The shell is one prefix away. "To run any command at the system shell, simply prefix it with ! , e.g.: !ping www.bbc.co.uk You can capture the output into a Python list, e.g.: files = !ls ."[^1] Help is a suffix: `object?` gives "Details about ‘object’, use ‘object??’ for extra details."[^1]

**r5.** A Jupyter notebook runs its cells in a kernel. "Jupyter kernels themselves are independent programs (operating system processes), and each one is its own REPL in whatever language you requested".[^3] "This is why you don’t lose your data and variables when you execute multiple notebook cells in a row, they stay alive inside the REPL".[^3]

**r6.** A notebook is a file. "The .ipynb format is a type of json document, which holds your notebook cells, program outputs, and metadata inside a structured text file."[^3] "At the highest level, a Jupyter notebook is a dictionary with a few keys: metadata (dict) nbformat (int) nbformat_minor (int) cells (list)".[^4] "Code cells are the primary content of Jupyter notebooks. They contain source code in the language of the document’s associated kernel, and a list of outputs associated with executing that code. They also have an execution_count, which must be an integer or null ."[^4]

## Examples

### What a magic becomes

```python run id=transform
from IPython.core.inputtransformer2 import TransformerManager

manager = TransformerManager()
for cell in ['%timeit sorted(data)', 'files = !ls *.py', 'len?', '%%time\ntotal = sum(range(10))\n']:
    print(repr(cell), '->')
    print('   ', manager.transform_cell(cell).rstrip())
```

```output for=transform
'%timeit sorted(data)' ->
    get_ipython().run_line_magic('timeit', 'sorted(data)')
'files = !ls *.py' ->
    files = get_ipython().getoutput('ls *.py')
'len?' ->
    get_ipython().run_line_magic('pinfo', 'len')
'%%time\ntotal = sum(range(10))\n' ->
    get_ipython().run_cell_magic('time', '', 'total = sum(range(10))\n')
```

IPython turns each special line into ordinary Python before it runs it. A line magic becomes a call with the rest of the line as one string; a cell magic gets the lines below it as a separate string (r1). `!` becomes a call that captures the output, and `?` becomes the `pinfo` magic. This is why the code in a notebook cell with magics fails when it is copied into a `.py` file: `get_ipython` exists only inside IPython.

### Building and checking a notebook file

```python run id=notebook
import json

import nbformat
from nbformat.v4 import new_code_cell, new_markdown_cell, new_notebook, new_output

notebook = new_notebook(cells=[
    new_markdown_cell('# Totals'),
    new_code_cell('sum([1, 2, 3])', execution_count=1,
                  outputs=[new_output('execute_result', {'text/plain': '6'}, execution_count=1)]),
])
nbformat.validate(notebook)
text = nbformat.writes(notebook)
data = json.loads(text)
print('top-level keys:', sorted(data))
print('format:', data['nbformat'], data['nbformat_minor'])
for cell in data['cells']:
    print(cell['cell_type'], repr(cell['source']), cell.get('outputs', '-'))
broken = nbformat.reads(text, as_version=4)
broken.cells[1].execution_count = 'one'
try:
    nbformat.validate(broken)
except nbformat.ValidationError as error:
    print('ValidationError:', error.message)
```

```output for=notebook
top-level keys: ['cells', 'metadata', 'nbformat', 'nbformat_minor']
format: 4 5
markdown ['# Totals'] -
code ['sum([1, 2, 3])'] [{'data': {'text/plain': ['6']}, 'execution_count': 1, 'metadata': {}, 'output_type': 'execute_result'}]
ValidationError: 'one' is not of type 'integer', 'null'
```

The third-party `nbformat` package is the library that reads and writes `.ipynb` files. The notebook has a Markdown cell and a code cell with a stored result. "Results of executing a cell (as created by displayhook in Python) are stored in execute_result outputs."[^4] The JSON text has the four top-level keys (r6). `validate` "Checks whether the given notebook dict-like object conforms to the relevant notebook format schema."[^5] An `execution_count` must be "an integer or null" (r6), so a string is an error.

## Pitfalls

- Magics and `!` lines are not Python (r1, r4). A `.py` file made from a notebook needs them replaced with ordinary code.
- A notebook keeps the outputs of the last run in the file (r6). A cell that was changed and not run again still shows the old output, and the `execution_count` numbers show the order in which the cells ran.
- The variables live in the kernel, not in the file (r5). Opening a saved notebook again shows the outputs, but the variables are gone until the cells run again.
- `%time` measures one run (r2). `%timeit` will "Time execution of a Python statement or expression using the timeit module."[^2] Use it for small statements.

## Comparison

For Go, a near tool is the Go Playground, a web page. "The Go Playground is a web service that runs on go.dev 's servers. The service receives a Go program, vets , compiles, links, and runs the program inside a sandbox, then returns the output."[^6] Like a notebook cell, it shows the code next to its output, but each run is a whole program, and no kernel keeps variables between runs as in r5. "The only communication a playground program has to the outside world is by writing to standard output and standard error."[^6]

[^1]: Introducing IPython, IPython documentation.
[^2]: Built-in magic commands, IPython documentation.
[^3]: What is Jupyter?, Jupyter documentation.
[^4]: The Notebook file format, nbformat documentation.
[^5]: Python API for working with notebook files, nbformat documentation.
[^6]: The Go Playground.
