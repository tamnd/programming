---
tag: E2YDNN
kind: feature
owner: python
slug: editors-and-language-servers
name: Editors and Language Servers
aliases: [IDE, language server, Language Server Protocol, LSP, IDLE, idlelib, Jedi, python-lsp-server, pylsp, autocompletion, go to definition, code completion]
area: languages/python/tooling
level: intro
summary: >-
  How an editor knows about Python code: IDLE in the standard library, the Jedi analysis library, and language servers such as pylsp that speak the Language Server Protocol.
grammar: null
spec: {standard: "Language Server Protocol 3.17", url: "https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/", locator: "Base Protocol"}
see_also: [python/linters-and-formatters, python/tab-completion-and-history-editing, python/interactive-mode, python/the-pdb-debugger, python/the-ast-module, python/introspection-with-inspect]
sources:
  - url: https://docs.python.org/3.15/library/idle.html
    title: IDLE, Python 3.15 documentation
    locator: module overview (features)
    accessed: 2026-11-18
  - url: https://jedi.readthedocs.io/en/latest/
    title: Jedi documentation
    locator: front page
    accessed: 2026-11-18
  - url: https://microsoft.github.io/language-server-protocol/overviews/lsp/overview/
    title: Language Server Protocol overview
    locator: What is the Language Server Protocol?; How it works
    accessed: 2026-11-18
  - url: https://github.com/python-lsp/python-lsp-server
    title: python-lsp-server, GitHub
    locator: README, Installation
    accessed: 2026-11-18
  - url: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/
    title: Language Server Protocol Specification 3.17
    locator: Base Protocol; Header Part; Position; Completion Request; Goto Definition Request
    accessed: 2026-11-18
  - url: https://go.dev/gopls/
    title: Gopls, the language server for Go
    locator: front page
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import tempfile

import jedi

source = '''\
import json


def load_config(path, *, strict=False):
    """Read a JSON file."""
    with open(path) as f:
        return json.load(f)


config = load_config('app.json')
json.lo
'''
with tempfile.TemporaryDirectory() as tmp:
    jedi.settings.cache_directory = tmp
    script = jedi.Script(source, path='app.py')
    print('jedi', jedi.__version__)
    print('line 11:', [c.name for c in script.complete(11, len('json.lo'))])
    for name in script.goto(10, len('config = load')):
        print('goto:', name.name, 'line', name.line, 'column', name.column)
    for sig in script.get_signatures(10, len('config = load_config(')):
        print('signature:', sig.to_string())
```

```output for=synopsis
jedi 0.20.1
line 11: ['load', 'loads']
goto: load_config line 4 column 4
signature: load_config(path, *, strict=False)
```

An editor asks three common questions about the code at the cursor: which names can come next, where a name is defined, and which arguments a call takes. The third-party library Jedi answers them from the source text, and nothing in `source` runs: there is no file `app.json`, and no error occurs. At the end of `json.lo` it offers `load` and `loads`; on `load_config` it finds the `def` on line 4; inside the call it gives the signature. The first line of the output shows the Jedi version that made the output.

## Semantics

**r1.** Python includes an editor. "IDLE is Python’s Integrated Development and Learning Environment."[^1] Its features include a "Python shell window (interactive interpreter) with colorizing of code input, output, and error messages" and a "multi-window text editor with multiple undo, Python colorizing, smart indent, call tips, auto completion, and other features".[^1]

**r2.** Other editors get the same features from a library or a server. "Jedi is a static analysis tool for Python that is typically used in IDEs/editors plugins. Jedi has a focus on autocompletion and goto functionality. Other features include refactoring, code search and finding references."[^2] In the synopsis, Jedi reads `source` and does not run it, so the missing `app.json` causes no error.

**r3.** A language server puts this work in a separate process. "The idea behind a Language Server is to provide the language-specific smarts inside a server that can communicate with development tooling over a protocol that enables inter-process communication."[^3] The protocol is shared: "The idea behind the Language Server Protocol (LSP) is to standardize the protocol for how tools and servers communicate, so a single Language Server can be re-used in multiple development tools, and tools can support languages with minimal effort."[^3]

**r4.** A Python language server is a package. python-lsp-server is "A Python 3.9+ implementation of the Language Server Protocol", and "The base language server requires Jedi to provide Completions, Definitions, Hover, References, Signature Help, and Symbols".[^4] Installing it "will expose the command pylsp on your PATH."[^4]

**r5.** Each message has a header and a body. "The base protocol consists of a header and a content part (comparable to HTTP). The header and content part are separated by a ‘\r\n’."[^5] `Content-Length` is "The length of the content part in bytes. This header is required."[^5] The body is a JSON-RPC message, for example a request with `"method": "textDocument/completion"`.[^5]

**r6.** Positions in the protocol count from zero. "A position inside a document (see Position definition below) is expressed as a zero-based line and character offset."[^5] Jedi counts lines from 1: in the synopsis, `complete(11, ...)` asks about the eleventh line of `source`. "The Completion request is sent from the client to the server to compute completion items at a given cursor position."[^5]

## Examples

### Talking to a language server

```python run id=lsp
import json
import os
import subprocess
import sys
import tempfile


