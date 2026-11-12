This area is about how a language turns text into a program and what the program means. It covers the rules that are the same idea in many languages: tokens, grammars, expressions, the order of evaluation, scope, and behaviour that the standard leaves open.

## What it holds

- **Lexical structure.** The smallest parts of source text: characters become tokens, and tokens are keywords, identifiers, literals, operators and comments.
- **Grammars and notation.** How the rules of syntax are written: grammars, BNF and its relatives, the Chomsky hierarchy, parse trees and the notation of operators.
- **Expressions and statements.** The parts of a program that compute values and the parts that do actions, the operators that join values, and the blocks that group statements.
- **Evaluation order.** When and in what order the parts of an expression are evaluated, and what the strategies of evaluation are.
- **Scope and binding.** How names are declared, bound to things and found again, and where a name is visible.
- **Formal semantics.** The exact ways to state what a program means, and the reasoning that they allow.
- **Language design.** The criteria that language designers use, the main syntax choices, and how languages change over time. CS2023 FPL-Pragmatics and FPL-Design cover these topics, and no other area holds them.
- **Undefined behaviour.** The behaviour that a language standard does not define, and how it differs from unspecified and implementation-defined behaviour.

## What it does not hold

- Parser construction, such as how to write a recursive descent parser. That is in [Language implementation](/language-implementation/).
- The syntax of one language in full. That is in the guide of the language, under [Languages](/languages/).
- Formal semantics as mathematics, such as the soundness proof of Hoare logic. This site links out for it.
- Closures, which use scope. They are in [Functions and abstraction](/functions-and-abstraction/).
