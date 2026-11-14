---
tag: 1FXZ14
kind: task
slug: compute-word-frequencies
name: Compute word frequencies
aliases: [most frequent words of a file, top words of a file, word frequency pipeline, tr sort uniq -c]
area: data-and-persistence/text-processing
level: core
summary: Read a file of any size in parts and return its n most frequent words, with a word as a run of ASCII letters in lower case and ties in byte order, as the tr, sort and uniq pipeline does.
contract: contract.yaml
category: text-processing
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Word-frequency/00-TASK.txt
  title: Rosetta Code, Word frequency (task text, from acmeism/RosettaCodeData)
  locator: Task; History
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map
  title: MDN Web Docs, Map
  locator: the introduction
- url: https://docs.python.org/3/library/re.html
  title: The Python Standard Library, re
  locator: the introduction (str and bytes patterns)
- url: https://docs.python.org/3/builtins/stdtypes.html
  title: The Python Standard Library, Built-in Types
  locator: bytes.lower; str.lower
- url: https://docs.python.org/3/library/collections.html#collections.Counter
  title: The Python Standard Library, collections, Counter objects
  locator: Counter; most_common
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Map.html
  title: Java SE 25 API, java.util.Map
  locator: merge
- url: https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.collectionsmarshal.getvaluereforadddefault
  title: .NET API reference, CollectionsMarshal.GetValueRefOrAddDefault
- url: https://pkg.go.dev/slices#SortFunc
  title: Go standard library, package slices
  locator: SortFunc
- url: https://doc.rust-lang.org/std/collections/struct.HashMap.html
  title: The Rust Standard Library, std::collections::HashMap
  locator: entry; get_mut
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.sort_unstable_by
  title: The Rust Standard Library, slice
  locator: sort_unstable_by
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Word-frequency/UNIX-Shell/word-frequency-1.sh
  title: Rosetta Code, Word frequency, UNIX Shell solution (from acmeism/RosettaCodeData)
  locator: the whole file
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/tr.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), tr"
  locator: OPTIONS (-c, -s)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/uniq.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), uniq"
  locator: DESCRIPTION; OPTIONS (-c)
- url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/sort.html
  title: "The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), sort"
  locator: DESCRIPTION (the order of lines that compare equal); OPTIONS (-n, -k)
- url: https://www.unicode.org/reports/tr29/
  title: "Unicode Standard Annex #29, Unicode Text Segmentation"
  locator: 2 Conformance
---

## Task

Read a text file and return its `n` most frequent words, with the number of times each word occurs. Rosetta Code gives the task as: "Given a text file and an integer n, print/display the n most common words in the file (and the number of their occurrences) in decreasing frequency."[^1]

The task text leaves some rules open. It says: "A word is a sequence of one or more contiguous letters." It also says: "You are free to define what a letter is" and "You may also choose not to support non US-ASCII characters."[^1] This page uses these rules:

- A word is a run of one or more ASCII letters, `A` to `Z` and `a` to `z`.
- Each byte that is not an ASCII letter ends a word. So a space, a line break, a digit, an apostrophe, a hyphen and each byte above 0x7F are separators. `it's` is the two words `it` and `s`, and `café` in UTF-8 gives `caf`. Rosetta Code allows this: "You may treat a compound word like well-dressed as either one word or two."[^1]
- Each word changes to lower case before it is counted, because "Uppercase letters are considered equivalent to their lowercase counterparts."[^1]
- The pairs are sorted by count from high to low. Rosetta Code says: "Words of equal frequency can be listed in any order."[^1] This page sorts words with the same count in byte order, so each solution gives the same result and a test can compare it.
- The result holds the first `n` pairs. If there are fewer than `n` words, it holds all of them. A file with no words gives an empty list.

The solutions read bytes and do not decode them. So the file can be in UTF-8, in Latin-1 or in another encoding that keeps the ASCII letters as single bytes, and bytes that are not valid UTF-8 do not cause an error. If the file does not exist, the solution reports an error. The file can be larger than the memory: the memory that a solution needs depends on the number of different words, not on the size of the file.

The task [[count-word-frequencies]] counts the words of a string in memory, and its words keep the letters that are not ASCII and the digits.

