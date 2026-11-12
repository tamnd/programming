---
tag: 9364CR
kind: concept
slug: identifier
name: Identifier
aliases: [identifier, identifiers, name, names, symbol, variable name, keyword, reserved word, contextual keyword, soft keyword, raw identifier, verbatim identifier, exported identifier, blank identifier, name mangling]
area: syntax-and-semantics/lexical-structure
level: core
summary: >-
  An identifier is a name that a program gives to a variable, function, type or other entity. The lexer reads it as one token, and the rules of the language decide which characters it can hold.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [Martin von Löwis]
  note: The 1956 FORTRAN manual allowed variable names of 1 to 6 letters and digits, and the first letter gave the type. Later languages allowed longer names and letters of other scripts. PEP 3131 added non-ASCII names to Python 3.0 in 2007.
see_also: [tokens-and-lexing, comment, lexical-scope, syntactic-macros, runtime-reflection]
same_as:
  wikidata: Q1773882
  wikipedia: Identifier_(computer_languages)
sources:
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Identifiers; Exported identifiers; Blank identifier
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-3.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 3
  locator: 3.8 Identifiers; 3.9 Keywords
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
  title: Lexical grammar, MDN Web Docs
  locator: Identifiers; Keywords
- url: https://en.cppreference.com/w/cpp/language/identifiers.html
  title: Identifiers, C++ reference, cppreference.com
  locator: In declarations
- url: https://docs.python.org/3/reference/lexical_analysis.html
  title: Lexical analysis, The Python Language Reference
  locator: 2.3 Names (identifiers and keywords)
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/lexical-structure
  title: Lexical structure, C# language specification
  locator: Identifiers
- url: https://doc.rust-lang.org/reference/identifiers.html
  title: Identifiers, The Rust Reference
  locator: Normalization; Raw identifiers
- url: https://www.unicode.org/reports/tr31/
  title: Unicode Standard Annex 31, Unicode Identifiers and Syntax
  locator: 2 Default Identifiers
- url: https://en.cppreference.com/w/c/language/identifier.html
  title: Identifier, C reference, cppreference.com
  locator: Reserved identifiers; Translation limits
- url: https://archive.computerhistory.org/resources/text/Fortran/102649787.05.01.acc.pdf
  title: The FORTRAN Automatic Coding System for the IBM 704 EDPM, Programmer's Reference Manual, IBM, 1956
  locator: Chapter 2, Variables
- url: https://peps.python.org/pep-3131/
  title: PEP 3131, Supporting Non-ASCII Identifiers
  locator: Abstract
- url: https://en.wikipedia.org/wiki/Identifier_(computer_languages)
  title: Identifier (computer languages), Wikipedia
  locator: Introduction
---

## Idea

A program names the things it makes: variables, functions, types, fields and modules. Each name is an identifier. The lexer reads an identifier as one token, and each language has a rule for its form, usually a letter or `_` followed by letters, digits and `_`. Some words have the form of a name but are reserved by the language. These are keywords.

```python run id=idea
import keyword

# An identifier is a name that the program chooses. The lexer checks its form,
# and a keyword has the form of a name but is reserved by the language.
for name in ["count", "_tmp", "café", "π", "x2", "2x", "my-var", "class", "match"]:
    if not name.isidentifier():
        verdict = "not a name"
    elif keyword.iskeyword(name):
        verdict = "keyword, not usable"
    elif keyword.issoftkeyword(name):
        verdict = "soft keyword, usable as a name"
    else:
        verdict = "name"
    print(f"{name:8} {verdict}")

# Names are case-sensitive, and Python compares them after NFKC normalization,
# so the ligature fi (one character) and the two letters f and i are one name.
ns = {}
exec(chr(0xFB01) + "le = 3\nFile = 4", ns)
print(sorted(k for k in ns if not k.startswith("__")))
```

```output for=idea
count    name
_tmp     name
café     name
π        name
x2       name
2x       not a name
my-var   not a name
class    keyword, not usable
match    soft keyword, usable as a name
['File', 'file']
```

