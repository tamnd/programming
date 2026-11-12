```python run id=speech
LEXICON = {"hel lo": "hello", "wor ld": "world"}


def segment(signal):
    return signal[0].split("-")


def lookup(syllables):
    pairs = [" ".join(syllables[i:i + 2]) for i in range(0, len(syllables), 2)]
    return [LEXICON[p] for p in pairs]


def parse(words):
    return [" ".join(words).capitalize() + "."]


# Each knowledge source: name, input level, output level, function.
SOURCES = [
    ("grammar", "words", "sentence", parse),
    ("lexicon", "syllables", "words", lookup),
    ("segmenter", "signal", "syllables", segment),
]

board = {"signal": ["hel-lo-wor-ld"]}

# The control component selects a source that can contribute now.
while "sentence" not in board:
    name, src, dst, run = next(
        s for s in SOURCES if s[1] in board and s[2] not in board
    )
    board[dst] = run(board[src])
    print(f"{name} adds {dst}: {' '.join(board[dst])}")
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. A dictionary is the blackboard, and functions are the knowledge sources.
