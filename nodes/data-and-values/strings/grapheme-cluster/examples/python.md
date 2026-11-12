```python run id=emoji-code-points-and-nfc
import unicodedata

# The names of the code points in one emoji grapheme.
for c in "\U0001F469\U0001F3FD\u200d\U0001F692":
    print(f"U+{ord(c):04X} {unicodedata.name(c)}")

# NFC can join "e" and U+0301 into one code point, but most graphemes have no such form.
e = "e\u0301"
print(len(e), len(unicodedata.normalize("NFC", e)))
flag = "\U0001F1EB\U0001F1F7"
print(len(flag), len(unicodedata.normalize("NFC", flag)))

# A cut by code points can split a grapheme. Here the accent is lost.
word = "cafe\u0301"
print(word[:4], len(word[:4]))

# A Tamil syllable: the vowel sign is a spacing mark, so it is a separate code point.
ni = "\u0BA8\u0BBF"
print(ni, len(ni), [unicodedata.category(c) for c in ni])
```

```output for=emoji-code-points-and-nfc
U+1F469 WOMAN
U+1F3FD EMOJI MODIFIER FITZPATRICK TYPE-4
U+200D ZERO WIDTH JOINER
U+1F692 FIRE ENGINE
2 1
2 2
cafe 4
நி 2 ['Lo', 'Mc']
```

- Verdict: stdlib. The firefighter emoji is four code points, and `unicodedata.name` names each one. NFC joins `e` and U+0301 into one code point, but a flag stays two code points. A cut of 4 code points loses the accent. The Tamil syllable is a letter and a spacing mark (`Mc`), so it is one extended cluster but two code points. Python 3.15 adds `unicodedata.iter_graphemes`, and the example does not use it, so it runs on older versions too.