## Solutions

Each solution reads the file in parts, keeps a map from each word to its count, and sorts the entries of the map at the end. There are two ways to read the parts:

- **Line by line** (Python, JavaScript, TypeScript). LF is not a letter, so no word goes across two lines, and each line can be searched on its own. A line is in memory as a whole, so a file of one very long line needs that much memory.
- **Byte by byte through a buffer** (Java, C#, Go, Rust, C, C++). The current word stays in a variable across the reads of the buffer, so a word that the end of a buffer cuts is not split. The memory is the map and the longest word.

The steps for each byte:

1. If the byte is `A` to `Z`, add its lower case letter to the current word. If it is `a` to `z`, add it.
2. Else, if the current word is not empty, add 1 to its count in the map and clear it.
3. At the end of the file, count the current word if it is not empty.
4. Copy the entries of the map to a list. Sort the list by count from high to low, then by word, and keep the first `n` entries.

The word is the second key of the sort. The order of a map is often not fixed, and a JavaScript `Map` "remembers the original insertion order of the keys",[^2] so without the second key the order of equal counts comes from the map or from the input.

- **Python.** `open(path, 'rb')` gives the file as bytes, and a `for` loop gives one line at a time. A compiled pattern `rb'[a-z]+'` finds the words in `line.lower()`. In `re`, "Both patterns and strings to be searched can be Unicode strings ( str ) as well as 8-bit strings ( bytes )."[^3] `bytes.lower` returns "a copy of the sequence with all the uppercase ASCII characters converted to their corresponding lowercase counterpart."[^4] A `collections.Counter` counts the words: "A Counter is a dict subclass for counting hashable objects."[^5] `sorted(counts.items(), key=lambda item: (-item[1], item[0]))` sorts them, and `[:n]` keeps the first `n`.
- **JavaScript and TypeScript (Node.js).** `fs.createReadStream(path, { encoding: "latin1" })` changes each byte to one character from U+0000 to U+00FF, so the file needs no decoding and a byte above 0x7F cannot be an ASCII letter. `readline.createInterface` with `crlfDelay: Infinity` gives the lines to a `for await` loop. `line.matchAll(/[A-Za-z]+/g)` finds the words, and `toLowerCase` on each match gives only ASCII letters. A `Map` counts them. The sort compares the counts with `y - x`, and then the words with `<` and `>`, which do not use the locale. The TypeScript file adds types only.
- **Java.** A `BufferedInputStream` of 64 KiB over `Files.newInputStream(path)` gives one byte for each `read()`. A `StringBuilder` holds the current word. `counts.merge(word.toString(), 1L, Long::sum)` counts it: "If the specified key is not already associated with a value or is associated with null, associates it with the given non-null value".[^6] Else `merge` adds 1 with `Long::sum`. The sort uses `Comparator.comparingLong(WordCount::count).reversed().thenComparing(WordCount::word)`, and the result is a list of the record `WordCount`.
- **C#.** `FileStream.Read` fills a buffer of 64 KiB, and a `StringBuilder` holds the current word. `CollectionsMarshal.GetValueRefOrAddDefault` "Gets a reference to a TValue in the specified dictionary, adding a new entry with a default value if the key does not exist."[^7] So `++` on that reference counts a word with one lookup. The query is `OrderByDescending(e => e.Value).ThenBy(e => e.Key, StringComparer.Ordinal).Take(n)`.
- **Go.** `bufio.NewReaderSize(f, 64*1024)` and `ReadByte` give one byte at a time. The current word is a `[]byte`, and `counts[string(word)]++` counts it in a `map[string]int`. `slices.SortFunc` sorts with `cmp.Or(cmp.Compare(b.Count, a.Count), strings.Compare(a.Word, b.Word))`. The Go documentation says: "This sort is not guaranteed to be stable."[^8] No two entries have the same word, so no two entries compare equal, and the order is fixed.
- **Rust.** `BufReader::with_capacity(64 * 1024, file)` with `fill_buf` and `consume` gives the buffer, and the code reads its bytes. The map is a `HashMap<Vec<u8>, u64>`. `entry` "Gets the given key's corresponding entry in the map for in-place manipulation", but it takes the key by value, so each call needs a new `Vec`, also for a word that is already in the map.[^9] `get_mut` takes a borrowed key: "The key may be any borrowed form of the map's key type".[^9] So the code calls `get_mut(word.as_slice())` first, and it copies the word only for a new key. `sort_unstable_by` sorts "without preserving the initial order of equal elements",[^10] which is safe here for the reason given for Go.
- **C.** The C library has no map, so the code has a small hash table: FNV-1a hashes of 64 bits, open addressing with linear probing, and a capacity that is a power of 2. The table doubles before it is half full, so a probe always finds an empty slot. `getc` reads the bytes from the buffer of the stream. `qsort` sorts the entries, with `strcmp` as the second key. `word_frequencies` returns an array that the caller frees with `word_frequencies_free`, or it returns -1 and sets `errno`.
- **C++.** A `std::istreambuf_iterator<char>` reads the bytes of a binary `std::ifstream`. `++counts[word]` on a `std::unordered_map<std::string, std::size_t>` counts a word, because `operator[]` inserts a count of 0 for a new word. `std::ranges::sort` sorts the entries, and `resize` keeps the first `n`. The result is a `std::expected` (C++23), and the errors are `cannot_open` and `read_failed`.
- **Bash.** A pipeline does the work. The Rosetta Code solution for the Unix shell is `tr -cs A-Za-z '\n' | tr A-Z a-z | LC_ALL=C sort | uniq -c | sort -rn`, and then `head` with the number of words.[^11] The history of the task says that Knuth solved it "using literate programming and then critiqued by Doug McIlroy, demonstrating solving the problem in a 6 line Unix shell script".[^1] For `tr`, `-c` means "Complement the set of values specified by string1", and `-s` means "Replace instances of repeated characters with a single character".[^12] So `tr -cs 'A-Za-z' '\n'` changes each run of bytes that are not letters to one LF, and each word is on its own line. `uniq -c` puts before each line "a count of the number of times the line occurred in the input", but "Repeated lines in the input shall not be detected if they are not adjacent", so `sort` comes first.[^13] The solution changes three things (see Pitfalls): each command runs with `LC_ALL=C`, `grep -v '^$'` removes an empty line, and `sort -k1,1nr -k2,2` sorts the ties by word. Then `awk` keeps the first `n` lines and prints `word count`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, with one file for each in `fixtures/`.

- `simple`: `the cat and the hat` with `n = 2` gives `the 2` and `and 1`. `and`, `cat` and `hat` each occur once, so the byte order selects `and`.
- `case`: `The THE the tHe` gives `the 4`.
- `punctuation`: `it's well-dressed, isn't it?` gives `it 2`, then `dressed`, `isn`, `s`, `t` and `well` with 1 each.
- `empty` and `no-letters`: an empty file and a file of digits and punctuation give `[]`.
- `ties`: `b a c b a c` with `n = 2` gives `a 2` and `b 2`. All three words have the count 2, so the byte order decides.
- `digits`: `abc123def 42 x9` gives `abc`, `def` and `x`. A digit ends a word, and a run of digits is not a word.
- `n-zero` and `n-above-count`: `n = 0` gives `[]`. An `n` above the number of words gives all of them.
- `crlf`: CR is not a letter, so CRLF line ends do not change the words.
- `non-ascii`: `café naïve cafe` in UTF-8 gives `caf`, `cafe`, `na` and `ve`. The bytes of `é` and `ï` are above 0x7F, so they end words.
- `leading-separators`: `  ...hello, world. Hello` gives `hello 2` and `world 1`, and no empty word.
- `no-final-newline`: `dog cat dog` with no LF at the end gives `dog 2` and `cat 1`. The last word is counted at the end of the file.
- `not-utf8`: the bytes `FF FE` between two words `ok` are not valid UTF-8. They are separators, and the result is `ok 2`.

The tests also write a file of 100000 lines `a b a`, and a file of one line of 100000 words, which is longer than a buffer of 64 KiB. The case in `cases/errors.jsonl` must give the error `not-found`. The toolchain does not load that file yet, so each test file holds the case by name.

## Pitfalls

- **The empty line from `tr -cs`.** If the file starts with a byte that is not a letter, `tr -cs` changes that run to an LF too, and the first line of its output is empty. `uniq -c` counts the empty line as a word:

```bash run id=bash-tr-empty-line
printf '  ...hello, world. Hello\n' |
  LC_ALL=C tr -cs 'A-Za-z' '\n' |
  LC_ALL=C tr 'A-Z' 'a-z' |
  LC_ALL=C sort |
  LC_ALL=C uniq -c |
  awk '{ printf "[%s] %s\n", $2, $1 }'
```

```output for=bash-tr-empty-line
[] 1
[hello] 2
[world] 1
```

Remove empty lines with `grep -v '^$'` before `sort`. In the other languages, a pattern such as `[a-z]+` cannot match an empty string, and the byte loop counts a word only if it is not empty. A split at each separator byte gives empty strings, so skip them.

- **`sort -rn` does not sort the ties.** POSIX says that lines that compare equal on the keys are then compared on all their bytes, "but with -r still in effect, if it was specified".[^14] So after `sort -rn`, the words with the same count are in reverse byte order. Add the word as a second key with `-k1,1nr -k2,2`:

```bash run id=bash-sort-ties
count() {
  printf 'b a c b a c\n' | LC_ALL=C tr -cs 'A-Za-z' '\n' | LC_ALL=C sort | LC_ALL=C uniq -c
}
count | LC_ALL=C sort -rn | awk '{ print $2, $1 }'
echo ---
count | LC_ALL=C sort -k1,1nr -k2,2 | awk '{ print $2, $1 }'
```

```output for=bash-sort-ties
c 2
b 2
a 2
---
a 2
b 2
c 2
```

Python `Counter.most_common` has the same problem in a different form: "Elements with equal counts are ordered in the order first encountered".[^5] So two files with the same words in a different order give two results. Sort with the word as a second key, as all the solutions do.

- **Change the case after the match, or on bytes.** On decoded text, `str.lower` uses the Unicode case mappings: "The lowercasing algorithm used is described in section 3.13.2".[^4] Some letters that are not ASCII change to ASCII letters. KELVIN SIGN (U+212A) changes to `k`. LATIN CAPITAL LETTER I WITH DOT ABOVE (U+0130) changes to `i` and U+0307 COMBINING DOT ABOVE. So a pattern of ASCII letters that runs after `lower` finds words that are not in the file. `bytes.lower` changes only `A` to `Z`:

```python run id=python-lower-first
import re

# U+212A is KELVIN SIGN, and U+0130 is LATIN CAPITAL LETTER I WITH DOT ABOVE.
text = "\u212aelvin \u0130stanbul"
print(re.findall(r"[a-z]+", text.lower()))
print(re.findall(rb"[a-z]+", text.encode().lower()))
```

```output for=python-lower-first
['kelvin', 'i', 'stanbul']
[b'elvin', b'stanbul']
```

The JavaScript solution matches `[A-Za-z]+` first and then changes the case of the match, so only ASCII letters change.

- **Reading the whole file.** A solution that reads the whole file into one string needs memory for the whole file. A solution that decodes the file as UTF-8 can stop with an error on bytes that are not valid UTF-8, as in [[compute-a-word-count]]. Read the bytes in parts, and keep only the map.
- **The words of natural language.** The rule of this page cuts `café` into `caf` and `it's` into `it` and `s`. A rule for real text needs Unicode, and for some languages it needs more: "reliable detection of word boundaries in languages such as Thai, Lao, Chinese, or Japanese requires the use of dictionary lookup or other mechanisms".[^15] For such text, use a word segmenter that follows UAX #29, and say which rule the program uses.

## Related tasks

- [[count-word-frequencies]]: the same count for a string in memory, with letters that are not ASCII and digits in the words.
- [[compute-a-word-count]]: count the lines, words and bytes of a file, as `wc` does.
- [[sort-the-lines-of-a-file]]: sort the lines of a file, as `sort` does.
- [[unix-text-processing-pipeline]]: join small Unix tools with pipes.

## References