`isidentifier` checks only the form, so `2x` and `my-var` fail and `class` passes. `class` is a keyword, so a program cannot use it as a name. `match` is a soft keyword: it has a special meaning only at the start of a `match` statement, and it is a normal name elsewhere. The last line shows two more rules. Names are case-sensitive, so `file` and `File` are two names, and Python normalizes names, so the ligature `ﬁ` followed by `le` is the name `file`.

## How it works

**The form.** Most languages use the same base rule. Go states it as "An identifier is a sequence of one or more letters and digits. The first character in an identifier must be a letter", and Go counts `_` as a letter[^1]. A digit cannot be first, so the lexer can tell `x2` from the number `2`. A character such as `-` cannot be in a name, so `my-var` is `my - var`. Java also allows `$`, which "should be used only in mechanically generated source code"[^2], and JavaScript allows `$` freely[^3].

**Keywords.** A keyword has the form of a name, but the grammar gives it a meaning, so the language excludes it from names. The Java grammar defines an identifier as "IdentifierChars but not a ReservedKeyword or BooleanLiteral or NullLiteral"[^2]. A contextual keyword is a keyword only in some places. In Java, `var`, `record`, `sealed`, `permits` and `yield` cannot name a type but can name a variable[^2]. In C++, `final`, `import`, `module` and `override` "are used explicitly in a certain context rather than being regular identifiers"[^4]. Contextual keywords let a language add a keyword without breaking old programs that use the word as a name. Python calls them soft keywords[^5].

**Escapes for keywords.** Some languages let a program use a keyword as a name with a prefix. In C#, "An identifier with an @ prefix is called a verbatim identifier", so `@class` is a variable named `class`[^6]. In Rust, `r#match` is a raw identifier[^7]. This matters when code calls a library that is written in another language or in an older edition, where the word was not a keyword.

**Unicode.** Most modern languages allow letters of all scripts, so `größe` and `π` are valid names. The common rule comes from Unicode Standard Annex 31, which gives the character classes `XID_Start` and `XID_Continue` for the first and the other characters of a name[^8]. The same visible text can have more than one encoding, for example `é` as one character or as `e` and a combining accent. Python converts all names "into the normalization form NFKC while parsing"[^5]. In Rust, "Identifiers are normalized using Normalization Form C (NFC)", and "Two identifiers are equal if their NFC forms are equal"[^7]. JavaScript and Go do not normalize names, so the two encodings give two names, or no name at all.

**Unicode escapes.** In JavaScript, Java and C#, a Unicode escape can spell a character of a name. MDN gives the example that `你好` and its escaped form "are the same identifiers"[^3][^2][^6].

**Names with a meaning.** In some languages, the form of a name changes what it means:

- In Go, a name is exported from its package when its first character is "a Unicode uppercase letter (Unicode character category Lu)"[^1]. The blank identifier `_` "does not introduce a binding and thus is not declared"[^1].
- In Python, a name in a class body that starts with two underscores is mangled with the class name, "to help avoid name clashes between “private” attributes of base and derived classes"[^5].
- In JavaScript, a name that starts with `#` is a private field of a class[^3].
- In C, the identifiers "that begin with an underscore followed by a capital letter or by another underscore" are reserved for the implementation, and a program that declares one has undefined behavior[^9].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | Unicode names with NFKC; hard and soft keywords; `__x` mangling |
| JavaScript | native | Unicode names, `$` and `_`; escapes; `#x` private names |
| TypeScript | native | as JavaScript |
| Java | native | Java letters and digits, `$`; contextual keywords; `_` is a keyword |
| Kotlin | native | Unicode names; soft and modifier keywords; names in backticks |
| C# | native | Unicode names; `@` verbatim identifiers; contextual keywords; `nameof` |
| Go | native | letters and digits; upper-case first letter exports; blank `_` |
| Rust | native | XID names with NFC; `r#` raw identifiers |
| C | native | separate name spaces; reserved `_X` and `__x`; C23 XID names |
| C++ | native | identifiers with special meaning; namespaces; overloading |
| Bash | native | variable names of letters, digits and `_`; function names are looser |
| PowerShell | native | `$name` variables; `${any text}` names; case-insensitive |
| SQL | native | regular and quoted identifiers; case folding |
| Regex | native | named groups `(?<name>...)` |
| HTML | idiom | `id` and `name` attributes; custom element names |
| CSS | native | identifiers for classes, IDs, properties and custom properties |

