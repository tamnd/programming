---
tag: C9V35Z
kind: concept
slug: context-free-grammars
name: Context-free grammars
aliases: [grammar, CFG, BNF, Backus-Naur form, EBNF, ABNF, PEG, parsing expression grammar, production rule, nonterminal, terminal, ambiguous grammar, syntax notation]
area: syntax-and-semantics/grammars-and-notation
level: core
summary: >-
  A context-free grammar defines the syntax of a language with rules that replace a name by a sequence of names and symbols. BNF, EBNF and PEG are the notations that language definitions use.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Noam Chomsky, John Backus, Peter Naur, Niklaus Wirth, Bryan Ford]
  note: Chomsky defined context-free grammars in 1956, Backus and Naur used the notation now called BNF for Algol 60 in 1960, Wirth proposed EBNF in 1977, and Ford defined PEGs in 2004.
see_also: [tokens-and-lexing, recursive-descent-parsing, abstract-syntax-tree, finite-state-machines, statements-and-expressions, order-of-evaluation, syntactic-macros]
same_as:
  wikidata: Q338047
  wikipedia: Context-free_grammar
sources:
- url: https://bford.info/pub/lang/peg.pdf
  title: 'Bryan Ford, Parsing Expression Grammars: A Recognition-Based Syntactic Foundation, POPL 2004'
  locator: Abstract and Introduction
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60
  locator: 1.1 Formalism for syntactic description
- url: https://go.dev/ref/spec#Notation
  title: The Go Programming Language Specification
  locator: Notation
- url: https://docs.python.org/3/reference/introduction.html#notation
  title: Introduction, The Python Language Reference
  locator: 1.2 Notation
- url: https://www.rfc-editor.org/rfc/rfc5234
  title: 'RFC 5234, Augmented BNF for Syntax Specifications: ABNF'
  locator: 1. Introduction
- url: https://en.cppreference.com/w/cpp/language/if.html
  title: if statement, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/direct_initialization.html
  title: Direct-initialization, cppreference.com
  locator: Notes
- url: https://peps.python.org/pep-0617/
  title: PEP 617, New PEG parser for CPython
  locator: Background on PEG parsers
---

## Idea

A grammar defines which sequences of tokens are programs of a language. A context-free grammar does this with production rules: each rule says that a name, a nonterminal, can be replaced by a sequence of nonterminals and tokens. The rule `E -> E "+" T` says that an expression can be an expression, a plus sign and a term. A sequence of tokens belongs to the language when the rules can derive it from the start symbol, and the steps of that derivation form a parse tree. Language references write their grammars in notations for such rules: BNF, EBNF, ABNF and, more and more, parsing expression grammars, or PEGs.

```python run id=idea
from functools import cache

AMBIGUOUS = {  # E -> E "+" E | E "*" E | num
    "E": [["E", "+", "E"], ["E", "*", "E"], ["num"]],
}
LAYERED = {  # E -> E "+" T | T ;  T -> T "*" F | F ;  F -> num
    "E": [["E", "+", "T"], ["T"]],
    "T": [["T", "*", "F"], ["F"]],
    "F": [["num"]],
}


def parses(grammar, tokens):
    @cache
    def trees(sym, i, j):  # every tree that derives tokens[i:j] from sym
        if sym not in grammar:  # a terminal matches exactly one token
            ok = j == i + 1 and (tokens[i] == sym or sym == "num" and tokens[i].isdigit())
            return [tokens[i]] if ok else []
        out = []
        for alt in grammar[sym]:
            for kids in fill(alt, i, j):
                out.append(kids[0] if len(kids) == 1 else "(" + " ".join(kids) + ")")
        return out

    def fill(alt, i, j):  # share tokens[i:j] among the symbols of alt
        if len(alt) == 1:
            return [[t] for t in trees(alt[0], i, j)]
        return [[t] + rest for k in range(i + 1, j - len(alt) + 2)
                for t in trees(alt[0], i, k) for rest in fill(alt[1:], k, j)]

    return trees("E", 0, len(tokens))


for text in ["1 + 2 * 3", "1 + 2 + 3 + 4 + 5"]:
    for name, grammar in [("ambiguous", AMBIGUOUS), ("layered", LAYERED)]:
        found = parses(grammar, text.split())
        print(f"{name:9} {text:17} {len(found):2} trees  {found[:2]}")
```

```output for=idea
ambiguous 1 + 2 * 3          2 trees  ['(1 + (2 * 3))', '((1 + 2) * 3)']
layered   1 + 2 * 3          1 trees  ['(1 + (2 * 3))']
ambiguous 1 + 2 + 3 + 4 + 5 14 trees  ['(1 + (2 + (3 + (4 + 5))))', '(1 + (2 + ((3 + 4) + 5)))']
layered   1 + 2 + 3 + 4 + 5  1 trees  ['((((1 + 2) + 3) + 4) + 5)']
```

