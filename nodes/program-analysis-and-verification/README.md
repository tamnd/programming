This area covers the methods and tools that find defects without running the program, or that prove a program correct. It goes from linters that every project uses to deductive verifiers and proof assistants.

## What it holds

- Static analysis: analysis of code without running it.
- Linters and type checkers: linters and type checkers as analyzers.
- Abstract interpretation: analysis by computing over abstract values, as used in real tools.
- Symbolic execution: running a program on symbolic inputs to find paths that fail.
- Model checking: checking every state of a model of a program.
- Deductive verification: proving a program against a specification, with tools such as Dafny and Frama-C.
- Proof assistants for programs: proof assistants used to verify programs.
- Design by contract: preconditions, postconditions and invariants as a specification.
- Runtime verification: checking a specification while the program runs.

## What it does not hold

- The logic and mathematics behind verification, such as the soundness of Hoare logic. This site links out for it.
- Assertions in everyday code. They are in [Errors and control flow](/errors-and-control/).
- Testing, which runs the program. It is in [Testing](/testing/).
- Type systems as language features. They are in [Types](/types/).