All the languages in the table have names, so most verdicts are native. HTML has no variables, but the `id` attribute names an element so that CSS, scripts and links can refer to it, so the verdict is idiom. The languages differ in which characters a name can hold, whether the case matters, whether names are normalized, and how a program can use a keyword as a name.

The example files show these rules. Python shows name mangling, a key that is not a valid name, and the soft keyword `match`. JavaScript shows a Unicode escape, reserved words as property names, two encodings of `café` that are two names, and a private field. Java shows contextual keywords and `_`. C# shows `@class` and `nameof`. Go shows export by case and the blank identifier. Rust shows a raw identifier and NFC normalization. C shows separate name spaces and `__func__`. C++ shows names with special meaning, namespaces and overloading.

## Variations and related ideas

- **Case.** Most languages are case-sensitive. SQL folds the case of a name that has no quotes, and PowerShell, Fortran, Pascal and Visual Basic ignore case.
- **Sigils.** Perl, PHP, Bash and PowerShell put `$` before a variable name. In Perl, the sigil also gives the kind of value, such as `@` for an array.
- **Quoted names.** SQL `"order"`, Kotlin and Scala names in backticks, C# `@` and Rust `r#` let a program use a name that is a keyword or that holds other characters.
- **Naming conventions.** `snake_case`, `camelCase`, `PascalCase` and `SCREAMING_CASE` are conventions, not rules, but Go export and Rust lints depend on case.
- **Scope and binding.** The identifier is the text. What it refers to depends on the scope and the bindings at that point in the program. See [lexical scope](/lexical-scope/).
- **Names at run time.** Reflection reads the names of fields and methods, and a stack trace shows the names of functions. See [runtime reflection](/runtime-reflection/). C and C++ give `__func__`, and C# gives `nameof`.

## Pitfalls

- **Confusable characters.** Unicode names allow letters that look the same, such as Latin `a` and Cyrillic `а`. Two names can look equal and be different. Rust warns about mixed-script confusables, and code review tools can flag non-ASCII names.
- **Names that shadow built-ins.** `list = [1, 2]` in Python or `let name` in a browser script hides a built-in name in that scope. Later code that uses the built-in fails.
- **New keywords.** A new version of a language can make a word a keyword, and old code that uses it as a name stops compiling. Languages use contextual keywords, editions and raw identifiers to reduce this problem.
- **Reserved names in C and C++.** Names such as `_Data` or `__count` are reserved, and a library header can define a macro with the same name. Do not start names with `_` and an upper-case letter or with two underscores.
- **Normalization differences.** A name copied from a document can be in another normal form. Python and Rust treat the two forms as one name, and JavaScript treats them as two.
- **Case and the file system.** Two names that differ only in case, such as `Config` and `config`, are different in the language, but can clash as file names on a file system that ignores case.

## Cost

In a compiled language, names have no cost at run time: the compiler turns each use into an address, an offset or a register. Names stay in the output only as metadata for reflection, as debug information or as symbols for the linker. Old C linkers had limits on the length of external names, so the C standard required only 6 significant initial characters in an external identifier before C99, and 31 after[^9]. In a dynamic language such as Python, many names are keys in a dictionary at run time, so a lookup of a global name costs a hash lookup, and a local name in a function is an index into an array.

## History

The 1956 FORTRAN manual for the IBM 704 allowed a variable name of "1 to 6 alphabetic or numeric characters", and the first letter gave the type: a name that starts with I, J, K, L, M or N is a fixed-point (integer) variable[^10]. This rule for names that start with I to N stayed in Fortran as implicit typing. C kept short external names for a long time because of the limits of linkers[^9].

The character set of names grew with Unicode. Unicode Standard Annex 31 gives the default rules that many languages now use[^8]. PEP 3131 of 2007 proposed "to support non-ASCII letters (such as accented characters, Cyrillic, Greek, Kanji, etc.) in Python identifiers", and Python 3.0 added them[^11]. C23 changed the C rule to the Unicode classes `XID_Start` and `XID_Continue`[^9]. Wikipedia describes the identifier as "a lexical token" that "names the language's entities"[^12].

## References
