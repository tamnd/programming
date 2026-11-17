---
tag: NCEZV1
kind: feature
owner: python
slug: cryptographic-services
name: Cryptographic services
aliases: [hashlib, hmac, secrets, sha256, message digest, password hashing, pbkdf2_hmac, scrypt, compare_digest]
area: languages/python/stdlib
level: core
summary: >-
  hashlib computes secure hashes such as SHA-256, and it derives keys from passwords. hmac signs messages with a secret key. secrets makes secure random tokens. There is no encryption module.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/crypto.html", locator: "Cryptographic Services"}
proposals: []
see_also: [python/mathematics, python/pickle-and-object-serialization, python/data-compression, python/bytes-and-bytearray]
sources:
  - url: https://docs.python.org/3.15/library/crypto.html
    title: Cryptographic Services, Python 3.15 documentation
    locator: Cryptographic Services
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/hashlib.html
    title: hashlib, Secure hashes and message digests, Python 3.15 documentation
    locator: Hash algorithms; Usage; File hashing; Key derivation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/hmac.html
    title: hmac, Keyed-Hashing for Message Authentication, Python 3.15 documentation
    locator: Introduction; compare_digest()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/secrets.html
    title: secrets, Generate secure random numbers for managing secrets, Python 3.15 documentation
    locator: Generating tokens; How many bytes should tokens use?
    accessed: 2026-11-16
---

## Synopsis

```python run id=hash-and-sign
import hashlib
import hmac

print(hashlib.sha256(b"hello").hexdigest())
h = hashlib.sha256()
h.update(b"hel")
h.update(b"lo")
print(h.hexdigest() == hashlib.sha256(b"hello").hexdigest(), h.digest_size)
key = b"server-secret"
tag = hmac.new(key, b"amount=10", "sha256").hexdigest()
print(hmac.compare_digest(tag, hmac.new(key, b"amount=10", "sha256").hexdigest()))
print(hmac.compare_digest(tag, hmac.new(key, b"amount=99", "sha256").hexdigest()))
```

```output for=hash-and-sign
2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824
True 32
True
False
```

## Semantics

**r1.** "The modules described in this chapter implement various algorithms of a cryptographic nature."[^1] The chapter has `hashlib`, `hmac` and `secrets`. It has no module for encryption.

**r2.** `hashlib` "implements a common interface to many different hash algorithms."[^2] A hash object takes bytes with `update`, and `digest` or `hexdigest` returns the result. "Repeated calls are equivalent to a single call with the concatenation of all the arguments".[^2] Example: [`hash-and-sign`](#hash-and-sign).

**r3.** Some constructors are "always present in this module", such as `sha256`, `sha3_256` and `blake2b`.[^2] `hashlib.new(name)` makes a hash object from a name. "Some algorithms have known hash collision weaknesses (including MD5 and SHA1)."[^2] Example: [`more-hashes`](#more-hashes).

**r4.** A fast hash is not a password hash. "Naive algorithms such as sha1(password) are not resistant against brute-force attacks. A good password hashing function must be tunable, slow, and include a salt."[^2] `pbkdf2_hmac` and `scrypt` are such functions. Example: [`password-hash`](#password-hash).

**r5.** `hmac` "implements the HMAC algorithm as described by RFC 2104."[^3] An HMAC tag shows that a message comes from someone who has the key and that nobody changed it. Example: [`hash-and-sign`](#hash-and-sign).

**r6.** Compare tags with `hmac.compare_digest`. It "uses an approach designed to prevent timing analysis by avoiding content-based short circuiting behaviour, making it appropriate for cryptography."[^3] `secrets.compare_digest` does the same job.

**r7.** `secrets` makes tokens: `token_bytes`, `token_hex` and `token_urlsafe`. "As of 2015, it is believed that 32 bytes (256 bits) of randomness is sufficient for the typical use-case expected for the secrets module."[^4] Example: [`tokens`](#tokens).

## Examples

```python run id=password-hash
import hashlib
import hmac
import os

def hash_password(password, salt=None):
    salt = salt or os.urandom(16)
    key = hashlib.pbkdf2_hmac("sha256", password.encode(), salt, 600_000)
    return salt, key

salt, stored = hash_password("correct horse")
print(len(salt), len(stored))
print(hmac.compare_digest(hash_password("correct horse", salt)[1], stored))
print(hmac.compare_digest(hash_password("wrong guess", salt)[1], stored))
print(hash_password("correct horse")[1] == stored)
```

```output for=password-hash
16 32
True
False
False
```

Store the salt and the key. To check a password, derive the key again with the stored salt and compare (rule r4). A new salt gives a different key for the same password. The salt "should be about 16 or more bytes from a proper source, e.g. os.urandom()".[^2] For the iteration count, "As of 2022, hundreds of thousands of iterations of SHA-256 are suggested."[^2]

```python run id=more-hashes
import hashlib
import io

f = io.BytesIO(b"x" * 1_000_000)
print(hashlib.file_digest(f, "sha256").hexdigest()[:16])
print(sorted(hashlib.algorithms_guaranteed)[:4])
print(hashlib.new("sha3_256", b"hello").hexdigest()[:16], hashlib.blake2b(b"hello", digest_size=8).hexdigest())
```

```output for=more-hashes
1b977e9f84f1b26b
['blake2b', 'blake2s', 'md5', 'sha1']
3338be694f50c5f3 a7b6eda801e5347d
```

`file_digest` reads a file object opened in binary mode and returns a hash object. `blake2b` takes a `digest_size`, so it can make a short hash (rule r3).

```python run id=tokens
import secrets

print(len(secrets.token_bytes(32)), len(secrets.token_hex(32)), len(secrets.token_urlsafe(32)))
print(secrets.compare_digest("abc", "abc"), secrets.randbelow(10) < 10)
```

```output for=tokens
32 64 43
True True
```

Each function takes the number of random bytes. For `token_urlsafe`, "The text is Base64 encoded, so on average each byte results in approximately 1.3 characters."[^4] The values change on each run, so the example prints only their lengths (rule r7).

## Versions

> [!since v="3.11"]
> `hashlib.file_digest` is new.[^2]

## Pitfalls

```python wrong id=plain-password-hash
import hashlib

users = {"ann": "letmein", "bob": "letmein"}
table = {name: hashlib.sha256(pw.encode()).hexdigest() for name, pw in users.items()}
print(table["ann"] == table["bob"], table["ann"][:16])
```

```output for=plain-password-hash
True 1c8bfe8f801d7974
```

With no salt, two users with the same password have the same hash, and one guess checks every user at once. SHA-256 is also fast, which helps an attacker who tries many guesses (rule r4). Use `pbkdf2_hmac` or `scrypt` with a random salt, as in [`password-hash`](#password-hash).

```python fails id=str-input expect=exception:TypeError
import hashlib

print(hashlib.sha256("hello").hexdigest())
```

A hash works on bytes, not on text. Encode the string first, as in `"hello".encode()`.

Do not compare a tag with `==`. Use `hmac.compare_digest`, which does not stop at the first different character (rule r6).

## Comparison

The standard library has hashing, message authentication and random tokens, but no ciphers. Programs that must encrypt data use a third-party package. For random numbers in a game or a simulation, see [[python/mathematics]]: the `random` module is not for security.
