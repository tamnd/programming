---
tag: NEW-generate-random-numbers
kind: task
slug: generate-random-numbers
name: Generate random numbers
aliases: [random integer in a range, roll a die, shuffle a list, seed a random number generator, secure random token]
area: data-and-values/primitive-types
level: core
summary: Get a random integer in a range with no modulo bias, shuffle a list, repeat a sequence with a seed, and make a token with a cryptographically secure generator.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/random.html
    title: The Python Standard Library, random
    locator: the warning at the top; random.getrandbits, random.randint, random.shuffle; Notes on Reproducibility
  - url: https://docs.python.org/3/library/secrets.html
    title: The Python Standard Library, secrets
    locator: secrets.randbits, secrets.token_hex
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/random
    title: MDN Web Docs, Math.random()
    locator: Description, the note on security
  - url: https://nodejs.org/api/crypto.html
    title: Node.js documentation, Crypto
    locator: crypto.getRandomValues(typedArray); crypto.randomBytes(size[, callback]); crypto.randomInt([min, ]max[, callback])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Random.html
    title: Java SE 25 API, java.util.Random
    locator: class description; nextInt(int bound)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/security/SecureRandom.html
    title: Java SE 25 API, java.security.SecureRandom
    locator: class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/HexFormat.html
    title: Java SE 25 API, java.util.HexFormat
    locator: of()
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Collections.html
    title: Java SE 25 API, java.util.Collections
    locator: shuffle(List, Random)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
    title: Java SE 25 API, java.lang.Math
    locator: abs(int)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.random?view=net-10.0
    title: .NET API reference, Random
    locator: Notes to Callers
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.random.shuffle?view=net-10.0
    title: .NET API reference, Random.Shuffle
    locator: Shuffle<T>(IList<T>)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.security.cryptography.randomnumbergenerator.gethexstring?view=net-10.0
    title: .NET API reference, RandomNumberGenerator.GetHexString
    locator: GetHexString(Int32, Boolean)
  - url: https://pkg.go.dev/math/rand/v2
    title: Go standard library, math/rand/v2
    locator: Overview; func IntN; func Shuffle; func NewPCG
  - url: https://pkg.go.dev/crypto/rand
    title: Go standard library, crypto/rand
    locator: func Read; func Text
  - url: https://doc.rust-lang.org/std/random/index.html
    title: The Rust Standard Library, std::random
    locator: the banner at the top of the module
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getentropy.html
    title: The Open Group Base Specifications Issue 8, getentropy
    locator: SYNOPSIS; DESCRIPTION; ERRORS
  - url: https://en.cppreference.com/w/c/numeric/random/rand.html
    title: cppreference.com, rand
    locator: the description; Notes
  - url: https://en.cppreference.com/w/cpp/numeric/random/random_device.html
    title: cppreference.com, std::random_device
    locator: the description of the class
  - url: https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine.html
    title: cppreference.com, std::mersenne_twister_engine
    locator: Predefined specializations
  - url: https://en.cppreference.com/w/cpp/algorithm/random_shuffle.html
    title: cppreference.com, std::random_shuffle, std::shuffle
    locator: the description of overload (3)
  - url: https://www.gnu.org/software/bash/manual/bash.html#Bash-Variables
    title: GNU Bash Reference Manual
    locator: "5.2 Bash Variables: RANDOM, SRANDOM"
  - url: https://man7.org/linux/man-pages/man4/random.4.html
    title: Linux manual page, random(4)
    locator: DESCRIPTION
  - url: https://raw.githubusercontent.com/openbsd/src/master/lib/libc/crypt/arc4random_uniform.c
    title: OpenBSD libc, arc4random_uniform.c
    locator: the comment and the body of arc4random_uniform
  - url: https://prng.di.unimi.it/splitmix64.c
    title: Sebastiano Vigna, splitmix64.c
    locator: the comment at the top; next()
---

## Task

Write four functions. `int_in_range` returns an integer from `lo` to `hi`, and each value has equal probability. `shuffle` puts a list in a random order, and each order has equal probability. A seeded generator gives the same numbers each time it gets the same seed. `token_hex(n)` gives `n` random bytes from a cryptographically secure generator as `2n` lowercase hexadecimal digits.

A random result changes on each run, so each function takes a word source: a function that returns an integer from 0 to 2^32 - 1. The cases give fixed words, and the tests check the exact results and that each word was used.

`int_in_range` uses the method of OpenBSD `arc4random_uniform`. Let `n = hi - lo + 1`. Take the next word `w`. If `w < 2^32 mod n`, reject it and take the next word. Then return `lo + (w mod n)`. Each remainder now comes from the same number of words. `n` can be 2^32, and `lo` and `hi` can be at the ends of the signed 64-bit range.

