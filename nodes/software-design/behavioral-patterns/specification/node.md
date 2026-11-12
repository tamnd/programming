---
tag: KM67J6
kind: pattern
slug: specification
name: Specification
aliases: [specification pattern, Specification pattern, composite specification]
area: software-design/behavioral-patterns
level: core
summary: >-
  A specification is an object that tells if a candidate object matches some criteria. Specifications combine with and, or and not, and the same object can select, validate and describe.
family: behavioral-design
polarity: positive
intent: "Separate the statement of how to match a candidate from the candidate object that it is matched against, in an object with the method isSatisfiedBy."
also_known_as: []
history:
  people: [Eric Evans, Martin Fowler]
  note: Evans used a model that he called specification for the requirements of a shipping route and for the storage of cargoes. Fowler had seen similar problems in trading and healthcare systems, and the two described the patterns together in the paper Specifications.
see_also: [strategy-pattern, interpreter-pattern, composite-pattern, null-object-pattern]
sources:
  - url: https://martinfowler.com/apsupp/spec.pdf
    title: Eric Evans and Martin Fowler, Specifications
    locator: Introduction; pattern summary table; The Need for a Specification; Specification; Consequences of Specification; When Not to Use Specification; Hard Coded, Parameterized and Composite Specification; Partially Satisfied Specification; Final Thoughts
  - url: https://en.wikipedia.org/wiki/Specification_pattern
    title: Specification pattern, Wikipedia
    locator: Introduction; Example of use
  - url: https://docs.python.org/3/reference/expressions.html#boolean-operations
    title: The Python Language Reference, 6.11 Boolean operations
    locator: 6.11 Boolean operations
  - book: isbn:9780321125217
    title: Eric Evans, Domain-Driven Design, Tackling Complexity in the Heart of Software, Addison-Wesley, 2003
    locator: Chapter 9, Specification
  - url: https://github.com/spring-projects/spring-data-jpa/blob/3.4.0/spring-data-jpa/src/main/java/org/springframework/data/jpa/domain/Specification.java#L31-L105
    title: Spring Data JPA 3.4.0 source, org/springframework/data/jpa/domain/Specification.java
    locator: interface Specification, lines 31 to 105
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Predicate.html
    title: Java SE 25 API, Interface Predicate
    locator: Interface description; Method Summary
---

## Intent

Specification separates the statement of how to match a candidate from the candidate object that it is matched against. As well as its use in selection, it is also valuable for validation and for building to order.[^1]

## Problem

Evans and Fowler give three problems:[^1]

- **Selection.** You need to select a subset of objects based on some criteria, and to refresh the selection at various times. A trader monitors a portfolio of all contracts that involve the yen.
- **Validation.** You need to check that only suitable objects are used for a certain purpose. A container for meat must be refrigerated and clean enough to store food.
- **Construction to order.** You need to describe what an object might do, without the details of how it does it, so that a candidate can be built to fulfill the requirement. A shipping company builds a route from the constraints of a request.

If each of these places has its own copy of the test, the copies can drift apart. Here the selection of containers checks two conditions, but the validation checks only one:

```python wrong id=two-copies
class Container:
    def __init__(self, name, refrigerated, clean):
        self.name = name
        self.refrigerated = refrigerated
        self.clean = clean


containers = [Container("c1", True, True), Container("c2", True, False)]

# selection: which containers can hold meat?
print("can hold meat:", [c.name for c in containers if c.refrigerated and c.clean])


# validation: may this container hold meat?
def assign_meat(container):
    if container.refrigerated:  # the clean test is missing here
        print(f"meat assigned to {container.name}")


assign_meat(containers[1])
```

```output for=two-copies
can hold meat: ['c1']
meat assigned to c2
```

## Solution

Create a specification that can tell if a candidate object matches some criteria. The specification has a method `isSatisfiedBy(anObject)` that returns true if all criteria are met by `anObject`.[^1] Selection, validation and construction then use the same object.

A composite specification is a particular form of interpreter. Leaf elements are the different kinds of tests, and composite nodes are the and, or and not operators.[^1]

```text
               Specification
               isSatisfiedBy(candidate): bool
               and(other), or(other), not()
                     ^
        _____________|______________________
       |             |          |           |
   Refrigerated    Clean    AndSpec(a, b)  NotSpec(a)
   (leaf)          (leaf)   OrSpec(a, b)   (composite)
                            (composite)
```

A business rule can then be made from other rules. In the example of Wikipedia, an invoice goes to a collection agency if it is overdue, notices have been sent, and it is not already with the agency: `overdue.And(noticeSent).And(inCollection.Not())`.[^2]

