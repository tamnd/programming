---
tag: 66NR5A
kind: concept
slug: row-polymorphism
name: Row polymorphism
aliases: [row types, row type, row variable, extensible records, open records, open variants]
area: types/algebraic-data-types
level: advanced
summary: >-
  Row polymorphism lets a function accept any record that has some named fields, and return the record with all its other fields kept. OCaml, PureScript and Elm use it.
applies_to: core-languages
viz: optional
formal: optional
history:
  year: 1987
  people: [Mitchell Wand]
  note: Wand defined row types to model complete type inference for multiple inheritance of objects.
see_also: [record-type, algebraic-data-type, union-type, subtyping, type, product-type]
same_as:
  wikidata: Q60794446
  wikipedia: Row_polymorphism
sources:
  - url: https://en.wikipedia.org/wiki/Row_polymorphism
    title: Row polymorphism, Wikipedia
    locator: the introduction; History; Theory; Row-polymorphic record type definition; Implementations
    accessed: 2026-11-16
  - url: https://raw.githubusercontent.com/purescript/documentation/master/language/Types.md
    title: PureScript documentation, Types
    locator: Row Polymorphism; Rows
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html
    title: TypeScript Handbook, Type Compatibility
    locator: Starting out
    accessed: 2026-11-16
  - url: https://ocaml.org/manual/5.5/polyvariant.html
    title: The OCaml Manual, Chapter 5 Polymorphic variants
    locator: the introduction; 5.1 Basic use
    accessed: 2026-11-16
  - url: https://ocaml.org/manual/5.5/objectexamples.html
    title: The OCaml Manual, Chapter 3 Objects in OCaml
    locator: 3.4 Immediate objects and object types (get_succ_x)
    accessed: 2026-11-16
  - url: https://raw.githubusercontent.com/evancz/guide.elm-lang.org/master/book/core_language.md
    title: An Introduction to Elm, Core Language
    locator: Records
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/objects.html
    title: TypeScript Handbook, Object Types
    locator: Excess Property Checks
    accessed: 2026-11-16
---

## Idea

Many functions need only a few fields of a record. A function that moves a point needs `x` and `y`. It does not care whether the point also has a `z`, a `label` or a `color`. Row polymorphism gives such a function a type that says exactly this: the input has `x` and `y` and any other fields, and the output has the same other fields. Wikipedia describes it as a kind of polymorphism for programs that are "polymorphic on record types and/or variants".[^1]

```python run id=idea
def shift(p: dict) -> dict:
    return {**p, "x": p["x"] + 1}


print(shift({"x": 1, "y": 2}))
print(shift({"x": 1, "y": 2, "label": "home"}))
print(shift({"x": 1, "y": 2, "z": 3, "color": "red"}))
```

```output for=idea
{'x': 2, 'y': 2}
{'x': 2, 'y': 2, 'label': 'home'}
{'x': 2, 'y': 2, 'z': 3, 'color': 'red'}
```

Python does not check the types of dictionaries, so this code works for any dictionary with an `x`. A language with row polymorphism checks the same code statically. In PureScript, a function that reads two fields gets the type `forall r. { foo :: Int, bar :: Int | r } -> Int`. The documentation explains that the function accepts any record that has the properties `foo` and `bar`, and any other properties.[^2] The variable `r` stands for the other fields.

## How it works

**Rows.** "Formally, a row is an association of labels to types."[^1] A record type is built from a row, such as `{ x : Int, y : Int }`. A row can be closed, with exactly the listed fields, or open, with a row variable that stands for more fields. PureScript writes an open row with a pipe: `( name :: String, age :: Number | r )`.[^2]

**Row variables.** A function type can mention a row variable, and each call fills it in with the extra fields of the argument. Wikipedia gives the type `transform2d : {x:Number, y:Number, ρ} → {x:Number, y:Number, ρ}`.[^1] The same `ρ` is on both sides, so "There is no loss of information: the type ensures that all the fields represented by the variable ρ are present in the return type."[^1]

**Record operations.** Selecting a field, adding a field and removing a field all get row-polymorphic types. Some systems also record which fields are absent, so that adding a field cannot make a duplicate label.[^1]

**Rows and subtyping.** Structural subtyping also lets a function accept a record with extra fields. TypeScript states the rule this way: "x is compatible with y if y has at least the same members as x".[^3] But with subtyping alone, the return type of the function names only the fields in its signature, and the type checker forgets the other fields. A row variable keeps them in the type. TypeScript code gets the same effect with a generic type parameter constrained by the needed fields, as the TypeScript example shows.