`shuffle` is the Fisher-Yates method. For `i` from the last index down to 1, it gets `j = int_in_range(0, i)` and swaps items `i` and `j`.

## Solutions

Each solution also has a seeded word source and a secure word source. In real code, use the range and shuffle functions of the library where they exist. The solutions write their own so that one contract tests each language.

- **Python.** `random.Random(seed).getrandbits(32)` is the seeded source, and `secrets.randbits(32)` is the secure source. `random.randint(a, b)` and `random.shuffle(x)` do the same work in the library. The `random` module must not be used for security. `secrets.token_hex(n)` converts each byte to two hex digits.
- **JavaScript and TypeScript (Node.js).** `Math.random()` has no seed that the user can set, so the solution has a SplitMix64 generator on `bigint` as the seeded source. `crypto.getRandomValues` fills a `Uint32Array` for the secure source. `crypto.randomBytes(n).toString("hex")` makes the token. `crypto.randomInt` avoids modulo bias, but its range must be less than 2^48.
- **Java.** The class `Random` specifies its algorithms, so `new Random(seed)` gives the same sequence in each Java implementation. `nextInt(bound)` rejects values that give an uneven distribution. `SecureRandom` is cryptographically strong. `HexFormat.of()` writes lowercase digits with no delimiter.
- **C#.** `new Random(seed)` is the seeded source, but its algorithm can change between major versions of .NET. `RandomNumberGenerator.GetBytes` is the secure source, and `RandomNumberGenerator.GetHexString(2 * n, lowercase: true)` makes the token. `Random.Shuffle` does the shuffle in the library.
- **Go.** `rand.New(rand.NewPCG(seed, 0)).Uint32` from `math/rand/v2` is the seeded source. The documentation says that this package is not for security, and points to `crypto/rand`. `rand.Read` from `crypto/rand` never returns an error and fills the buffer, and `hex.EncodeToString` makes the token. `rand.IntN` and `rand.Shuffle` give a range and a shuffle in the library.
- **Rust.** `std::random` is a nightly-only experimental API, so the solution for stable Rust has SplitMix64 as the seeded source, and reads `/dev/urandom` as the secure source, so it runs only on Unix-like systems.
- **C.** The solution has SplitMix64 as the seeded source. POSIX `getentropy` in `<unistd.h>` fills a buffer of at most `GETENTROPY_MAX` bytes from a cryptographically secure generator, so `token_hex` asks for the bytes in parts. A word source is a function pointer with a context pointer.
- **C++.** A word source is any object that returns an unsigned 32-bit value when called, so `std::mt19937` is one. `std::mt19937` is a `mersenne_twister_engine` with fixed parameters. The secure source calls `getentropy`. `std::shuffle` does the shuffle in the library.
- **Bash.** A word source is a function that sets `REPLY`. `RANDOM` gives 0 to 32767, and an assignment to it sets the seed, so the seeded source joins the bits of three values. `SRANDOM` cannot be seeded, and the manual does not say that it is secure, so the secure source reads `/dev/urandom` with `od`. Linux `random(4)` says that `/dev/urandom` is cryptographically secure against attackers without local root access, after the generator has its seed.

A program that must give the same output can print a seeded value or a property. The Python documentation says that `random()` gives the same sequence for the same seed in later versions. The other methods can change:

```python run id=python-seeded
import random
import secrets

rng = random.Random(42)
print(rng.random())
first = [rng.randint(1, 6) for _ in range(10)]
rng.seed(42)
rng.random()
print("same seed, same rolls:", first == [rng.randint(1, 6) for _ in range(10)])
token = secrets.token_hex(16)
print("token length:", len(token))
print("lowercase hex:", all(c in "0123456789abcdef" for c in token))
```

```output for=python-seeded
0.6394267984578837
same seed, same rolls: True
token length: 32
lowercase hex: True
```

SplitMix64 uses only 64-bit integer arithmetic, so a seed gives the same output on each platform:

```rust run id=rust-splitmix64
fn splitmix64(state: &mut u64) -> u64 {
    *state = state.wrapping_add(0x9e37_79b9_7f4a_7c15);
    let mut z = *state;
    z = (z ^ (z >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
    z = (z ^ (z >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
    z ^ (z >> 31)
}

fn main() {
    let mut state = 0;
    for _ in 0..3 {
        println!("{:#018x}", splitmix64(&mut state));
    }
}
```

