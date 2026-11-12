```python run id=capitalize-title-and-casefold
# capitalize changes the first character, and title changes each word.
s = "they're bill's friends"
print(s.capitalize())
print(s.title())

# Case mappings are not reversible.
name = "McGowan"
print(name.upper().lower(), name.lower().title())

# casefold is more than lower: it also folds "ß" and the ligature "ﬁ".
for w in ["Straße", "ﬁle", "ΌΣΟΣ"]:
    print(w, w.lower(), w.casefold())

# Python has no Turkish rules. A program can map i and ı first, then call upper.
tr_upper = str.maketrans({"i": "İ", "ı": "I"})
print("istanbul ılık".translate(tr_upper).upper())

# Caseless characters stay as they are, so isupper looks only at the cased ones.
print("ABC 123".isupper(), "日本".upper() == "日本")
```

```output for=capitalize-title-and-casefold
They're bill's friends
They'Re Bill'S Friends
mcgowan Mcgowan
Straße straße strasse
ﬁle ﬁle file
ΌΣΟΣ όσος όσοσ
İSTANBUL ILIK
True True
```

- Verdict: native. `capitalize` changes only the first character, and `title` starts a new word after each apostrophe, so it gives `They'Re`. After `upper` or `lower`, `McGowan` cannot come back. `lower` keeps `ß` and `ﬁ`, and `casefold` maps them to `ss` and `fi`. `lower` gives the final sigma `ς`, and `casefold` maps it to `σ`. Python has no Turkish rules, so the example maps `i` and `ı` with `translate` before it calls `upper`. `isupper` looks only at cased characters, and `upper` does not change `日本`.