## Example

The example files show one program in each language. Two leaf specifications test a container: `Refrigerated` and `Clean`. The program combines them with and, or and not, and selects from three containers. `c1` is refrigerated and clean, `c2` is refrigerated only, and `c3` is clean only. The program prints:

```text
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

## Consequences

The benefits:[^1]

- The design of requirements, fulfillment and validation is decoupled. Different parts of the system use the same constructs to express constraints, so similar code is not written many times.
- The system definitions are more clear and declarative. A `RouteSelector` finds a `Route` that satisfies a `RouteSpecification`. This defines its responsibility, even if no `isSatisfiedBy` test is done.

The costs depend on the implementation:[^1]

- A **hard-coded** specification is easy and expressive, but inflexible. Each new specification needs a new class, and you usually cannot create new specifications at run time.
- A **composite** specification is very flexible without many special classes, and it supports logical operations. But you must invest in a complex framework, and you are still limited by the leaf elements that are provided.
- If you select from many candidates in a database, performance can become an issue. You must find a way to index the candidates.

This Python version overloads the operator `&`. The keyword `and` is a different operator. The expression `x and y` returns `x` if `x` is false, and otherwise returns `y`.[^3] An object is true by default, so `and` returns the second specification only:[^3]

```python wrong id=keyword-and
class Spec:
    def __init__(self, name, test):
        self.name = name
        self.test = test

    def __and__(self, other):
        return Spec(f"{self.name} & {other.name}", lambda c: self.test(c) and other.test(c))


refrigerated = Spec("refrigerated", lambda c: c["refrigerated"])
clean = Spec("clean", lambda c: c["clean"])

print((refrigerated & clean).name)
print((refrigerated and clean).name)  # meant to be both tests
```

```output for=keyword-and
refrigerated & clean
clean
```

## When not to use it

The most common over-use is to build objects with flexible attributes from specifications. A specification has a clear responsibility: to identify an object that satisfies it, and to combine with other specifications. A route is not the same thing as a route specification. Evans and Fowler say that you should reconsider the pattern in these cases:[^1]

- You add to the protocol of the specification other than for initialization.
- You do not use `isSatisfiedBy`.
- The object represents an actual entity in the domain, not constraints on some other, possibly hypothetical, entity.

## Variants and related patterns

- **Hard Coded Specification** codes the criteria in the `isSatisfiedBy` method. It treats the specification as a Strategy.[^1]
- **Parameterized Specification** has attributes for the values that commonly vary, so new specifications can be built at run time without programming.[^1]
- **Composite Specification** uses an Interpreter, with composite nodes for and, or and not.[^1]
- **Subsumption** compares two specifications. The operation `isGeneralizationOf` tells whether the receiver is in every way equal to or more general than the argument.[^1]
- **Partially Satisfied Specification** adds a method `remainderUnsatisfiedBy`. It returns a specification of only the requirements that the candidate does not meet, so it can explain to the user why the specification was not satisfied.[^1]

This run block gives the unmet leaf tests of an and specification:

```python run id=remainder
class Leaf:
    def __init__(self, name):
        self.name = name

    def is_satisfied_by(self, c):
        return c[self.name]

    def remainder_unsatisfied_by(self, c):
        return [] if self.is_satisfied_by(c) else [self.name]


class And:
    def __init__(self, *parts):
        self.parts = parts

    def is_satisfied_by(self, c):
        return all(p.is_satisfied_by(c) for p in self.parts)

    def remainder_unsatisfied_by(self, c):
        return [name for p in self.parts for name in p.remainder_unsatisfied_by(c)]


meat = And(Leaf("refrigerated"), Leaf("clean"))
c2 = {"refrigerated": True, "clean": False}
print("satisfied:", meat.is_satisfied_by(c2))
print("missing:", meat.remainder_unsatisfied_by(c2))
```

```output for=remainder
satisfied: False
missing: ['clean']
```

The pattern is frequently used in the context of domain-driven design.[^2] Evans describes it in *Domain-Driven Design*.[^4]

## Known uses

- In Spring Data JPA, the interface `Specification<T>` is a "Specification in the sense of Domain Driven Design". It has `and`, `or` and `not`, and its method `toPredicate` creates a WHERE clause for a query of the entity.[^5]
- Java's `java.util.function.Predicate` represents a boolean-valued function of one argument. Its default methods `and` and `or` return composed predicates, and `negate` returns the logical negation.[^6]
- Much of the experience of Evans and Fowler with the pattern was in a Smalltalk/Gemstone environment.[^1]

## References
