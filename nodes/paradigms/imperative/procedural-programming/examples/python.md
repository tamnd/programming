```python run id=decompose
def split_words(text):
    return text.lower().split()


def count_words(words):
    counts = {}
    for w in words:
        counts[w] = counts.get(w, 0) + 1
    return counts


def top(counts, n):
    return sorted(counts.items(), key=lambda kv: (-kv[1], kv[0]))[:n]


def report(text, n):
    for word, c in top(count_words(split_words(text)), n):
        print(f"{word:5} {c}")


report("The cat and the hat and the bat", 2)
```

```output for=decompose
the   3
and   2
```

```python wrong id=global-state
total = 0


def add_all(xs):
    global total
    for x in xs:
        total += x
    return total


print(add_all([1, 2, 3]))
print(add_all([1, 2, 3]))
```

```output for=global-state
6
12
```

- Verdict: native. `def` defines a function, and a module groups functions in a file. A function can be called before it is defined in the text, as long as the definition has run before the call.
- The tutorial explains that a function cannot assign to a global variable unless the variable is "named in a global statement". The rule makes writes to shared state visible in the code.
- In the wrong block, `add_all` keeps its sum in a global variable, so two calls with the same argument return different results. Use a local variable and return it.
