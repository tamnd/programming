---
tag: 55TSM1
kind: task
slug: detect-anagrams
name: Detect anagrams
aliases: [anagram, anagram check, are anagrams, valid anagram, group anagrams, anagram groups, same letters, letter multiset]
area: data-and-values/classic-string-tasks
level: intro
summary: >-
  Check whether two texts use the same letters the same number of times, with case, spaces and punctuation ignored: count the letters, or sort them and compare. Group anagrams by their sorted letters.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Anagram
  title: Anagram, Wikipedia
  locator: Introduction
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Anagrams/00-TASK.txt
  title: Rosetta Code, Anagrams (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/library/collections.html
  title: collections, Container datatypes, Python documentation
  locator: Counter objects
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Character.html
  title: Character (Java SE 25 and JDK 25), Oracle
  locator: isLetter(char)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: String (Java SE 25 and JDK 25), Oracle
  locator: toLowerCase()
---

## Task

Wikipedia defines an anagram as "a word or phrase formed by rearranging the letters of a different word or phrase using all the original letters exactly once"[^1]. So `listen` and `silent` are anagrams, and so are `Dormitory` and `dirty room`. The Rosetta Code task says that words "composed of the same characters, but in a different order"[^2] are anagrams, and it asks for the largest sets of anagrams in a word list.

The function `are_anagrams(a, b)` returns true when the two texts hold each letter from `a` to `z` the same number of times. Upper case letters count as lower case letters. Spaces, punctuation and digits do not count. The contract uses ASCII text.

The definition counts each letter. It is not enough that the two texts use the same set of letters: `aab` and `abb` use the letters `a` and `b`, but they are not anagrams. Some puzzles also say that a word is not an anagram of itself. This task does not, so `are_anagrams("stop", "stop")` is true.

## Solutions

There are two methods:

- **Count the letters.** Keep 26 counters. Add 1 for each letter of `a`, and subtract 1 for each letter of `b`. The texts are anagrams if all counters are 0 at the end. This takes O(n) time, and the memory does not grow with the text.
- **Sort the letters.** Keep the letters of each text, in lower case, sort them, and compare the two results. This takes O(n log n) time. The sorted letters are also the key that groups anagrams.

| Language | Method | Counters or key |
|---|---|---|
| Python | count | `collections.Counter`, compared with `==` |
| JavaScript, TypeScript | count | an array of 26 numbers |
| Java | count | `int[26]` |
| C# | count | `Span<int>` of 26, with `stackalloc` |
| Go | count | `[26]int`, compared with `==` |
| Rust | count | `[usize; 26]`, compared with `==` |
| C | count | `int[26]` |
| C++ | count | `std::array<int, 26>`, compared with `==` |
| Bash | sort | `fold -w1`, `sort` and `tr -d` |

- **Python.** "A Counter is a dict subclass for counting hashable objects"[^3]. The solution builds one `Counter` of the letters of each text. Two counters compare equal when each element has the same count. "In equality tests, missing elements are treated as having zero counts"[^3].
- **Java, C#, JavaScript, TypeScript and C.** One array of 26 counters. The index of a letter is `c - 'a'` after the change to lower case. The solution adds for `a`, subtracts for `b`, and checks that all counters are 0.
- **Go, Rust and C++.** A helper counts the letters of one text into a fixed array. Arrays of the same type compare with `==` in these languages, so the result is `counts(a) == counts(b)`.
- **Bash.** A helper changes the text to lower case with `${1,,}`, removes all characters except `a` to `z`, puts each letter on its own line with `fold -w1`, sorts the lines with `LC_ALL=C sort`, and joins them again with `tr -d '\n'`. Two texts are anagrams when the helper prints the same text for both.

To group the anagrams in a word list, as the Rosetta Code task asks, use a map. The key of a word is its letters in sorted order, and the value is the list of words with that key:

```python run id=python-group-anagrams
from collections import defaultdict


def group_anagrams(words):
    groups = defaultdict(list)
    for word in words:
        groups["".join(sorted(word.lower()))].append(word)
    return [group for group in groups.values() if len(group) > 1]


words = ["listen", "google", "silent", "enlist", "banana", "inlets", "elgoog"]
print(group_anagrams(words))
print(sorted("listen"), "".join(sorted("listen")))
```

```output for=python-group-anagrams
[['listen', 'silent', 'enlist', 'inlets'], ['google', 'elgoog']]
['e', 'i', 'l', 'n', 's', 't'] eilnst
```

## Edge cases

- **Two empty texts.** No letters on either side, so all counts are equal. The result is true.
- **One empty text.** `a` and the empty text are not anagrams.
- **The same letters, other counts.** `aab` and `abb` are not anagrams.
- **One letter more.** `abc` and `abcd` are not anagrams.
- **Case.** `Listen` and `SILENT` are anagrams.
- **The same word.** `stop` and `stop` are anagrams under this contract.
- **Spaces and punctuation.** `The eyes!` and `They see.` are anagrams. A text with only punctuation has no letters, so `!?` and `...` give true.

## Pitfalls

- **Comparing sets.** A set keeps each letter once, so it loses the counts. A length check before the cleaning step is also wrong, because the spaces and the punctuation change the length:

```python wrong id=python-set-and-length
def are_anagrams_set(a, b):
    return set(a.lower()) == set(b.lower())


def are_anagrams_length(a, b):
    if len(a) != len(b):
        return False
    return sorted(a.lower()) == sorted(b.lower())


print(are_anagrams_set("aab", "abb"))
print(are_anagrams_length("Dormitory", "dirty room"))
```

```output for=python-set-and-length
True
False
```

Both answers are wrong. Count the letters, and check lengths only after the cleaning step, if at all.

- **Letters of other scripts.** `Character.isLetter` in Java "Determines if the specified character is a letter"[^4] in all of Unicode, not only `A` to `Z`. With an array of 26 counters, the index `c - 'a'` of such a letter is out of range. For Unicode text, use a map from code point to count, and normalize the text first.
- **The locale in a change of case.** In Java, `toLowerCase()` without an argument uses the rules of the default locale, and "may produce unexpected results if used for strings that are intended to be interpreted locale independently"[^5]. In a Turkish locale, `I` becomes a dotless `ı`, so `I` and `i` do not match. Use `toLowerCase(Locale.ROOT)`, or change only `A` to `Z`.
- **A sort that copies.** The sort method sorts the letters of both texts. For two long texts, the count method is faster and uses less memory.

## Related tasks

- [[check-whether-a-string-is-a-palindrome]]: another check on the cleaned letters of a text.
- [[count-the-letters-of-a-text]]: the counts that this task compares.
- [[map-and-dictionary]]: the map that groups the anagrams.

## References
