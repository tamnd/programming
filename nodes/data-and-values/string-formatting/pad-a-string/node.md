---
tag: 30T0B3
kind: task
slug: pad-a-string
name: Pad a string
aliases: [pad a string, left pad, right pad, padStart, padEnd, rjust, ljust, PadLeft, PadRight, zero padding, leading zeros, fill character, justify text, leftpad]
area: data-and-values/string-formatting
level: intro
summary: >-
  Pad a string to a width with a fill character, on the left and on the right, with padStart, rjust, PadLeft or a format width. A string that is already wide enough does not change.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: str.rjust; str.ljust; str.zfill
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Align-columns/00-TASK.txt
  title: Rosetta Code, Align columns (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/padStart
  title: String.prototype.padStart(), MDN Web Docs
  locator: Description; Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Class String, Java SE 25 API
  locator: repeat
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.padleft?view=net-9.0
  title: String.PadLeft Method, .NET API
  locator: Overloads; Remarks
- url: https://pkg.go.dev/fmt
  title: Package fmt, The Go Standard Library
  locator: Printing; Other flags
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Fill/Alignment
- url: https://en.cppreference.com/w/cpp/utility/format/spec.html
  title: Standard format specification, C++ reference, cppreference.com
  locator: Fill and align
---

## Task

Make a string a given width by adding a fill character. Padding on the left puts the text on the right of the field, and padding on the right puts it on the left. Python says: "Return the string right justified in a string of length width. Padding is done using the specified fillchar (default is an ASCII space)"[^1].

The function `pad(s, width, fill)` returns two strings:

1. `s` with `fill` added on the left until the length is `width`;
2. `s` with `fill` added on the right until the length is `width`.

For example, `pad("7", 3, "0")` is `("007", "700")`, and `pad("abc", 6, "*")` is `("***abc", "abc***")`.

If `s` has `width` characters or more, both results are `s`. Padding never cuts text: "The original string is returned if width is less than or equal to len(s)"[^1].

Padding is the base of text tables, fixed-width records and numbers with leading zeros. The Rosetta Code task Align columns asks to "allow for each word in a column to be either left justified, right justified, or center justified within its column"[^2], and each of those is a padding.

## Solutions

| Language | Left padding | Right padding |
|---|---|---|
| Python | `s.rjust(width, fill)` | `s.ljust(width, fill)` |
| JavaScript, TypeScript | `s.padStart(width, fill)` | `s.padEnd(width, fill)` |
| Java | `fill.repeat(n) + s` | `s + fill.repeat(n)` |
| C# | `s.PadLeft(width, c)` | `s.PadRight(width, c)` |
| Go | `strings.Repeat(fill, n) + s` | `s + strings.Repeat(fill, n)` |
| Rust | `fill.repeat(n) + s` | `s + &fill.repeat(n)` |
| C | `memset` and `memcpy` into a buffer | the same |
| C++ | `std::string(n, c) + s` | `s + std::string(n, c)` |
| Bash | `printf '%*s'` and a replacement | the same |

In the table, `n` is `max(0, width - len(s))`.

- **Python.** `rjust` and `ljust` take the fill character. `center` pads both sides, and `zfill` pads with zeros after the sign.
- **JavaScript.** `padStart` "pads this string with a given string (repeated and/or truncated, if needed) so that the resulting string has a given length"[^3]. If the width "is less than or equal to str.length, then str is returned as-is"[^3]. The fill can be longer than one character.
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** `String` has no pad method. `repeat` "Returns a string whose value is the concatenation of this string repeated count times"[^4]. `String.format("%5s", s)` pads with spaces only.
- **C#.** `PadLeft` "right-aligns the characters in this instance by padding them with spaces on the left"[^5], and an overload takes a fill character. If the width is less than the length, "the method returns a reference to the existing instance"[^5].
- **Go.** `strings.Repeat` builds the fill. `fmt.Sprintf("%*s", width, s)` pads with spaces, and the flag `-` is "pad with spaces on the right rather than the left"[^6]. The flag `0` pads numbers with zeros.
- **Rust.** `str::repeat` builds the fill. `format!` has a fill too, but the fill character must be written in the format string: "The optional fill character and alignment is provided normally in conjunction with the width parameter"[^7]. So `format!("{s:*>6}")` works, but a fill from a variable needs `repeat`.
- **C.** C has no string type with padding. The solution writes the fill with `memset` and copies the text with `memcpy`, into buffers from the caller. `printf("%*s")` pads with spaces only.
- **C++.** `std::string(n, c)` makes the fill. In `std::format`, the "fill-and-align is an optional fill character (which can be any character other than { or }), followed by one of the align options"[^8], but it is part of the format string, as in Rust.
- **Bash.** `printf -v p '%*s' n ''` makes `n` spaces, and `${p// /"$fill"}` changes them to the fill character.

The Python solution:

```python run id=python-pad
def pad(s, width, fill):
    return s.rjust(width, fill), s.ljust(width, fill)


print(pad("7", 3, "0"))
print(pad("abc", 6, "*"))
print(pad("hello", 3, " "))
print("-42".zfill(6), "-42".rjust(6, "0"))
```

```output for=python-pad
('007', '700')
('***abc', 'abc***')
('hello', 'hello')
-00042 000-42
```

## Edge cases

- **Empty string.** The result is only the fill: `pad("", 3, "-")` is `("---", "---")`.
- **Width 0 or the exact width.** No padding: `pad("abcd", 4, ".")` is `("abcd", "abcd")`.
- **A string wider than the width.** The string does not change, and nothing is cut.
- **A negative number as text.** `pad("-42", 6, "0")` gives `000-42`, because the function does not know that `-` is a sign. For a number, use a numeric format such as `%06d` or `zfill`, which give `-00042`.
- **A fill that is in the text.** `pad("aa", 5, "a")` is `("aaaaa", "aaaaa")`. The two results are the same.
- **Spaces inside the text.** They count as characters: `pad("a b", 7, ".")` is `("....a b", "a b....")`.

## Pitfalls

- **Length is not width on the screen.** The contract uses ASCII. With other text, `len` counts code points in Python, UTF-16 units in JavaScript, Java and C#, and bytes in Go, Rust, C and Bash with a byte locale. `"é".padStart(3)` can give 2 or 1 spaces for the same letter, and a Chinese character takes 2 columns on a terminal:

```javascript wrong id=javascript-pad-code-units
const composed = "\u00e9";
const decomposed = "e\u0301";
const emoji = "\u{1F600}";
// Show each code point outside ASCII as \u{...}.
const show = (t) =>
  [...t].map((c) => (c < "\u0080" ? c : `\\u{${c.codePointAt(0).toString(16)}}`)).join("");
for (const s of [composed, decomposed, emoji]) {
  console.log(show(s.padStart(4, ".")), s.length);
}
```

```output for=javascript-pad-code-units
...\u{e9} 1
..e\u{301} 2
..\u{1f600} 2
```

All three strings show as one character on the screen, but the padding is different. For text from people, count grapheme clusters with `Intl.Segmenter`, or measure the columns with a library.

- **Zero padding of a negative number.** Padding the text `-42` gives `000-42`. Pad the digits and add the sign after, or use a number format.
- **A string padding function that cuts.** Some format widths cut: in C, the precision `%.3s` cuts a string. A width alone never cuts.

## Related tasks

- [[print-formatted-output]]: format widths pad each column.
- [[format-specifiers]]: the fill, the alignment and the width in a format string.

## References
