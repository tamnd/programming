---
tag: 1Q1FY1
kind: practice
slug: make-illegal-states-unrepresentable
name: Make illegal states unrepresentable
aliases: [illegal states unrepresentable, Minsky's maxim]
area: types/algebraic-data-types
level: core
summary: >-
  Choose types so that a wrong combination of data cannot be built at all, for example a sum type in place of a state flag plus optional fields; a maxim of Yaron Minsky from 2010.
history:
  year: 2010
  people: [Yaron Minsky, Scott Wlaschin, Alexis King]
  note: Yaron Minsky used the maxim in his Effective ML talk at Harvard in 2010 and published the code of the talk in 2011; Scott Wlaschin and Alexis King later wrote about it.
see_also: [sum-types, algebraic-data-type, enumeration, record-type, union-type, pattern-matching, primitive-obsession, value-object]
sources:
  - url: https://blog.janestreet.com/effective-ml-revisited/
    title: Yaron Minsky, Effective ML Revisited, Jane Street Tech Blog, March 9, 2011
    locator: the introduction and the section Make illegal states unrepresentable
    accessed: 2026-11-16
  - url: https://fsharpforfunandprofit.com/posts/designing-with-types-making-illegal-states-unrepresentable/
    title: Scott Wlaschin, Designing with types, Making illegal states unrepresentable, F# for fun and profit
    locator: the whole post
    accessed: 2026-11-16
  - url: https://lexi-lambda.github.io/blog/2019/11/05/parse-don-t-validate/
    title: Alexis King, Parse, don't validate, November 5, 2019
    locator: the sections Turning partial functions total, The power of parsing and Parsing, not validating, in practice
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: TypeScript Handbook, Narrowing
    locator: the sections Discriminated unions, The never type and Exhaustiveness checking
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch06-01-defining-an-enum.html
    title: The Rust Programming Language, 6.1 Defining an Enum
    locator: the section Enum Values, Listing 6-1 and the IpAddr enum
    accessed: 2026-11-16
---

## Statement

"Make illegal states unrepresentable" is the title of one section of the code that Yaron Minsky published from his Effective ML talk.[^1] Alexis King gives the same advice in two short rules: "Use a data structure that makes illegal states unrepresentable. Model your data using the most precise data structure you reasonably can."[^3]

## Meaning

A program has rules about which combinations of data are correct. The practice is to put these rules into the types, so that a value that breaks a rule cannot be built. Then the compiler rejects the wrong combination, and the code that reads a value does not need a check for it.

The common case is a record with a state flag and some optional fields. Each optional field is valid only in some states, so most combinations of the flag and the fields are wrong. The fix is a [[sum-types|sum type]]: each state becomes a variant, and each variant carries only the fields that are valid in that state. Code then reads the fields with [[pattern-matching|pattern matching]], and in each branch the fields that it needs are present.

Scott Wlaschin shows the same fix for a business rule: "A contact must have an email or a postal address".[^2] A record with two required fields does not allow a contact with only one address. A record with two optional fields allows a contact with no address: "But now we have gone too far the other way."[^2] A sum type with the three cases EmailOnly, PostOnly and EmailAndPost allows exactly the legal values: "All three cases are explicitly represented, and the fourth possible case (with no email or postal address at all) is not allowed."[^2]

Alexis King connects the practice to parsing: "a parser is just a function that consumes less-structured input and produces more-structured output".[^3] A function that turns loose data into a precise type checks the rules once. After that, the precise type carries the proof that the rules hold.

## Examples

Minsky's example is the state of a network connection.[^1] The first type is a record with a state field (Connecting, Connected or Disconnected) and five optional fields: last_ping_time, last_ping_id, session_id, when_initiated and when_disconnected. The second type moves the fields into the variants. A connecting state has when_initiated. A connected state has session_id and an optional last_ping that holds a time and an id together. A disconnected state has when_disconnected.[^1]

The example below writes both types in Python. It builds every combination of the first type, with each optional field present or absent, and counts the combinations that the second type also allows. Then it builds one value of each form of the second type and checks that each one is legal.

```python run id=connection-state
from dataclasses import dataclass
from itertools import product

FIELDS = ("last_ping_time", "last_ping_id", "session_id",
          "when_initiated", "when_disconnected")
SAMPLE = {"last_ping_time": 12, "last_ping_id": 7, "session_id": "s1",
          "when_initiated": 10, "when_disconnected": 20}


# Before: a state flag and five optional fields.
@dataclass(frozen=True)
class FlagSet:
    state: str
    last_ping_time: int | None = None
    last_ping_id: int | None = None
    session_id: str | None = None
    when_initiated: int | None = None
    when_disconnected: int | None = None


# The fields that each state must have, and no others.
ALLOWED = {
    "connecting": [{"when_initiated"}],
    "connected": [{"session_id"},
                  {"session_id", "last_ping_time", "last_ping_id"}],
    "disconnected": [{"when_disconnected"}],
}


def legal(c: FlagSet) -> bool:
    present = {f for f in FIELDS if getattr(c, f) is not None}
    return present in ALLOWED[c.state]


# After: each state carries its own fields.
@dataclass(frozen=True)
class Connecting:
    when_initiated: int


@dataclass(frozen=True)
class Connected:
    session_id: str
    last_ping: tuple[int, int] | None  # (time, id)


@dataclass(frozen=True)
class Disconnected:
    when_disconnected: int


type ConnectionState = Connecting | Connected | Disconnected


def to_flags(s: ConnectionState) -> FlagSet:
    match s:
        case Connecting(t):
            return FlagSet("connecting", when_initiated=t)
        case Connected(sid, None):
            return FlagSet("connected", session_id=sid)
        case Connected(sid, (time, pid)):
            return FlagSet("connected", last_ping_time=time,
                           last_ping_id=pid, session_id=sid)
        case Disconnected(t):
            return FlagSet("disconnected", when_disconnected=t)


flag_sets = [
    FlagSet(state, **{f: SAMPLE[f] for f, on in zip(FIELDS, mask) if on})
    for state in ALLOWED
    for mask in product((False, True), repeat=len(FIELDS))
]
count = sum(legal(c) for c in flag_sets)
print(f"flag set: {len(flag_sets)} forms, {count} legal")

# One value of each form: the optional last_ping gives Connected two forms.
sums = [Connecting(10), Connected("s1", None), Connected("s1", (12, 7)),
        Disconnected(20)]
count = sum(legal(to_flags(s)) for s in sums)
print(f"sum type: {len(sums)} forms, {count} legal")

bad = FlagSet("connected")  # no session id, but the type accepts it
print(f"built: {bad.state} without a session id, "
      f"legal: {'yes' if legal(bad) else 'no'}")
```

```output
flag set: 96 forms, 4 legal
sum type: 4 forms, 4 legal
built: connected without a session id, legal: no
```

The first type has 3 states and 5 optional fields, so it has 3 × 2⁵ = 96 forms, and only 4 of them are legal. The second type has exactly the 4 legal forms. In Python, nothing stops a program from building the illegal value at the end; in Rust and TypeScript, the second type gives no way to write a connected state without a session id. Other sources show the same change in other languages:

| Language | Loose type | Precise type |
|---|---|---|
| OCaml | A record with a state field and the option fields last_ping_time, session_id and others.[^1] | Variants that carry records: `Connecting of connecting`, `Connected of connected`, `Disconnected of disconnected`.[^1] |
| F# | A Contact with two optional address fields.[^2] | A union ContactInfo with the cases EmailOnly, PostOnly and EmailAndPost.[^2] |
| Haskell | A function head that takes a list, which can be empty.[^3] | The type `NonEmpty a`, defined as `data NonEmpty a = a :\| [a]`, which always has a first element.[^3] |
| TypeScript | A Shape interface with a kind field and the optional fields radius and sideLength.[^4] | A union of a Circle and a Square type, each with its own kind and its own required field. "When every type in a union contains a common property with literal types, TypeScript considers that to be a discriminated union, and can narrow out the members of the union."[^4] |
| Rust | A struct IpAddr with a kind field of type IpAddrKind and an address field of type String.[^5] | An enum IpAddr whose variants carry the data: `V4(u8, u8, u8, u8)` and `V6(String)`. "Each variant can have different types and amounts of associated data."[^5] |

The Rust and TypeScript examples of this page build the same values as the Python example.

## Criticism and limits

- **Some rules do not fit the type system.** Alexis King writes: "Sometimes, making an illegal state truly unrepresentable is just plain impractical given the tools Haskell provides, such as ensuring an integer is in a particular range." For that case, King suggests "an abstract newtype with a smart constructor": a [[smart-constructor|smart constructor]] checks the rule at run time, and the type hides the raw constructor.[^3]
- **More types to write.** Wlaschin answers the objection that the sum type makes the code more complex: "If your code is not this complicated, you are not handling all the cases properly."[^2] The cost is real but moves the complexity of the rules into one visible place.
- **A change to the rules breaks the code.** When the rules change, the type changes, and each function that matches on the type must change too. Wlaschin says that such breaking changes are generally a good thing.[^2] In TypeScript, an exhaustive switch makes the compiler report each place to fix: adding a new member to the union causes an error in a switch that assigns the remaining value to never.[^4]
- **Data from outside is still loose.** Input from files, networks and users arrives in a loose form. King's advice is to "Push the burden of proof upward as far as possible, but no further" and to parse the data early: "Ideally, this should happen at the boundary of your system, before any of the data is acted upon".[^3]
- **An ideal, not a rule.** King writes that "it can be genuinely difficult to capture certain invariants in the type system" and adds: "Consider the principles in this blog post ideals to strive for, not strict requirements to meet."[^3]

## History

In March 2011, Yaron Minsky of Jane Street wrote about a guest lecture on OCaml at Harvard: "I gave a version of the Effective ML talk that I gave last year to the same class." The post gives the code of the talk, and one section has the title "Make illegal states unrepresentable".[^1] Scott Wlaschin used the maxim in the F# series Designing with types and called it "a phrase borrowed from Yaron Minsky".[^2] In November 2019, Alexis King's post Parse, don't validate listed it as the first of two simple ideas that a Haskell example shows.[^3]

## References
