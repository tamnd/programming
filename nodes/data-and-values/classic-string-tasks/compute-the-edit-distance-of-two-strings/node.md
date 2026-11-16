---
tag: QQS374
kind: task
slug: compute-the-edit-distance-of-two-strings
name: Compute the edit distance of two strings
aliases: [edit distance, Levenshtein distance, string distance, fuzzy match, spelling distance, Wagner-Fischer, minimum edits]
area: data-and-values/classic-string-tasks
level: core
summary: >-
  Compute the Levenshtein distance of two strings, the minimum number of one-character insertions, deletions and substitutions that change one into the other, with a dynamic program that keeps two rows.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Levenshtein-distance/00-TASK.txt
  title: Rosetta Code, Levenshtein distance (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Levenshtein_distance
  title: Levenshtein distance, Wikipedia
  locator: Introduction; Properties and bounds; Applications; Relationship to other string distances; Memory reduction and edit-script reconstruction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String types; Conversions to and from a string type
---

## Task

The Levenshtein distance of two strings "is defined as the minimum number of edits needed to transform one string into the other, with the allowable edit operations being insertion, deletion, or substitution of a single character"[^1]. The Rosetta Code task gives the classic example: the distance between `kitten` and `sitting` is 3. Replace `k` with `s`, replace `e` with `i`, and insert `g` at the end. The distance "is named after Soviet mathematician Vladimir Levenshtein, who introduced it in 1965"[^2].

The function `levenshtein(a, b)` returns this distance for two ASCII strings. Upper and lower case letters are different characters, so `levenshtein("Apple", "apple")` is 1. Wikipedia lists its uses: "Applications include spell checkers, correction of optical character recognition output, and approximate lookup in textual databases."[^2]

This page is about the code in each language. The algorithm, its proof and faster methods are on the algorithm site, in [[al:edit-distance|Edit distance]].

## Solutions

The standard method is a dynamic program. Let `d[i][j]` be the distance between the first `i` characters of `a` and the first `j` characters of `b`. Then:

- `d[i][0]` is `i`, because the program deletes `i` characters.
- `d[0][j]` is `j`, because the program inserts `j` characters.
- `d[i][j]` is the minimum of `d[i-1][j] + 1` (delete), `d[i][j-1] + 1` (insert) and `d[i-1][j-1] + cost`, where `cost` is 0 if the two characters are equal and 1 if not.

The answer is `d[len(a)][len(b)]`. The full table takes O(mn) time and memory. But "If only the numerical distance is required, each row depends only on the previous row and the current row"[^2]. So all solutions here keep two rows of `len(b) + 1` numbers, and swap them after each row. The memory is then O(n).

| Language | Rows | Minimum of three |
|---|---|---|
| Python | two lists | `min(x, y, z)` |
| JavaScript, TypeScript | two arrays, swapped with a destructuring assignment | `Math.min(x, y, z)` |
| Java | two `int[]`, swapped after each row | `Math.min(x, Math.min(y, z))` |
| C# | two `int[]` | `Math.Min(x, Math.Min(y, z))` |
| Go | two `[]int` | the built-in `min(x, y, z)` |
| Rust | two `Vec<usize>` and `std::mem::swap` | `x.min(y).min(z)` |
| C | two arrays from `malloc` | a small helper |
| C++ | two `std::vector<std::size_t>` and `std::swap` | `std::min({x, y, z})` |
| Bash | two indexed arrays | tests in `(( ))` |

- **Python.** The standard library has no Levenshtein function. `difflib.SequenceMatcher` computes a ratio of a different kind. Packages on PyPI give fast versions in C.
- **Rust and C++.** The solutions swap the two rows with `std::mem::swap` or `std::swap`, so no row is copied.
- **C.** The function allocates two rows of `len(b) + 1` numbers and frees them before it returns. It returns -1 if the allocation fails.
- **Bash.** The function works on the characters of the two strings with `${a:i-1:1}`. It is slow for long strings, because each step of the loop runs in the shell. Use it for short words only.

Python, with the examples of the Rosetta Code task:

```python run id=python-levenshtein
def levenshtein(a, b):
    prev = list(range(len(b) + 1))
    for i, ca in enumerate(a, 1):
        cur = [i]
        for j, cb in enumerate(b, 1):
            cur.append(min(prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (ca != cb)))
        prev = cur
    return prev[-1]


print(levenshtein("kitten", "sitting"))
print(levenshtein("rosettacode", "raisethysword"))
print(levenshtein("edocattesor", "drowsyhtesiar"))
print(levenshtein("ab", "ba"), levenshtein("", "abc"))
```

```output for=python-levenshtein
3
8
8
2 3
```

The third line uses the two strings in reverse. The task notes that "The distance between two strings is same as that when both strings are reversed."[^1]

## Edge cases

- **Empty strings.** The distance from the empty string to `abc` is 3, the length of `abc`. Two empty strings have the distance 0.
- **Equal strings.** The distance is 0.
- **A swap of two characters.** `ab` and `ba` have the distance 2, not 1, because Levenshtein has no swap operation. The Damerau-Levenshtein distance "additionally allows a transposition of two adjacent symbols"[^2].
- **Case.** `Apple` and `apple` have the distance 1.
- **Bounds.** The distance is at least the difference of the two lengths, and at most the longer length. Wikipedia explains the lower bound: "a single insertion or deletion changes the string length by only one"[^2]. `abc` and `abcdef` have the distance 3, and `abc` and `xyz` also have the distance 3.

## Pitfalls

- **Bytes are not characters.** In Go, "A string's bytes can be accessed by integer indices"[^3]. A loop over `s[i]` compares bytes, and `é` is 2 bytes in UTF-8:

```go wrong id=go-byte-distance
package main

import "fmt"

func distance(a, b string) int {
	prev := make([]int, len(b)+1)
	for j := range prev {
		prev[j] = j
	}
	for i := 1; i <= len(a); i++ {
		cur := make([]int, len(b)+1)
		cur[0] = i
		for j := 1; j <= len(b); j++ {
			cost := 1
			if a[i-1] == b[j-1] {
				cost = 0
			}
			cur[j] = min(prev[j]+1, cur[j-1]+1, prev[j-1]+cost)
		}
		prev = cur
	}
	return prev[len(b)]
}

func main() {
	fmt.Println(len("café"), distance("café", "cafe"))
	fmt.Println(len([]rune("café")))
}
```

```output for=go-byte-distance
5 2
4
```

The distance of `café` and `cafe` is 1. A conversion to `[]rune` gives "a slice containing the individual Unicode code points of the string"[^3]. The same problem occurs in C and Rust with bytes, and in JavaScript, Java and C# with UTF-16 code units for characters outside the Basic Multilingual Plane.

- **A full table for long strings.** Two strings of 100,000 characters need a table of 10^10 numbers. Keep two rows.
- **Recursion without a table.** The recursive definition calls itself three times at each step, so a direct translation takes exponential time. Use the table, or a cache.
- **A distance is not a similarity.** A distance of 3 is small for two long strings and large for two short ones. To compare, divide by the longer length, or use a similarity ratio.

## Related tasks

- [[check-whether-a-string-is-a-palindrome]]: another loop over the characters of two sequences.
- [[detect-anagrams]]: equality of letter counts, which ignores the order.

## References
