---
tag: 11V81G
kind: pattern
slug: blackboard
name: Blackboard
aliases: [blackboard pattern, blackboard architecture, blackboard system]
area: software-design/behavioral-patterns
level: advanced
summary: >-
  In the blackboard pattern, independent knowledge sources add partial solutions to a shared data store, and a control component selects which source runs next.
family: behavioral-design
polarity: positive
intent: Assemble knowledge from a variety of sources when no deterministic solution strategy is known.
also_known_as: [blackboard system]
history:
  year: 1980
  people: [L. D. Erman, F. Hayes-Roth, V. R. Lesser, D. R. Reddy]
  note: The pattern was identified by the members of the Hearsay-II project and first applied to speech recognition. Erman, Hayes-Roth, Lesser and Reddy described the Hearsay-II speech-understanding system in ACM Computing Surveys in 1980. Buschmann and his co-authors later described Blackboard as an architectural pattern in Pattern-Oriented Software Architecture, Volume 1.
see_also: [mediator-pattern, observer-pattern, strategy-pattern, repository-pattern, event-driven-programming, mutual-exclusion]
sources:
  - url: https://en.wikipedia.org/wiki/Blackboard_(design_pattern)
    title: Blackboard (design pattern), Wikipedia
    locator: lead; Structure; Implementation; Applications; Consequences
  - url: https://en.wikipedia.org/wiki/Blackboard_system
    title: Blackboard system, Wikipedia
    locator: lead and its citation of Erman, Hayes-Roth, Lesser and Reddy (1980); Metaphor; Components; Implementations
  - url: https://www.cs.uni.edu/~wallingf/teaching/062/sessions/support/pattern-examples.pdf
    title: "Examples to Accompany: Design Patterns, Elements of Reusable Object-Oriented Software, AG Communication Systems, 1999"
    locator: Blackboard, page A-38
  - book: isbn:9780471958697
    title: "Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad and Michael Stal, Pattern-Oriented Software Architecture, Volume 1: A System of Patterns, John Wiley and Sons"
    locator: Chapter 2, Architectural Patterns, Blackboard
---

## Intent

The Blackboard pattern is used to assemble knowledge from a variety of sources when no deterministic solution strategy is known.[^3] Buschmann and his co-authors describe Blackboard as an architectural pattern in Pattern-Oriented Software Architecture, Volume 1.[^4]

## Problem

Some problems have no known fixed algorithm. Speech recognition is the first example: the pattern was identified by the members of the Hearsay-II project and first applied to speech recognition.[^1] Other uses include vehicle identification and tracking, protein structure identification and the interpretation of sonar signals.[^1]

For such a problem, many specialized modules each know a part of the answer. Each module has its own representation. The system must combine large and diverse specialized modules and use complex, non-deterministic control strategies.[^1]

## Solution

The blackboard model defines three main components:[^1]

- The **blackboard**, a structured global memory that contains objects from the solution space.
- The **knowledge sources**, specialized modules with their own representation.
- The **control component**, which selects, configures and executes modules.

```text
   +-------------------------------------------+
   |               blackboard                  |
   |   signal -> syllables -> words -> sentence |
   +-------------------------------------------+
        ^ read and write    ^           ^
        |                   |           |
   segmenter            lexicon      grammar     (knowledge sources)
        ^                   ^           ^
        +---------- control component --+
           selects the next source that can contribute
```

The metaphor is a group of specialists in a room with a large blackboard. The specialists all watch the blackboard and look for an opportunity to apply their expertise to the developing solution. When one of them writes something that lets another specialist apply their expertise, the second specialist records their contribution on the blackboard.[^2] A moderator keeps the specialists from all trying to write at the same time. In a blackboard system, the control shell does this work.[^2]

To implement the pattern, first design the solution space, which gives the structure of the blackboard. Then identify the knowledge sources. These two activities are closely related. Then specify the control component.[^1]

## Example

The example files show one program in each language. The blackboard has four levels: a signal, syllables, words and a sentence. Each knowledge source can contribute when its input level is on the blackboard and its output level is not. The control component looks at the blackboard and runs a source that can contribute, until the sentence is there. The program prints:

```text
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

## Consequences

The benefits:

- The pattern gives effective solutions for complex systems in which different modules must be combined dynamically to solve a problem.[^1]
- It gives reusability, changeability and robustness.[^1] The knowledge sources are independent, so it is easy to change and maintain them. It is easy to experiment with different approaches.[^3]
- Only conclusions that are supported by data and other hypotheses survive.[^3]
- Multiple processes can work closer together on separate threads, and poll and react when necessary.[^1]

The costs:

- The control component generally takes the form of a complex scheduler. It uses a set of domain-specific heuristics to rate the relevance of the knowledge sources that can run.[^1]

A fixed order of steps is not a blackboard. Here each source runs once in a fixed order. The grammar runs before there are words, so the program stops with no sentence:

```python wrong id=fixed-order
LEXICON = {"hel lo": "hello", "wor ld": "world"}
board = {"signal": "hel-lo-wor-ld"}


def segmenter(b):
    if "signal" in b:
        b["syllables"] = b["signal"].split("-")


def lexicon(b):
    if "syllables" in b:
        s = b["syllables"]
        b["words"] = [LEXICON[" ".join(s[i:i + 2])] for i in range(0, len(s), 2)]


def grammar(b):
    if "words" in b:
        b["sentence"] = " ".join(b["words"]).capitalize() + "."


for source in [grammar, lexicon, segmenter]:  # a fixed order, no control
    source(board)
print("sentence:", board.get("sentence"))
```

```output for=fixed-order
sentence: None
```

## When not to use it

- A deterministic algorithm for the problem is known.[^3] Then a direct program, or a pipeline of steps, is simpler.
- The parts of the solution must be produced in an order that is known before the program runs.

## Variants and related patterns

- **Rated knowledge sources.** The control component can rate each knowledge source that can run and select the best one.[^1] Here two lexicons can turn syllables into words, and the control selects the one with the higher rating:

```python run id=rated
board = {"syllables": ["wor", "ld"]}
sources = [
    ("short lexicon", 0.4, {"wor ld": "word"}),
    ("long lexicon", 0.9, {"wor ld": "world"}),
]


def rate(source):
    name, rating, table = source
    return rating if " ".join(board["syllables"]) in table else 0.0


name, rating, table = max(sources, key=rate)  # the control rates the sources
board["words"] = [table[" ".join(board["syllables"])]]
print(f"{name} ({rating}) adds words: {' '.join(board['words'])}")
```

```output for=rated
long lexicon (0.9) adds words: world
```

- **Control knowledge sources.** In the BB1 system, the control itself used the same blackboard model. Control knowledge sources could monitor whether problem solving was going as expected or had stalled.[^2]
- **Mediator.** In both patterns, the parts do not call each other directly. In a blackboard, the knowledge sources share only the data on the blackboard, and the control component selects, configures and executes them.[^1]

## Known uses

- **Hearsay-II.** A speech recognition system, and one of the well-known early academic blackboard systems.[^2]
- **BB1 and GBB.** Two foundational blackboard systems. BB1 focused on opportunistic planning and control, and GBB focused on efficiency.[^2]
- **Copycat and Numbo.** Douglas Hofstadter's projects are also blackboard systems.[^2]
- **RADARSAT-1.** The PLAN component of the Mission Control System for this Canadian Earth observation satellite.[^2]
- **Adobe Acrobat Capture.** It used a blackboard system to decompose and recognize image pages.[^2]
- **Game AI.** Blackboards are considered a standard AI tool in video games.[^2]

## References
