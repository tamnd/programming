---
tag: BGXM53
kind: technique
slug: smell-detection-tools
name: Detecting smells with tools
aliases: [smell detection, linting for smells, static analysis for smells]
area: construction-practice/code-smells
level: advanced
summary: >-
  Run linters and static analysers with rules for size, complexity, parameters and unused code, so that the signs of a smell are reported on each change and a person decides what to refactor.
see_also: [code-smell, design-smell, long-function, long-parameter-list, large-class, dead-code-smell]
sources:
  - url: https://martinfowler.com/bliki/CodeSmell.html
    title: Martin Fowler, Code Smell, bliki, 9 February 2006
    locator: the definition and the two points that follow it
  - url: https://en.wikipedia.org/w/index.php?title=Static_program_analysis&oldid=1379053559
    title: Wikipedia, Static program analysis, revision 1379053559
    locator: the introduction and Formal methods
  - url: https://eslint.org/docs/latest/rules/max-params
    title: ESLint, max-params
    locator: the rule summary, Rule Details and Options
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/refactor/too-many-arguments.html
    title: Pylint, too-many-arguments / R0913
    locator: the message and its description
  - url: https://pmd.github.io/pmd/pmd_rules_java_design.html
    title: PMD, Java rules, Design
    locator: GodClass, ExcessiveParameterList and CyclomaticComplexity
  - url: https://staticcheck.dev/docs/checks/
    title: Staticcheck, Checks
    locator: SA4006
---

## Purpose

A [[code-smell|code smell]] is by definition quick to spot. Fowler writes that "a smell is by definition something that's quick to spot".[^1] Much of what is quick for a person to spot is also quick for a program to count: the lines of a function, the parameters of a method, the branches of a condition, the variables that are assigned and never read.

Smell detection with tools puts those counts into the build. A linter or a static analyser reads the source without running it; Wikipedia defines static program analysis as "the analysis of computer programs performed without executing them".[^2] Each rule reports a sign, the report comes with each change, and a person decides whether the sign is a smell and what to do about it.

## Steps

1. **Choose the rules that match the smells.** Most linters have a rule for each sign that can be counted:
   - [[long-parameter-list|Long Parameter List]]: ESLint `max-params`, Pylint `too-many-arguments` (R0913), PMD `ExcessiveParameterList`.[^3][^4][^5]
   - [[long-function|Long Function]] and complex conditionals: ESLint `max-lines-per-function` and `complexity`, PMD `CyclomaticComplexity`.[^5]
   - [[large-class|Large Class]]: PMD `GodClass`.[^5]
   - [[dead-code-smell|Dead code]]: ESLint `no-unreachable`, Staticcheck SA4006.[^6]
2. **Set the thresholds for the codebase.** The defaults are guesses. ESLint's `max-params` has the option `"max" (default 3)`.[^3] Start at a level that the current code mostly meets, so that the report lists the worst places, not every place.
3. **Run the tools on each change, in the editor and in CI.** A warning on the lines that a change touches is cheap to act on; a list of three thousand old warnings is not.
4. **Ratchet.** Do not fail the build on old warnings. Fail it when a change adds a warning, or when the count of warnings in a file goes up. Lower the thresholds as the code improves.
5. **Read each warning as a question.** The tool says that a function has seven parameters. The person decides whether they are a [[data-clumps|data clump]] that wants a parameter object, or a list that the domain needs.
6. **Suppress with a reason.** When a warning is wrong for a place, suppress it at that place, with a comment that says why, so that the next reader does not raise it again.

## Example

A small detector in the spirit of ESLint's `max-params`: it inspects the functions of a module and reports those with more parameters than a threshold, or more lines than a limit. Like the real tools, it reports signs, not smells.