**Variants.** Rows also describe open variants, that is, open sum types. OCaml polymorphic variants are an example: "a variant tag does not belong to any type in particular, the type system will just check that it is an admissible value according to its use".[^4] A tag is a name with a backquote, such as `` `On ``. An inferred type that starts with `[<` lists the tags that a function can accept, and a type that starts with `[>` lists the tags that a value can have. The manual says: "The > and < inside the variant types show that they may still be refined, either by defining more tags or by allowing less."[^4]

## In languages

The example files define a function that adds 1 to the `x` field of any record with `x` and `y` fields, and return a value of the same type with the other fields kept.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a generic function bound by a `Protocol`, with `dataclasses.replace`; type checkers check it, the run time does not |
| JavaScript | idiom | object spread; no static types |
| TypeScript | idiom | a type parameter with a constraint, `<P extends { x: number; y: number }>`; structural types[^3] |
| Java | idiom | a generic interface whose methods return the type parameter |
| C | n/a | each function takes one struct type |
| C++ | idiom | a template, checked when it is used |
| C# | idiom | a generic method constrained to a base record, and a `with` expression |
| Go | idiom | a generic function with a constraint interface; type parameters cannot access struct fields |
| Rust | idiom | a generic function bound by a trait |
| Kotlin | idiom | a generic interface, as in Java |
| PowerShell | idiom | objects with extra properties; no static types |
| Bash | n/a | Bash has no records |
| SQL | n/a | a query names its columns |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup has no types |

None of these languages has row variables. The examples show the closest idiom in each.

**OCaml.** "In Ocaml, row polymorphism is used by Ocaml's object types and also by its polymorphic variants. Ordinary record types are not row-polymorphic in Ocaml."[^1] The manual shows the function `let get_succ_x p = p#get_x + 1` with the type `< get_x : int; .. > -> int`. It applies "to any object p that has this method (and possibly some others, which are represented by an ellipsis in the type)".[^5]

**PureScript.** Records have row types, and a function that uses some fields accepts any record with those fields.[^2] The documentation notes that `addProps { foo: 1 }` does not compile, because the `bar` property is missing.[^2] PureScript is listed among the languages that support row polymorphism.[^1]

**Elm.** An Elm record update keeps the other fields of the record. For the function `celebrateBirthday person = { person | age = person.age + 1 }`, the Elm guide shows the type `{ a | age : number } -> { a | age : number }`.[^6] The variable `a` stands for the other fields.

**Standard ML and F#.** "Row polymorphism is not supported in Standard ML", but extensions such as SML# support it.[^1] Wikipedia notes that F# can simulate row polymorphism with "Statically Resolved Type Parameters" in inline functions.[^1]

## Variations and related ideas

- **Structural subtyping.** A record with more fields is a subtype of a record with fewer. It accepts the same arguments, but it forgets the extra fields in the result. See [[subtyping]].
- **Extensible records.** Records that can gain or lose fields with typed operations.[^1]
- **Open variants.** Sum types to which other code can add cases, such as OCaml polymorphic variants.[^4] See [[sum-types]].
- **Effect rows.** "In the modern day, row types are most commonly used to type extensible algebraic effects."[^1] Wikipedia names Links and Koka as languages that use rows for effect types.[^1]
- **Duck typing.** Dynamic languages accept any object with the needed fields, but check nothing before the program runs. See [[duck-typing]].

## Pitfalls

### Subtyping forgets the extra fields

A function that takes and returns a fixed record type accepts a larger record but returns the smaller type. In TypeScript the extra fields are still in the object at run time, but the type checker does not let the program use them. A generic type parameter keeps the full type. The TypeScript example shows both forms.

### A literal with extra fields is an error in TypeScript

TypeScript applies excess property checking to object literals: "Object literals get special treatment and undergo excess property checking when assigning them to other variables, or passing them as arguments."[^7] A literal with a property that the target type does not have is an error. So a call such as `shift({ x: 1, y: 2, label: "home" })` to a function that takes `{ x: number; y: number }` fails, but the same object passed through a variable is accepted, because a variable does not get excess property checks.[^7] A generic function with a constraint accepts both forms.

### Duplicate labels

A row system must say what happens when a record gains a field that it already has. Wikipedia notes that the theory "is somewhat complicated by the need to have distinct labels in a record".[^1] Some systems forbid the duplicate, and PureScript keeps both: duplicate labels "have their types collected together in order".[^2]

## Cost

Row polymorphism is a property of the type system. In a language that compiles a record to a fixed layout, a row-polymorphic function cannot know the offset of a field at compile time, so an implementation passes the offset, looks up the field by name, or compiles a separate copy of the function for each record type. Dynamic languages look up fields by name. So does PureScript: "PureScript records correspond to JavaScript objects."[^2]

## History

"Row types and row polymorphism were introduced by Mitchell Wand in the late 1980s as an approach to structurally typing extensible records and variants."[^1] Wikipedia cites Wand's 1987 paper on complete type inference for simple objects. Rémy found that the original inference algorithm was not complete, and both Rémy and Wand corrected it.[^1] The first version of SML# added row polymorphism to Standard ML, based on a 1989 paper by Ohori and colleagues.[^1] OCaml uses rows for its object types and polymorphic variants.[^1][^5][^4]

## References
