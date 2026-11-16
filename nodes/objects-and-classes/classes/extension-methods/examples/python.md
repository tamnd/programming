```python run id=plain-function-or-subclass
def word_count(text):  # a plain function: the usual answer in Python
    return len(text.split())


print(word_count("Hello plain functions"))
try:
    str.word_count = word_count  # a built-in type cannot get new attributes
except TypeError as e:
    print(e)


class Text(str):  # a subclass can add the method
    def word_count(self):
        return len(self.split())


print(Text("one two").word_count(), Text("x").upper())
```

```output for=plain-function-or-subclass
3
cannot set 'word_count' attribute of immutable type 'str'
2 X
```

- Verdict: absent. Python has no extension methods. A plain function is the usual answer, and a user-defined class can get a new method at run time, which is monkey patching.
- A built-in type such as `str` is immutable, so the assignment fails. A subclass such as `Text` adds the method and keeps the methods of `str`, but only for values made as `Text`.