def frame(message):
    body = json.dumps(message).encode('utf-8')
    return b'Content-Length: %d\r\n\r\n' % len(body) + body


def read(stream):
    headers = {}
    while (line := stream.readline().decode('ascii').rstrip('\r\n')):
        name, value = line.split(': ', 1)
        headers[name] = value
    return json.loads(stream.read(int(headers['Content-Length'])))


uri = 'file:///project/app.py'
text = 'import json\njson.du\n'
requests = [
    {'jsonrpc': '2.0', 'id': 1, 'method': 'initialize',
     'params': {'processId': None, 'rootUri': None, 'capabilities': {}}},
    {'jsonrpc': '2.0', 'method': 'initialized', 'params': {}},
    {'jsonrpc': '2.0', 'method': 'textDocument/didOpen',
     'params': {'textDocument': {'uri': uri, 'languageId': 'python', 'version': 1, 'text': text}}},
    {'jsonrpc': '2.0', 'id': 2, 'method': 'textDocument/completion',
     'params': {'textDocument': {'uri': uri}, 'position': {'line': 1, 'character': 7}}},
    {'jsonrpc': '2.0', 'id': 3, 'method': 'shutdown'},
]
print(frame(requests[3])[:40])
with tempfile.TemporaryDirectory() as tmp:
    env = dict(os.environ, XDG_CACHE_HOME=tmp)
    server = subprocess.Popen([sys.executable, '-m', 'pylsp'], stdin=subprocess.PIPE,
                              stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, env=env)
    server.stdin.write(b''.join(frame(m) for m in requests))
    server.stdin.flush()
    answers = {}
    while 3 not in answers:
        message = read(server.stdout)
        if 'id' in message:
            answers[message['id']] = message['result']
    server.stdin.write(frame({'jsonrpc': '2.0', 'method': 'exit'}))
    server.stdin.close()
    server.wait(timeout=5)
print('server:', answers[1]['serverInfo'])
print('completion provider:', 'completionProvider' in answers[1]['capabilities'])
items = answers[2]['items'] if isinstance(answers[2], dict) else answers[2]
print('completions:', sorted(item['label'].split('(')[0] for item in items))
print('shutdown result:', answers[3], '| exit code:', server.returncode)
```

```output for=lsp
b'Content-Length: 168\r\n\r\n{"jsonrpc": "2.0"'
server: {'name': 'pylsp', 'version': '1.15.0'}
completion provider: True
completions: ['dump', 'dumps']
shutdown result: None | exit code: 0
```

The program plays the part of the editor. It starts `pylsp`, which reads messages on standard input and writes answers on standard output. The first line shows the start of one framed message: the header, the empty line, and then the JSON body (r5). The editor sends `initialize`, says that it opened a document, and asks for completions at line 1, character 7, which is the end of `json.du` on the second line (r6). The answer to `initialize` names the server and lists what it can do. The answer to the completion request comes from Jedi (r4). `shutdown` and then `exit` stop the server.

## Pitfalls

- A line number from Jedi or from a traceback counts from 1, but an LSP position counts from 0 (r6). Code that moves between them must add or subtract 1.
- `Content-Length` counts bytes, not characters (r5). A body with non-ASCII text is longer in bytes than `len()` of the string.
- The server is a separate package. It is installed with `pip install python-lsp-server`, and "The base language server requires Jedi" (r4). An editor that cannot start `pylsp` has no completions from it.

## Comparison

Go has one language server from the language team. "gopls (pronounced “Go please”) is the official language server for Go, developed by the Go team. It provides a wide variety of IDE features to any LSP -compatible editor."[^6] Its docs add: "You should not need to interact with gopls directly–it will be automatically integrated into your editor."[^6] python-lsp-server is a separate project that a Python user installs with pip (r4). The editor in the Python standard library is IDLE (r1).

[^3]: Language Server Protocol overview.
[^5]: Language Server Protocol Specification 3.17.
[^2]: Jedi documentation.
[^4]: python-lsp-server, GitHub.
[^1]: IDLE, Python 3.15 documentation.
[^6]: Gopls, the language server for Go.