The program stores each grammar as data, a map from a nonterminal to its alternatives, and finds every parse tree for a list of tokens by trying each way to share the tokens among the symbols of each rule. Both grammars accept the same expressions. The first grammar is ambiguous: it gives two trees for `1 + 2 * 3` and 14 trees for a sum of five numbers, so it does not say what the expression means. The layered grammar gives one tree for each input, with `*` below `+` and each sum grouped from the left, because each rule has only one way to apply.

## How it works

**Rules, terminals and nonterminals.** A context-free grammar has four parts: the terminals, which are the tokens that a lexer produces; the nonterminals, which name the constructs of the language; the production rules; and a start symbol. "Context-free" means that a rule for a nonterminal applies wherever the nonterminal appears, whatever is around it. Bryan Ford sums up the position of these grammars: "For decades we have been using Chomsky’s generative system of grammars, particularly context-free grammars (CFGs) and regular expressions (REs), to express the syntax of programming languages".[^1] Regular expressions describe the tokens, as in [tokens and lexing](/tokens-and-lexing/), and context-free grammars describe how tokens nest.

**BNF.** The Algol 60 report introduced the notation now called Backus-Naur form. A rule such as `<ab> ::= ( | [ | <ab> ( | <ab> <d>` defines `<ab>`, and the report explains: "Sequences of characters enclosed in the bracket <> represent metalinguistic variables whose values are sequences of symbols."[^2] The `::=` reads "is defined as", the `|` separates alternatives, and repetition needs a recursive rule.

**EBNF and its relatives.** Extended BNF adds operators for the patterns that BNF writes with extra rules: brackets for an optional part, braces for repetition, and parentheses for grouping. The Go specification states: "The syntax is specified using a variant of Extended Backus-Naur Form (EBNF)", and it adds a naming rule: "Lowercase production names are used to identify lexical (terminal) tokens. Non-terminals are in CamelCase."[^3] Python's reference uses "a grammar notation that is a mixture of EBNF and PEG".[^4] Internet standards use ABNF; RFC 5234 says that "a modified version of Backus-Naur Form (BNF), called Augmented BNF (ABNF), has been popular among many Internet specifications."[^5] Each notation describes the same class of languages, with a different syntax for the rules.

**Ambiguity, precedence and associativity.** A grammar is ambiguous when one input has two parse trees, as the first grammar in the example shows. A language definition must remove the ambiguity, because each tree gives the program a different meaning. Layered grammars do this for operators: one nonterminal for each level of precedence, with the tighter operators lower in the tree. Left-recursive rules such as `E -> E "+" T` make an operator group from the left, and right-recursive rules make it group from the right. Where a grammar stays ambiguous, the specification adds a rule in words. C and C++ attach an `else` to the nearest `if`, and cppreference states that "the else is associated with the closest if that does not yet have an associated else".[^6] C++ reads a statement that can be a declaration as a declaration, a rule that "is sometimes counter-intuitive and has been called the most vexing parse".[^7]

**Parsing expression grammars.** A PEG looks like EBNF but describes a parser, not a set of strings. Ford writes: "Where CFGs express nondeterministic choice between alternatives, PEGs instead use prioritized choice."[^1] PEP 617, which moved CPython to a PEG parser in Python 3.9, puts it the same way: "The fundamental technical difference is that the choice operator is ordered."[^8] A PEG is never ambiguous, because the first alternative that matches wins, and a packrat parser runs any PEG in linear time by remembering the results of each rule at each position. The cost is that alternatives must be in the right order, as the Python example shows with `<` and `<=`.

**From a grammar to a parser.** A grammar is also a plan for a parser. [Recursive descent](/recursive-descent-parsing/) writes one function for each nonterminal; parser generators such as yacc, Bison and ANTLR read the grammar and produce the parser; parser combinators build it from functions that mirror the operators of EBNF. Each method has limits. A recursive descent parser cannot use left recursion as written, which PEP 617 defines: "A rule is left-recursive if it can derive to a sentential form with itself as the leftmost symbol."[^8] Such rules become loops over EBNF repetition, and LR generators accept them directly. The parser builds an [abstract syntax tree](/abstract-syntax-tree/), which keeps the structure and drops the punctuation.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | library | Lark, pyparsing, ANTLR; CPython parses with a PEG |
| JavaScript | library | Peggy, nearley, Chevrotain; parser combinators |
| TypeScript | library | as JavaScript, with typed parse trees |
| Java | library | ANTLR, JavaCC, parboiled |
| Kotlin | library | ANTLR, better-parse |
| C# | library | ANTLR, Pidgin, Superpower; regex balancing groups |
| Go | library | `goyacc`, `golang.org/x/exp/ebnf`; recursive descent |
| Rust | library | pest, LALRPOP, nom, chumsky |
| C | library | yacc, GNU Bison, flex |
| C++ | library | Boost.Spirit, PEGTL, Bison |
| Bash | n/a | no notation for grammars |
| PowerShell | idiom | .NET regular expressions with balancing groups |
| SQL | n/a | queries cannot define grammars |
| Regex | native | a regular expression is a grammar for a regular language |
| HTML | n/a | markup cannot define a grammar |
| CSS | n/a | the value definition syntax is for specifications only |

