This area is about finding defects by running the program. It covers the levels of testing, the techniques to design tests, the tools of each language and the problems of tests themselves, such as flaky tests.

## What it holds

- **Testing fundamentals.** What testing is and what it can and cannot show: the oracle problem, testability, the shape of a test suite and the main principles.
- **Levels of testing.** Tests by the size of what they check, from one unit to the whole system and its users.
- **Test design techniques.** How to choose test cases: the black-box and experience-based techniques, and the techniques that relate many inputs and outputs.
- **xUnit basics, strategy and organization.** The basic patterns of xUnit frameworks, the test strategy patterns and the test organization patterns of *xUnit Test Patterns*.
- **Fixtures and assertions.** How tests build and tear down their fixtures, share them, work with databases, choose values and verify results.
- **Test doubles.** Objects that stand in for real dependencies in tests, the design patterns that make doubles possible, and the two schools of their use.
- **Test smells.** Signs that tests are hard to read, slow, fragile or wrong: the 15 smells of *xUnit Test Patterns* with their main causes, and other test smells from later research.
- **Property-based testing.** Tests that state properties over many generated inputs and shrink the failing ones.
- **Fuzzing.** Testing with large numbers of generated or mutated inputs to find crashes and security bugs.
- **Mutation testing.** Testing the tests: change the code in small ways and check that the tests notice.
- **Snapshot and approval testing.** Tests that compare output with a stored, approved copy.
- **Test coverage.** Measures of how much of the code or the requirements tests exercise, and how to use them well.
- **TDD and BDD.** Test-driven development and behaviour-driven development: the cycle, the patterns of Beck's book and the BDD tools.
- **Test frameworks.** The test frameworks of the tier 1 languages and the main other ones, and the formats test runners use.
- **Flaky tests.** Tests that pass and fail without a code change: their causes, how to find them and how to fix or contain them.
- **Testing concurrent code.** How to test code that uses threads, async tasks or distributed nodes, where the schedule decides the result.
- **Non-functional testing.** Tests of how well a system works rather than what it does: load, stress, resilience, accessibility, compatibility and other qualities. Benchmarks are in performance/measuring.
- **UI and end-to-end test automation.** Automating tests through a browser or a mobile or desktop user interface.
- **Test infrastructure.** The machinery that runs large test suites fast and reliably: environments, test data, selection, parallel runs and reports.

## What it does not hold

- Proof that code is correct. It is in [Program analysis and verification](/program-analysis-and-verification/).
- Debugging a failure that a test found. It is in [Debugging and observability](/debugging-and-observability/).
- Benchmarks. They are in [Performance](/performance/).
- CI systems that run the tests. They are in [Tools and workflow](/tools-and-workflow/).