```javascript run id=count-the-signs
import assert from "node:assert/strict";

// The module under review.
const mod = {
  totalPrice(items) {
    return items.reduce((sum, i) => sum + i.price * i.qty, 0);
  },
  createUser(name, email, street, city, zip, country) {
    return { name, email, address: { street, city, zip, country } };
  },
  report(rows) {
    const lines = [];
    for (const r of rows) {
      if (r.qty > 0) {
        const total = r.price * r.qty;
        lines.push(`${r.name}: ${total}`);
      }
    }
    lines.sort();
    return lines.join("\n");
  },
};

// The detector: a function's length property counts its declared parameters,
// and its source text gives its length in lines.
function detect(module, { maxParams = 3, maxLines = 8 } = {}) {
  const warnings = [];
  for (const [name, fn] of Object.entries(module)) {
    if (fn.length > maxParams) warnings.push(`${name}: ${fn.length} parameters (max ${maxParams})`);
    const lines = fn.toString().split("\n").length;
    if (lines > maxLines) warnings.push(`${name}: ${lines} lines (max ${maxLines})`);
  }
  return warnings;
}

assert.deepEqual(detect(mod), ["createUser: 6 parameters (max 3)", "report: 11 lines (max 8)"]);
assert.deepEqual(detect(mod, { maxParams: 6, maxLines: 12 }), []);
console.log(detect(mod).join("\n"));
```

```output
createUser: 6 parameters (max 3)
report: 11 lines (max 8)
```

The detector finds two signs. `createUser` has six parameters, four of which are an address, so it is a data clump, and a parameter object fixes it. `report` has eleven lines, but they read in order and do one job; a person may decide that it is fine. The tool cannot tell these two cases apart, which is why the next step is a person.

## When to use

- **On each change, in a codebase that more than one person works on.** The tools hold a level that the team agreed on, without a reviewer having to count.
- **When taking over a large codebase.** A first run gives a map of the places with the most signs, which are often the places with the most bugs and the most changes.
- **Before a refactoring.** The counts give a before and after, and show whether the change reduced what it set out to reduce.

Tools are a poor fit for the smells that are about meaning: [[mysterious-name|Mysterious Name]], [[divergent-change|Divergent Change]], [[feature-envy|Feature Envy]] in most forms, and the design smells of abstraction and hierarchy. These need a reader.

## Pitfalls

**Treating a warning as a defect.** A warning is a sign. Code that is refactored only to meet a number can become worse: a long function split into pieces with meaningless names, or parameters hidden in an options bag with no type.

**Thresholds as targets.** When the count is the goal, people meet the count. Keep the thresholds as a floor that catches the worst cases.

**False positives and false negatives.** No tool can be exact. Wikipedia notes that finding all run-time errors in an arbitrary program is undecidable: "there is no mechanical method that can always answer truthfully whether an arbitrary program may or may not exhibit runtime errors."[^2] Smell rules are approximations, and some warnings are wrong while some smells pass.

**A wall of old warnings.** A tool that is added to a large codebase and reports everything is ignored at once. Start with a baseline, and report only what a change adds.

## Tools that do it

| Language | Tool | Rules for smells |
|---|---|---|
| JavaScript, TypeScript | ESLint | `max-params`, `max-lines-per-function`, `max-depth`, `complexity`, `no-unreachable`, `no-unused-vars`[^3] |
| Python | Pylint | `too-many-arguments` (R0913), "Used when a function or method takes too many arguments", and the other R09 design checks[^4] |
| Java, Apex and others | PMD | `GodClass`, `ExcessiveParameterList`, `CyclomaticComplexity`, `DataClass`, `CouplingBetweenObjects`[^5] |
| Go | Staticcheck, `go vet` | unused code, and SA4006, "A value assigned to a variable is never read before being overwritten."[^6] |

PMD's `GodClass` rule shows how far a rule can go: it "detects the God Class design flaw using metrics", and the rule "uses the detection strategy described in "Object-Oriented Metrics in Practice"."[^5] It combines several measures, not one count, which makes it closer to how a person judges a large class.

## References
