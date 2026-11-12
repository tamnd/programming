This area covers the methods and tools that find defects without running the program, or that prove a program correct. It goes from linters that every project uses to deductive verifiers and proof assistants.

## What it holds

- **Correctness concepts.** The ideas that every analysis and proof uses: what it means for a program to be correct, which properties a tool can check, and why every useful analysis must approximate.
- **Static analysis.** The methods that static analyzers use: program representations, data-flow analysis, pointer analysis and the main precision choices.
- **Static analysis tools.** The general bug-finding and quality analyzers that work across many rules, with their query languages and report formats.
- **Linters and type checkers.** Linters and separate type checkers seen as analyzers: what they check, how to configure them, and the main tool for each tier 1 language.
- **Abstract interpretation.** The theory and tools of sound static analysis by computing over abstract values instead of concrete ones.
- **Symbolic execution.** Running a program on symbols, collecting path conditions and solving them to find inputs and bugs.
- **Model checking.** Exhaustive checking of a model or a program against a property, with counterexamples when the property fails.
- **Deductive verification.** Proving code correct against contracts with verification conditions sent to automatic provers.
- **Proof assistants.** Interactive theorem provers used to write and check proofs about programs and to extract certified code.
- **Contracts and specifications.** Contracts as a practice: preconditions, postconditions and invariants, and the languages for formal specifications.
- **Runtime verification.** Checking properties on real executions with monitors, instrumentation and dynamic analysis.
- **Analysis in practice.** How teams adopt analyzers and verification tools, and how they keep the results useful.

## What it does not hold

- The logic and mathematics behind verification, such as the soundness of Hoare logic. This site links out for it.
- Assertions in everyday code. They are in [Errors and control flow](/errors-and-control/).
- Testing, which runs the program. It is in [Testing](/testing/).
- Type systems as language features. They are in [Types](/types/).
