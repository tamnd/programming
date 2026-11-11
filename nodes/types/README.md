A type system gives each value and expression a type and rejects programs whose types do not fit. This area explains the main ideas of type systems as programmers use them, and how the tier 1 languages differ.

## What it holds

- Type systems: what a type system is, what it guarantees, and the terms strong, weak, sound and unsound.
- Static and dynamic checking: type checks before the run and during the run, and what each finds.
- Type inference: how a compiler finds types that the programmer did not write.
- Generics and parametric polymorphism: code that works for many types with one definition.
- Subtyping and variance: when a value of one type can stand for another, and how generic types vary.
- Algebraic data types: sum and product types, and pattern matching on them.
- Gradual typing: languages that mix checked and unchecked code, such as TypeScript and Python with type hints.
- Dependent and refinement types: types that depend on values, as used in practice.
- Nullability: null references, option types and null safety.
- Type classes and traits: ad hoc polymorphism with type classes, traits and protocols.
- Type erasure and reification: whether the type arguments of a generic exist at run time.
- Units of measure: types that carry a physical unit, so that a program cannot add metres to seconds.

## What it does not hold

- Type theory as mathematics, such as the proofs of the lambda calculus. This site links out for it.
- Type checkers as tools, such as mypy or the TypeScript compiler as a product. They are in [Program analysis and verification](/program-analysis-and-verification/).
- The values of the primitive types and their limits. They are in [Data and values](/data-and-values/) and [Foundations](/foundations/).
- Interfaces and class hierarchies. They are in [Objects and classes](/objects-and-classes/).
