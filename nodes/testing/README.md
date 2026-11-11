This area is about finding defects by running the program. It covers the levels of testing, the techniques to design tests, the tools of each language and the problems of tests themselves, such as flaky tests.

## What it holds

- Unit, integration and end-to-end testing: unit, integration, system and end-to-end tests.
- Test doubles: stubs, mocks, fakes and spies.
- Test design techniques: techniques to choose test inputs, such as equivalence classes and boundary values.
- Property-based testing: tests that check a property on many generated inputs.
- Fuzzing: finding crashes with random and mutated inputs.
- Mutation testing: measuring the quality of tests by changing the code.
- Snapshot and golden testing: comparing output with a stored golden copy.
- Coverage: line, branch and other coverage measures, and their limits.
- TDD and BDD: test-driven development and behaviour-driven development.
- Test frameworks: the test frameworks of each language.
- Flaky tests: tests that pass and fail with no change, and how to fix them.
- Testing concurrency: how to test code that runs in more than one thread.

## What it does not hold

- Proof that code is correct. It is in [Program analysis and verification](/program-analysis-and-verification/).
- Debugging a failure that a test found. It is in [Debugging and observability](/debugging-and-observability/).
- Benchmarks. They are in [Performance](/performance/).
- CI systems that run the tests. They are in [Tools and workflow](/tools-and-workflow/).
