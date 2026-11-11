```python run id=tokenize-module
import io
import keyword
import tokenize

src = "if n > 0x_ff:  # a comment\n    total = 1_000\n"
for tok in tokenize.generate_tokens(io.StringIO(src).readline):
    print(f"{tokenize.tok_name[tok.type]:9} {tok.string!r}")
print(0x_ff, 1_000, keyword.iskeyword("match"), keyword.issoftkeyword("match"))
```

```output for=tokenize-module
NAME      'if'
NAME      'n'
OP        '>'
NUMBER    '0x_ff'
OP        ':'
COMMENT   '# a comment'
NEWLINE   '\n'
INDENT    '    '
NAME      'total'
OP        '='
NUMBER    '1_000'
NEWLINE   '\n'
DEDENT    ''
ENDMARKER ''
255 1000 False True
```

- Verdict: stdlib. The `tokenize` module of the standard library runs the lexer of Python on source text and gives the same tokens as the interpreter, comments included. Python is the core language whose lexer does the most work: it turns changes of indentation into `INDENT` and `DEDENT` tokens, which take the place of braces. Underscores group digits in any numeric literal. `match` is a soft keyword, a name that is a keyword only where a `match` statement can start, so old code that uses it as a variable still runs.
