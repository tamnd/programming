This area is about how a language turns text into a program and what the program means. It covers the rules that are the same idea in many languages: tokens, grammars, expressions, the order of evaluation, scope, and behaviour that the standard leaves open.

## What it holds

- Lexical structure: tokens, identifiers, literals, comments and whitespace rules.
- Grammars and notation: BNF, EBNF, PEG and the other notations that define the syntax of a language.
- Expressions and statements: the difference between an expression and a statement, operators, precedence and associativity.
- Evaluation order: the order in which a language evaluates operands and arguments, short-circuit operators and sequence points.
- Scope and binding: lexical and dynamic scope, name resolution, shadowing and the lifetime of a binding.
- Formal semantics for programmers: operational, denotational and axiomatic semantics at the level of a working programmer.
- Undefined and implementation-defined behaviour: undefined, unspecified and implementation-defined behaviour, and what a compiler may do with each.

## What it does not hold

- Parser construction, such as how to write a recursive descent parser. That is in [Language implementation](/language-implementation/).
- The syntax of one language in full. That is in the guide of the language, under [Languages](/languages/).
- Formal semantics as mathematics, such as the soundness proof of Hoare logic. This site links out for it.
- Closures, which use scope. They are in [Functions and abstraction](/functions-and-abstraction/).