```output for=rust-splitmix64
0xe220a8397b1dcdaf
0x6e789e6aa1b965f4
0x06c45d188009454f
```

## Edge cases

The cases are in `cases/basic.jsonl`, `cases/edge.jsonl` and `cases/shuffle.jsonl`.

- `one-value`: `lo = hi`. Each result is `lo`. It still takes one word, as the contract requires.
- `reject-low-words`: for a die, 2^32 mod 6 is 4, so the words 0 and 3 are rejected and the word 4 gives 5.
- `accept-least`: the word 4 is equal to 2^32 mod 6, which is the least word that is accepted.
- `power-of-two`: when `n` divides 2^32, no word is rejected.
- `worst-case`: `n = 2^31 + 1`. Almost half of the words are rejected, so a result can take many words.
- `full-unsigned-32` and `full-signed-32`: `n = 2^32`. 2^32 mod n is 0, so each word is accepted and the result is `lo + w`. A 32-bit computation of `n` or of 2^32 mod n overflows here.
- `near-int64-max`, `near-int64-min` and `wide-int64`: `hi - lo` and `lo + w mod n` must not overflow at the ends of the signed 64-bit range. Java, C# and Rust use unsigned arithmetic for the difference.
- `empty` and `one-item`: no swap, and no word is used.
- `two-items-swap` and `two-items-keep`: one word decides the order.
- `reject-in-shuffle`: the first word for `i = 2` is rejected, so the shuffle takes three words for three items.
- `duplicates` and `non-ascii`: the shuffle moves items and does not look at them.

## Pitfalls

- **A remainder is biased.** `RANDOM % 6` in Bash maps 32768 values to 6 results. 32768 is not a multiple of 6, so 0 and 1 come more often. The bias is small here, but it grows with `n`. For `n = 2^31 + 1` and a 32-bit word, each value below 2^31 - 1 is twice as probable as the other two values:

```bash wrong id=bash-modulo-bias
counts=(0 0 0 0 0 0)
for ((r = 0; r <= 32767; r++)); do
    counts[r % 6]=$((counts[r % 6] + 1))
done
echo "${counts[@]}"
```

```output for=bash-modulo-bias
5462 5462 5461 5461 5461 5461
```

- **`Math.abs` of the least `int` is negative.** The Java line `Math.abs(rng.nextInt()) % n` is biased. When `nextInt` returns `Integer.MIN_VALUE`, `Math.abs` returns the same negative value, so the result is negative. Use `rng.nextInt(n)`:

```java wrong id=java-abs-min-value
public class Main {
    public static void main(String[] args) {
        int word = Integer.MIN_VALUE;
        System.out.println(Math.abs(word));
        System.out.println(Math.abs(word) % 6);
    }
}
```

```output for=java-abs-min-value
-2147483648
-2
```

- **A swap with any index is not a fair shuffle.** If each item swaps with a random index from the full list, `n` items give `n^n` paths of the same probability. 27 paths cannot fall evenly on 6 orders. Swap item `i` only with an index from 0 to `i`:

```python wrong id=python-naive-shuffle
from collections import Counter
from itertools import product

counts = Counter()
for path in product(range(3), repeat=3):
    order = list("abc")
    for i, j in enumerate(path):
        order[i], order[j] = order[j], order[i]
    counts["".join(order)] += 1
for order, count in sorted(counts.items()):
    print(order, count)
```

```output for=python-naive-shuffle
abc 4
acb 5
bac 5
bca 5
cab 4
cba 4
```

- **A general-purpose generator is not for secrets.** Python `random`, JavaScript `Math.random`, Go `math/rand/v2` and C `rand` are not cryptographically secure. cppreference says that `rand` gives no guarantee of quality. For tokens, keys and passwords, use `secrets`, `crypto.getRandomValues`, `SecureRandom`, `RandomNumberGenerator`, `crypto/rand` or `getentropy`.
- **`std::random_device` can be deterministic.** cppreference says that an implementation can use a pseudo-random engine for it, and then each object can give the same sequence. Do not use it as the only source of a secret.
- **A seed does not always give the same numbers in a new version.** .NET does not guarantee that `Random` stays the same across major versions. Python guarantees only `random()`. If a test needs fixed numbers for all versions, write the generator, as the solutions do with SplitMix64, or give the words to the code.

## Related tasks

- [[parse-an-integer]]: read a seed or a range from text into a 64-bit integer.
- [[sort-a-list-of-records]]: put the items back in a fixed order after a shuffle.
- [[measure-elapsed-time]]: another task whose output changes on each run, and how to test it.

## References
