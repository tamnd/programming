```python run id=code-points-and-encodings
import unicodedata

s = "cafe\u0301 \U0001f44d\U0001f3fd"  # e + combining acute, thumbs up + skin tone
print(len(s), len(s.encode("utf-8")), len(s.encode("utf-16-le")) // 2)
print([f"U+{ord(c):04X}" for c in s[3:5]])

nfc = unicodedata.normalize("NFC", s)  # compose e + accent into one code point
print(len(nfc), unicodedata.name(nfc[3]))

print("Stra\u00dfe".upper(), "Stra\u00dfe".casefold() == "STRASSE".casefold())
print(bytes([0x63, 0x61, 0x66, 0xC3, 0xA9]).decode("utf-8"))
print(b"caf\xe9".decode("utf-8", errors="replace") == "caf\ufffd")
```

```output for=code-points-and-encodings
8 15 10
['U+0065', 'U+0301']
7 LATIN SMALL LETTER E WITH ACUTE
STRASSE True
café
True
```

```python wrong id=unnormalized-compare
typed = "caf\u00e9"  # one code point for the accented letter
pasted = "cafe\u0301"  # e followed by a combining accent
print(typed == pasted, len(typed), len(pasted))
print({typed: 1}.get(pasted))
```

```output for=unnormalized-compare
False 4 5
None
```

- Verdict: native. A Python `str` is a sequence of code points, so `len` counts code points: 8 for the sample, which a reader sees as 6 characters. `encode` gives the bytes of an encoding, 15 in UTF-8 and 10 UTF-16 code units. `unicodedata.normalize` converts between the composed and decomposed forms, and `casefold` gives a form for comparing text without case, in which `ß` matches `SS`. The standard library has no function for grapheme clusters; the third-party `regex` module supports `\X`.
- Decoding bytes that are not valid UTF-8 raises an error by default; `errors="replace"` puts U+FFFD in place of each bad sequence.
- The wrong block compares a word typed as one code point with the same word pasted with a combining accent. They look the same but are different strings, so `==` and dictionary lookups fail. Normalize text to NFC when it enters the program.
