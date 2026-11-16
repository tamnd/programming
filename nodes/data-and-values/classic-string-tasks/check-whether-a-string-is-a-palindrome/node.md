---
tag: ES05X5
kind: task
slug: check-whether-a-string-is-a-palindrome
name: Check whether a string is a palindrome
aliases: [palindrome, palindrome check, palindrome detection, is palindrome, valid palindrome, reads the same backwards, inexact palindrome, two pointer palindrome]
area: data-and-values/classic-string-tasks
level: intro
summary: >-
  Check whether a text reads the same backwards as forwards, with case, spaces and punctuation ignored: compare the cleaned text with its reverse, or move two indexes toward the middle.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Palindrome
  title: Palindrome, Wikipedia
  locator: Introduction; Words and phrases
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Palindrome-detection/00-TASK.txt
  title: Rosetta Code, Palindrome detection (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: str.isalnum; str.isascii; str.lower
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/split
  title: String.prototype.split(), MDN Web Docs
  locator: Description (the warning on the empty separator)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/Symbol.iterator
  title: String.prototype[Symbol.iterator](), MDN Web Docs
  locator: Return value; Description
---

## Task

A palindrome, in the words of Wikipedia, is a "sequence of symbols that reads the same backwards as forwards"[^1], such as `racecar`. For a phrase, "Punctuation, capitalization, and spaces are usually ignored"[^1], so `A man, a plan, a canal: Panama` is a palindrome. The Rosetta Code task asks for a function that "checks whether a given sequence of characters (or, if you prefer, bytes) is a palindrome"[^2], and for a second one that "detects inexact palindromes"[^2], where white space and punctuation do not count and the case does not matter.

The function `is_palindrome(text)` does the inexact check:

1. Keep only the ASCII letters and digits of the text.
2. Change `A` to `Z` to `a` to `z`.
3. Return true if the result is the same as its reverse.

So `is_palindrome("No 'x' in Nixon")` is true, and `is_palindrome("race a car")` is false. A text with no letters and no digits gives true, because an empty sequence is the same as its reverse. The contract uses ASCII text. The section Pitfalls shows what changes with other text.

## Solutions

There are two methods:

- **Compare with the reverse.** Build the cleaned text, reverse it, and compare. This is short, and it uses memory for two copies.
- **Two indexes.** Put one index at the start and one at the end. Move each index past the characters that do not count. Compare the two characters, then move both indexes toward the middle. Stop when the indexes meet. This uses no extra memory, and it stops at the first difference.

| Language | Keep letters and digits | Lower case | Method |
|---|---|---|---|
| Python | `c.isascii() and c.isalnum()` | `c.lower()` | compare with `kept[::-1]` |
| JavaScript, TypeScript | `replace(/[^a-z0-9]/g, "")` | `toLowerCase()` | two indexes |
| Java | a test on the ranges `a-z`, `A-Z`, `0-9` | `Character.toLowerCase` | two indexes |
| C# | `char.IsAsciiLetterOrDigit` | `char.ToLowerInvariant` | two indexes |
| Go | a test on the byte ranges | `b \| 0x20` on letters | two indexes |
| Rust | `u8::is_ascii_alphanumeric` | `to_ascii_lowercase` | `iter().eq(iter().rev())` |
| C | `isalnum((unsigned char)c)` | `tolower` | two indexes |
| C++ | `std::isalnum` on `unsigned char` | `std::tolower` | two indexes |
| Bash | `${s//[^a-z0-9]/}` | `${1,,}` | two indexes |

- **Python.** `str.isalnum` alone is not enough. It returns true "if all characters in the string are alphanumeric"[^3], and that includes letters such as `é` and digits of other scripts. The test `c.isascii()` keeps the check on the contract. `kept[::-1]` is the list in reverse order.
- **JavaScript and TypeScript.** The solution changes the text to lower case first, so the regular expression has to remove only the characters outside `a-z` and `0-9`. Then it compares `s[i]` with `s[s.length - 1 - i]` for the first half.
- **Java, C#, C and C++.** Each solution moves the two indexes over the original text, so it makes no copy. In C and C++, the argument of `isalnum` and `tolower` must be a value of `unsigned char`, because a negative `char` gives undefined behavior. In the "C" locale, `isalnum` is true only for ASCII letters and digits.
- **Go and Rust.** The text is ASCII, so each byte is a character, and the solutions work on bytes. `text[i]` in Go and `text.bytes()` in Rust give the bytes.
- **Bash.** `${1,,}` changes the text to lower case, and `${s//[^a-z0-9]/}` removes all other characters. The loop compares `${s:i:1}` with `${s:n-1-i:1}`. The function prints `true` or `false`.

Python, with the method that compares with the reverse:

```python run id=python-palindrome
def is_palindrome(text):
    kept = [c.lower() for c in text if c.isascii() and c.isalnum()]
    return kept == kept[::-1]


for text in ["racecar", "A man, a plan, a canal: Panama", "race a car", "0P", ""]:
    print(repr(text), is_palindrome(text))
print("é".isalnum(), "é".isascii())
```

```output for=python-palindrome
'racecar' True
'A man, a plan, a canal: Panama' True
'race a car' False
'0P' False
'' True
True False
```

## Edge cases

- **Empty text.** It is a palindrome. A program that reads `text[0]` before it checks the length fails on it.
- **No letters and no digits.** `?! ...` cleans to the empty text, so it is a palindrome too.
- **One character.** Always a palindrome. With two indexes, the loop does not run.
- **Case.** `Aa` is a palindrome only after the change to lower case.
- **Digits.** Digits count, so `12321` is a palindrome and `123` is not.
- **A digit and a letter.** `0P` is not a palindrome. A program that compares `0` (code 48) with `p` (code 112) is correct. A program that ignores digits, or that removes all characters that are not letters, gives the wrong answer.
- **Even and odd length.** `abba` has no middle character, and `abcba` has one. The two index loop stops when `i >= j`, so both work.

## Pitfalls

- **Reversing UTF-16 code units.** In JavaScript, `split("")` splits a string "by UTF-16 code units. This destroys surrogate pairs"[^4]. An emoji is a pair of code units, so the reverse of the string puts the two halves in the wrong order:

```javascript wrong id=javascript-split-reverse
function isPalindrome(s) {
  return s === s.split("").reverse().join("");
}

console.log(isPalindrome("abba"), isPalindrome("a😀a"));
const s = "a😀a";
console.log(s === [...s].reverse().join(""));
```

```output for=javascript-split-reverse
true false
true
```

The spread syntax `[...s]` uses the string iterator, which "yields the Unicode code points of the string value as individual strings"[^5], so it keeps each pair together.

- **Combining marks.** A code point is still not a character on the screen. `é` can be one code point, or `e` and the combining mark U+0301. If the program reverses the code points of `été`, each mark moves to the wrong letter. For text that is not ASCII, normalize the text first, for example to NFC, or compare grapheme clusters.
- **Letters of other scripts.** `str.isalnum` in Python, `Character.isLetterOrDigit` in Java and `char.IsLetterOrDigit` in C# accept letters and digits of all scripts. That is correct for Unicode text, but it is not the contract of this task.
- **A sign in C.** `isalnum(c)` with a `char` that has a negative value, such as a byte of UTF-8 text, is undefined behavior. Cast it to `unsigned char` first.
- **Copying in a loop.** A loop that builds the reverse with `reversed = c + reversed` copies the string at each step, so it takes O(n²) time. Use a reverse function of the language, or two indexes.

## Related tasks

- [[reverse-a-string]]: the reverse of a string, and why code points and grapheme clusters give different answers.
- [[detect-anagrams]]: another check on the letters of a text.
- [[string-concept]]: code units, code points and grapheme clusters.

## References
