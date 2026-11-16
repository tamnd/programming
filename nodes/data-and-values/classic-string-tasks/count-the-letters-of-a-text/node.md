---
tag: ACKW1C
kind: task
slug: count-the-letters-of-a-text
name: Count the letters of a text
aliases: [letter frequency, letter count, character frequency, count characters, histogram of letters, frequency table, count each letter]
area: data-and-values/classic-string-tasks
level: intro
summary: >-
  Count each letter from a to z in a text, with case ignored: an array of 26 counters indexed by the letter, or a map such as Counter. Letters outside ASCII need a map and a case fold.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Letter-frequency/00-TASK.txt
  title: Rosetta Code, Letter frequency (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Letter_frequency
  title: Letter frequency, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3/library/collections.html
  title: collections, Container datatypes, Python documentation
  locator: Counter.most_common
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Character.html
  title: Character (Java SE 25 and JDK 25), Oracle
  locator: isLetter(char)
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: str.casefold
---

## Task

The Rosetta Code task is short: "Open a text file and count the occurrences of each letter."[^1] It adds that "Some of these programs count all characters (including punctuation), but some only count letters A to Z."[^1] Letter counts are old tools. Wikipedia notes that "Letter frequency analysis dates back to the Arab mathematician Al-Kindi (c. AD 801–873), who formally developed the method to break ciphers."[^2]

The function `count_letters(text)` returns a list of 26 counts. The count at index 0 is for `a`, the count at index 1 is for `b`, and so on to `z` at index 25. An upper case letter counts as its lower case letter, so `AaAa` gives 4 at index 0. Other characters, such as digits, spaces and punctuation, do not count. The contract uses ASCII text.

## Solutions

The letters `a` to `z` have the codes 97 to 122 in ASCII, one after the other. So `c - 'a'` is the index of a lower case letter `c`, and an array of 26 counters is enough. For a text with letters outside ASCII, the set of letters is not known in advance, so the program uses a map from the letter to its count.

| Language | Counters | Index of a letter |
|---|---|---|
| Python | `collections.Counter`, then 26 counts in order | `string.ascii_lowercase` gives the order |
| JavaScript, TypeScript | `new Array(26).fill(0)` | `c.charCodeAt(0) - 97` |
| Java | `long[26]` | `c - 'a'` |
| C# | `long[26]` | `c - 'a'` |
| Go | `[]int64` of length 26 | `b - 'a'` on a byte |
| Rust | `vec![0; 26]` | `usize::from(b - b'a')` on a byte |
| C | an array of 26 `long long` from the caller | `tolower(c) - 'a'` |
| C++ | `std::vector<long long>(26)` | `std::tolower(c) - 'a'` |
| Bash | an indexed array | `$(( $(printf '%d' "'$c") - 97 ))` |

- **Python.** The solution counts the letters with `Counter` and reads the 26 counts in the order of `string.ascii_lowercase`. A `Counter` returns 0 for a letter that it has not seen. `most_common` gives a list "from the most common to the least"[^3]. "Elements with equal counts are ordered in the order first encountered"[^3].
- **JavaScript, TypeScript, Java and C#.** The solution changes each `A` to `Z` to lower case, and adds 1 to the counter at `c - 'a'` if the result is in the range `a` to `z`.
- **Go and Rust.** The text is ASCII, so the solution reads bytes. Go uses `b | 0x20` to change an upper case letter to lower case.
- **C.** The caller gives an array of 26 counters, and the function fills it. `isalpha` and `tolower` take a value of `unsigned char`.
- **Bash.** A `for` loop reads each character with `${text:i:1}`. `printf '%d' "'$c"` gives the code of the character, because a leading quote makes `printf` print the code. The function prints the 26 counts, one on each line.

Python, with `Counter`, `most_common` and a change of case that is not ASCII:

```python run id=python-counter
from collections import Counter

text = "The quick brown fox jumps over the lazy dog"
counts = Counter(c for c in text.lower() if c.isalpha())
print(len(counts), counts.most_common(3))
print("Straße".upper(), "ß".lower(), "ß".casefold())
composed, decomposed = "\u00e9", "e\u0301"
print(len(composed), len(decomposed), composed.isalpha(), decomposed.isalpha())
```

```output for=python-counter
26 [('o', 4), ('e', 3), ('t', 2)]
STRASSE ß ss
1 2 True False
```

The sentence has all 26 letters. `t`, `h`, `u` and `r` all occur twice, and `most_common` lists `t` because it comes first in the text.

## Edge cases

- **Empty text.** All 26 counts are 0. The result is still a list of 26 numbers, not an empty list.
- **No letters.** `123 !?` gives 26 zeros too.
- **Case.** `AaAa` gives 4 at index 0, not 2 and 2.
- **The first and the last letter.** `aZ` gives 1 at index 0 and 1 at index 25. A bound written as `< 'z'` instead of `<= 'z'` misses `z`.
- **White space.** Spaces, tabs and newlines do not count.

## Pitfalls

- **Letters outside A to Z.** `Character.isLetter` in Java "Determines if the specified character is a letter"[^4] in all of Unicode. With an array of 26 counters, the index of such a letter is out of range:

```java wrong id=java-is-letter-index
public class Main {
    public static void main(String[] args) {
        int[] counts = new int[26];
        try {
            for (char c : "caf\u00e9".toCharArray()) {
                if (Character.isLetter(c)) {
                    counts[Character.toLowerCase(c) - 'a']++;
                }
            }
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }
    }
}
```

```output for=java-is-letter-index
Index 136 out of bounds for length 26
```

`é` has the code 233, and `233 - 97` is 136. In C, the same code writes outside the array, and that is undefined behavior. Test the range `a` to `z`, or use a map.

- **Case in Unicode.** One upper case letter can be two lower case letters, and the change can depend on the language. `casefold` in Python "is intended to remove all case distinctions in a string"[^5]: `ß` stays `ß` with `lower`, and becomes `ss` with `casefold`. For caseless counts of Unicode text, use `casefold`, not `lower`.
- **Code points are not letters.** `é` can be one code point or two: `e` and the combining mark U+0301. The mark is not a letter, so a count of the decomposed form finds `e` and no `é`. Normalize the text, for example to NFC, before you count.
- **Bytes are not letters.** In C, Go and Rust, a loop over the bytes of UTF-8 text sees 2 bytes for `é`. That is correct for ASCII only.
- **A sorted map for the output.** A hash map has no fixed order. To print the counts in the order of the alphabet, sort the keys, or use an array as this task does.

## Related tasks

- [[count-word-frequencies]]: the same method for words.
- [[detect-anagrams]]: two texts with equal letter counts.
- [[apply-a-caesar-or-rot13-cipher]]: a cipher that letter counts can break.

## References