Every language has a grammar, but no core language of this site has a notation for grammars in its standard library, so the table records how programs get one. Parser generators and combinator libraries exist for all of the general-purpose languages, and ANTLR alone has targets for most of them. Regex is the exception in the other direction: a regular expression is itself a grammar, for the smaller class of regular languages, and the .NET and PCRE engines add features that reach past that class.

Each example file shows a different side of grammars. The Python file shows the ordered choice of a PEG and the effect of right recursion. The JavaScript file writes EBNF rules as parser combinators, and the Rust file turns a layered grammar into recursive descent with loops. The Java file uses a grammar to generate sentences, and the Go file checks a grammar for undefined and unused rules. The C# file matches nested parentheses, which a true regular expression cannot do. The C and C++ files show two places where the grammar of the language is ambiguous and a rule in the standard decides: the dangling else and the most vexing parse.

## Variations and related ideas

- **The Chomsky hierarchy.** Regular grammars describe tokens, context-free grammars describe nesting, and context-sensitive and unrestricted grammars describe more but are hard to parse. See [finite-state machines](/finite-state-machines/).
- **LL and LR grammars.** Subsets of the context-free grammars that a parser can handle with one token of lookahead, top-down for LL and bottom-up for LR and LALR. Parser generators report a conflict when a grammar is outside the subset.
- **Attribute grammars.** Rules carry computations, such as types or values, that flow up and down the parse tree.
- **Syntax diagrams.** Railroad diagrams draw each rule as a track, as in the JSON home page and the Pascal and SQLite manuals.
- **Scannerless parsing.** PEGs and some generalized parsers read characters directly, with no separate lexer, so the grammar also holds the lexical rules.
- **Generalized parsing.** GLR, Earley and GLL parsers accept every context-free grammar, including ambiguous ones, and return all the trees, as the example does.
- **Island and fuzzy grammars.** Tools that read code in many languages, or code with errors, parse only the parts that they need.
- **Macros and syntax extension.** Some languages let programs add rules to the grammar. See [syntactic macros](/syntactic-macros/).

## Pitfalls

- **Ambiguous grammars.** A grammar with two trees for one input gives two meanings. Layer the rules by precedence, or state the rule that picks one tree, and test the grammar with a parser generator that reports conflicts.
- **The wrong associativity.** A right-recursive rule makes subtraction group from the right, so `10 - 3 - 2` gives 9. The wrong block in the Python file shows this. Use EBNF repetition and build the tree in a loop.
- **Left recursion in a top-down parser.** A recursive descent parser for `E -> E "+" T` calls itself forever. Rewrite the rule as `E -> T { "+" T }`.
- **Order of PEG alternatives.** A PEG choice commits to the first alternative that matches, so a short alternative before a longer one with the same start hides the longer one. Put the longer alternative first.
- **The dangling else.** Indentation does not decide which `if` owns an `else` in C, C++, Java, C# and JavaScript. The wrong block in the C file shows this. Use braces.
- **Regular expressions for nested text.** HTML, JSON and source code nest without limit, and a true regular expression handles only a fixed depth. Use a parser.

## Cost

A recursive descent or LL parser for an LL grammar, and an LR parser for an LR grammar, run in time linear in the number of tokens. A packrat parser runs a PEG in linear time but stores a result for each rule at each position, so its memory grows with the input times the number of rules, and most PEG parsers, CPython's among them, store results only for some rules. General context-free parsing with Earley or GLR is cubic in the worst case and close to linear for grammars of real languages. Enumerating every tree, as the example does, is exponential, because ambiguous grammars can have a number of trees that grows like the Catalan numbers, 14 for five numbers and 4862 for ten.

## History

Noam Chomsky defined context-free grammars, which he called phrase-structure grammars, in 1956 and 1957 to describe natural language. John Backus presented a notation for the syntax of IAL, the language that became Algol, at a conference in Paris in 1959, and Peter Naur used it, with changes, in the Algol 60 report of 1960.[^2] Donald Knuth proposed the name Backus-Naur form in 1964. Algol 60 was the first major language with a formal grammar. Its 1960 report allowed the dangling else, and the Revised Report of 1963 removed it with a rule that forbids an `if` statement directly after `then`.

Stephen C. Johnson wrote yacc at Bell Labs in the 1970s, which made LALR parser generators a standard tool, and Niklaus Wirth proposed EBNF in 1977 to end the variety of notations. ISO standardized a form of EBNF in 1996, and ABNF became an Internet standard with RFC 5234 in 2008.[^5] Terence Parr started the tool that became ANTLR around 1989. Bryan Ford defined parsing expression grammars in 2004,[^1] and Python moved to a PEG parser in version 3.9 in 2020.[^8]

## References
