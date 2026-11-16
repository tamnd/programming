---
tag: Z13EGF
kind: task
slug: apply-a-caesar-or-rot13-cipher
name: Apply a Caesar or ROT13 cipher
aliases: [Caesar cipher, Caesar shift, shift cipher, ROT13, rot-13, rot13 encode, rotate letters, letter rotation]
area: data-and-values/classic-string-tasks
level: intro
summary: >-
  Move each letter a fixed number of places along the alphabet, from z back to a, and keep the case and all other characters. ROT13 is a shift of 13 and is its own inverse. The cipher gives no security.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Caesar_cipher
  title: Caesar cipher, Wikipedia
  locator: Introduction
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Caesar-cipher/00-TASK.txt
  title: Rosetta Code, Caesar cipher (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Rot-13/00-TASK.txt
  title: Rosetta Code, Rot-13 (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/ROT13
  title: ROT13, Wikipedia
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
  title: Remainder (%), MDN Web Docs
  locator: Description
- url: https://doc.rust-lang.org/std/primitive.i64.html
  title: i64, The Rust Standard Library
  locator: rem_euclid
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: str.maketrans; str.translate
- url: https://docs.python.org/3/library/codecs.html
  title: codecs, Codec registry and base classes, Python documentation
  locator: Text Transforms (rot_13)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tr.html
  title: tr, The Open Group Base Specifications Issue 8
  locator: Description; Operands
---

## Task

A Caesar cipher replaces each letter with a letter a fixed number of places along the alphabet. "The method is named after Julius Caesar, who used it in his private correspondence."[^1] The Rosetta Code task asks for "both encoding and decoding"[^2]. Its example is that the key 2 changes `HI` to `JK`, and the key 20 changes `HI` to `BC`. The alphabet wraps, so the letter after `Z` is `A`.

ROT13 is the Caesar cipher with the shift 13. The Rosetta Code task explains that it "is commonly known from the early days of Usenet"[^3], where it hid spoilers and offensive text from a casual reader. Because the alphabet has 26 letters, "the ROT13 function is its own inverse"[^4]: the same function encodes and decodes.

The function `caesar(text, shift)` moves each letter `A` to `Z` and `a` to `z` by `shift` places, from `z` back to `a`, and keeps its case. All other characters, such as digits, spaces, punctuation and letters outside ASCII, stay as they are. The shift can be any integer. A shift of 26 changes nothing, and a negative shift moves the letters back, so `caesar(caesar(text, k), -k)` is `text`.

The cipher is a puzzle, not a protection. As Wikipedia says, it "offers essentially no communications security"[^1]. The Rosetta Code task notes that an attacker "can either use frequency analysis to guess the key, or just try all 25 keys"[^2].

## Solutions

Each solution first reduces the shift to a number `k` from 0 to 25. Then a letter with the index `i` from 0 to 25 in its alphabet becomes the letter with the index `(i + k) mod 26`.

| Language | Shift from 0 to 25 | Method |
|---|---|---|
| Python | `shift % 26` | `str.maketrans` and `str.translate` |
| JavaScript, TypeScript | `((shift % 26) + 26) % 26` | `replace(/[A-Za-z]/g, fn)` |
| Java | `Math.floorMod(shift, 26)` | a loop over a `char[]` |
| C# | `((shift % 26) + 26) % 26` | `string.Create` with a loop |
| Go | `((shift % 26) + 26) % 26` | `strings.Map` |
| Rust | `shift.rem_euclid(26)` | `chars().map(...).collect()` |
| C | `((shift % 26) + 26) % 26` | a loop over the bytes |
| C++ | `((shift % 26) + 26) % 26` | `std::ranges::transform` |
| Bash | `(((shift % 26) + 26) % 26)` | `tr` with two rotated alphabets |

- **The remainder.** In most languages of this site, `%` gives a negative result for a negative left side. In JavaScript, the remainder "always takes the sign of the dividend"[^5], so `-1 % 26` is `-1`. `((shift % 26) + 26) % 26` gives a value from 0 to 25 for each shift. Python's `%` already gives a result with the sign of the right side. Rust has `rem_euclid`, which "Calculates the least nonnegative remainder"[^6], and Java has `Math.floorMod`.
- **Python.** `str.maketrans` "returns a translation table usable for `str.translate()`"[^7]. The solution maps the alphabet to the alphabet rotated by `k`, for upper and lower case. For ROT13 only, `codecs.encode(text, "rot13")` works: the codec rot_13 is listed with the meaning "Return the Caesar-cypher encryption of the operand"[^8].
- **Go.** `strings.Map` calls a function for each rune and builds a new string. A rune outside `A` to `Z` and `a` to `z` comes back unchanged.
- **C.** The function changes the text in place. Each byte of a UTF-8 character outside ASCII is 0x80 or more, so it is not in a range of letters, and it stays as it is.
- **Bash.** The `tr` utility copies its input "with substitution or deletion of selected characters"[^9]. The solution builds the rotated alphabets with `${upper:k}${upper:0:k}` and calls `tr 'A-Za-z'` with them. `LC_ALL=C` makes `tr` work on bytes, so the bytes of UTF-8 text outside ASCII are not changed.

Python, with a table and with the codec:

```python run id=python-caesar
import codecs
import string


def caesar(text, shift):
    k = shift % 26
    lower, upper = string.ascii_lowercase, string.ascii_uppercase
    table = str.maketrans(lower + upper, lower[k:] + lower[:k] + upper[k:] + upper[:k])
    return text.translate(table)


secret = caesar("Hello, World!", 3)
print(secret, caesar(secret, -3))
print(caesar("HI", 2), caesar("HI", 20), -1 % 26)
print(codecs.encode("Hello, World!", "rot13"))
print(caesar(caesar("Uryyb", 13), 13))
```

```output for=python-caesar
Khoor, Zruog! Hello, World!
JK BC 25
Uryyb, Jbeyq!
Uryyb
```

## Edge cases

- **Empty text.** The result is empty.
- **Shift 0 and shift 26.** The text does not change.
- **Negative shift.** `caesar("abc", -1)` is `zab`. A shift of -13 decodes ROT13, and so does a shift of 13.
- **A shift above 26.** 27 is the same as 1, so `caesar("xyz", 27)` is `yza`.
- **The end of the alphabet.** `caesar("Zz", 1)` is `Aa`, not `[` and `{`, the characters after `Z` and `z` in ASCII.
- **Characters next to the letters.** `@` comes before `A`, and `[`, `` ` `` and `{` come next to the letters. They are not letters, so they stay as they are.
- **Letters outside ASCII.** `café ü` with shift 1 becomes `dbgé ü`. The contract changes only `A` to `Z` and `a` to `z`.
- **A large shift.** With the shift 2^53 - 1, which is 5 mod 26, `abc` becomes `fgh`. A program that adds the shift to the letter before it takes the remainder can overflow a 32-bit integer.

## Pitfalls

- **A negative remainder.** In JavaScript, Java, C#, Go, C and C++, `%` keeps the sign of the left side. A shift of -1 then gives an index of -1, and the program makes a character that is not a letter:

```javascript wrong id=javascript-negative-remainder
function caesar(text, shift) {
  return text.replace(/[a-z]/g, (c) =>
    String.fromCharCode(((c.charCodeAt(0) - 97 + shift) % 26) + 97),
  );
}

console.log(caesar("abc", 1), caesar("abc", -1));
```

```output for=javascript-negative-remainder
bcd `ab
```

`a` became `` ` ``, the character before `a`. Reduce the shift with `((shift % 26) + 26) % 26` first.

- **Only one case.** A loop that tests only `a` to `z` leaves upper case letters unchanged. A loop that uses the base `'a'` for upper case letters too gives wrong characters, because `'Z' - 'a'` is negative.
- **Letters outside ASCII.** `str.isalpha` in Python, `Character.isLetter` in Java and `char.IsLetter` in C# accept `é`. Then `c - 'a'` is not an index from 0 to 25. Test the ranges `A` to `Z` and `a` to `z`.
- **Security.** A Caesar cipher or ROT13 hides text from a casual reader only. Never use it to protect a password or private data.

## Related tasks

- [[count-the-letters-of-a-text]]: the letter counts that frequency analysis uses to break a Caesar cipher.
- [[check-whether-a-string-is-a-palindrome]]: another loop that treats letters and other characters in different ways.

## References
