---
tag: 1FXC5Z
kind: feature
owner: python
slug: the-python-grammar
name: The Python grammar
aliases: [Python PEG grammar, python.gram, full grammar specification, PEP 617, Python parser]
area: languages/python/syntax
level: advanced
summary: >-
  The Python grammar is a PEG grammar, the one that generates the CPython parser, and its alternatives are tried in order. The tokenize and ast modules show the tokens and the tree.
grammar: "file: [statements] ENDMARKER; interactive: statement_newline; eval: expressions NEWLINE* ENDMARKER (Language Reference, 10)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/grammar.html", locator: "10. Full Grammar specification"}
see_also: [python/top-level-components, python/soft-keywords, python/operator-precedence-in-python]
sources:
  - url: https://docs.python.org/3.15/reference/grammar.html
    title: The Python Language Reference, Python 3.15
    locator: 10. Full Grammar specification
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/introduction.html#notation
    title: The Python Language Reference, Python 3.15
    locator: 1.2. Notation
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0617/
    title: "PEP 617: New PEG parser for CPython"
    locator: Background on PEG parsers; Migration plan
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tokenize.html
    title: tokenize, Python 3.15 documentation
    locator: tokenize.generate_tokens(), TokenInfo
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/ast.html#ast.parse
    title: ast, Python 3.15 documentation
    locator: ast.parse()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Notation
    title: The Go Programming Language Specification
    locator: Notation
    accessed: 2026-11-16
---

## Synopsis

```python run id=two-views
import ast, io, tokenize

src = "x = 1 + 2 * 3\n"
for tok in tokenize.generate_tokens(io.StringIO(src).readline):
    print(tokenize.tok_name[tok.type], repr(tok.string))
print(ast.dump(ast.parse(src).body[0]))
```

```output for=two-views
NAME 'x'
OP '='
NUMBER '1'
OP '+'
NUMBER '2'
OP '*'
NUMBER '3'
NEWLINE '\n'
ENDMARKER ''
Assign(targets=[Name(id='x', ctx=Store())], value=BinOp(left=Constant(value=1), op=Add(), right=BinOp(left=Constant(value=2), op=Mult(), right=Constant(value=3))))
```

The tokenizer gives a flat list of tokens. The parser follows the grammar and gives a tree, where `2 * 3` is one node inside the `+` node.

## Semantics

**r1.** Chapter 10 of the Language Reference is "the full Python grammar, derived directly from the grammar used to generate the CPython parser (see Grammar/python.gram)". The listing "omits details related to code generation and error recovery".[^1]

**r2.** The notation of the reference is "a mixture of EBNF and PEG". The vertical bar `e1 | e2` is the PEG ordered choice: "if e1 matches, e2 is not considered".[^2] The order of the alternatives is therefore part of the meaning of a rule. Example: [`match-as-name`](#match-as-name).

**r3.** PEP 617 replaced the LL(1) parser of CPython with a PEG parser in Python 3.9. Its migration plan removed the old parser and the `parser` module in Python 3.10.[^3] The PEP gives this property of PEG parsers: "if a string parses, it has exactly one valid parse tree".[^3]

**r4.** The grammar listing adds operators to the notation of chapter 1. `&e` succeeds if `e` can be parsed and `!e` fails if `e` can be parsed, and both consume no input. `s.e+` matches one or more `e` separated by `s`. `~` is a cut, which commits the parser to the current alternative. Names in double quotes, such as `"match"`, are soft keywords, and names in single quotes are keywords.[^1]

**r5.** The grammar has four starting rules: `file`, `interactive`, `eval` and `func_type`.[^1] The `mode` argument of `ast.parse()` selects one of them with the values `'exec'`, `'single'`, `'eval'` and `'func_type'`. The function is equivalent to `compile()` with the `ast.PyCF_ONLY_AST` flag.[^5] Example: [`start-rules`](#start-rules).

**r6.** `tokenize.generate_tokens()` reads source text and yields `TokenInfo` named tuples. Each tuple has the fields `type`, `string`, `start`, `end` and `line`.[^4] The tokenizer does not check the grammar. Example: [`tokens-are-not-syntax`](#tokens-are-not-syntax).

**r7.** `ast.parse()` takes a `feature_version` tuple. With it, the parser makes a "best-effort" attempt to use the grammar of that Python version. For example, `feature_version=(3, 9)` "will attempt to disallow parsing of match statements".[^5] Example: [`old-grammar`](#old-grammar).

## Examples

The rule `match_stmt` starts with the soft keyword `"match"` (rule r4). The parser tries the alternatives in order, so `match` can be a variable name and the subject of a `match` statement in the same program (rule r2).

```python run id=match-as-name
match = [1, 2]
match match:
    case [a, b]:
        print(a + b)
```

```output for=match-as-name
3
```

Each mode of `ast.parse()` starts at a different rule and gives a different root node (rule r5).

```python run id=start-rules
import ast

cases = [
    ("exec", "x = 1\ny = 2\n"),
    ("eval", "x + 1"),
    ("single", "x = 1\n"),
    ("func_type", "(int, str) -> bool"),
]
for mode, src in cases:
    print(mode, type(ast.parse(src, mode=mode)).__name__)
```

```output for=start-rules
exec Module
eval Expression
single Interactive
func_type FunctionType
```

The current grammar parses a `match` statement. The grammar of Python 3.9 does not (rule r7).

```python run id=old-grammar
import ast

src = "match x:\n    case 1:\n        pass\n"
print(type(ast.parse(src).body[0]).__name__)
try:
    ast.parse(src, feature_version=(3, 9))
except SyntaxError as e:
    print("SyntaxError:", e.msg)
```

```output for=old-grammar
Match
SyntaxError: Pattern matching is only supported in Python 3.10 and greater
```

## Pitfalls

### Tokens are not syntax

The tokenizer accepts `1 +* 2`, because each part is a valid token. Only the parser finds the error (rule r6).

```python run id=tokens-are-not-syntax
import ast, io, tokenize

src = "1 +* 2\n"
print([t.string for t in tokenize.generate_tokens(io.StringIO(src).readline)])
try:
    ast.parse(src)
except SyntaxError:
    print("SyntaxError from the parser")
```

```output for=tokens-are-not-syntax
['1', '+', '*', '2', '\n', '']
SyntaxError from the parser
```

### The grammar accepts more than the compiler

In the grammar, `return_stmt` is one of the alternatives of `simple_stmt`, so it can appear at the top level of a file.[^1] The rule that `return` must be inside a function is checked after parsing. `ast.parse()` gives a tree, and the compile step rejects it.

```python run id=parse-then-compile
import ast

tree = ast.parse("return 1\n")
print(type(tree.body[0]).__name__)
try:
    compile(tree, "<demo>", "exec")
except SyntaxError as e:
    print("SyntaxError:", e.msg)
```

```output for=parse-then-compile
Return
SyntaxError: 'return' outside function
```

A tool that uses `ast.parse()` to check a file must also compile it to find these errors.

## Comparison

The Go specification gives its syntax in "a variant of Extended Backus-Naur Form (EBNF)", where `|` separates alternatives.[^6] PEP 617 gives the difference between the two kinds of parser: a context-free-grammar parser decides which alternative to expand, but a PEG parser checks the first alternative "and only if it fails, will it continue with the second or the third one in the order in which they are written".[^3] The chapter 1 notation says that traditional PEG grammars write the ordered choice as a slash, `/`, and Python writes it as `|`.[^2]
