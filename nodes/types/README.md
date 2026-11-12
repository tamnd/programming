A type system gives each value and expression a type and rejects programs whose types do not fit. This area explains the main ideas of type systems as programmers use them, and how the tier 1 languages differ.

## What it holds

- **Type systems.** What a type is, what a type system does, and the main properties that people use to describe type systems.
- **Forms of types.** The forms that types take beyond the primitive types: composite types, function types, and the special types unit, bottom and top.
- **Static and dynamic checking.** When types are checked: before the run, during the run, or both; and how a program asks for the type of a value.
- **Type inference.** How a compiler finds types that the programmer did not write.
- **Polymorphism and generics.** The kinds of polymorphism, and generic code that works for many types through type parameters.
- **Subtyping and variance.** When one type can stand in for another, and how subtyping passes through generic types and function types.
- **Algebraic data types.** Types built by combining other types with "and" (products: records and tuples) and "or" (sums: variants and enumerations).
- **Nullability and optional values.** How languages say that a value may be missing: null references, option types and the operators that work with them.
- **Type classes and traits.** Ad hoc polymorphism with named sets of operations that types provide: interfaces, type classes, traits and protocols.
- **Gradual typing.** Type systems that mix static and dynamic checking in one program, and the practice of adding types to untyped code.
- **Dependent and refinement types.** Types that depend on values, and types that carry a predicate on their values. The sub-area is small by nature.
- **Type-level programming.** Types that take types as arguments, hide types, or compute types. CS2023 FPL-Types topic 9 and the candidates of the core list name these items.
- **Substructural and effect types.** Types that count how many times a value is used, and types that record what effects code can have.
- **Type theory for programmers.** The theory that stands behind type systems, at the level that a programmer needs to read papers and specifications. The proofs link to math2.
- **Erasure and reification.** What happens to types at run time: erased, kept, or wrapped.
- **Type-driven design.** The use of types to make wrong programs hard to write: wrapper types, phantom types, smart constructors and units of measure.

## What it does not hold

- Type theory as mathematics, such as the proofs of the lambda calculus. This site links out for it.
- Type checkers as tools, such as mypy or the TypeScript compiler as a product. They are in [Program analysis and verification](/program-analysis-and-verification/).
- The values of the primitive types and their limits. They are in [Data and values](/data-and-values/) and [Foundations](/foundations/).
- Interfaces and class hierarchies. They are in [Objects and classes](/objects-and-classes/).
