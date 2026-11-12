```python run id=thread-confinement
import threading

WORDS = ["a", "b", "c", "d", "e"]
chunks = [WORDS * 2000 for _ in range(4)]  # 10000 words in each chunk
results = [None] * len(chunks)


def count(index, chunk):
    counts = {}  # local: only this thread can reach it, so no lock
    for word in chunk:
        counts[word] = counts.get(word, 0) + 1
    results[index] = counts  # hand the finished object over


threads = [threading.Thread(target=count, args=(i, c)) for i, c in enumerate(chunks)]
for t in threads:
    t.start()
for t in threads:
    t.join()  # after join, the main thread owns the results

total = {}
for counts in results:
    for word, n in counts.items():
        total[word] = total.get(word, 0) + n
print("chunks:", len(chunks))
print("words counted:", sum(total.values()))
print("distinct words:", len(total))
print("count of 'a':", total["a"])
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: idiom. Each thread fills a local `counts` dictionary, and `join` makes the main thread the only owner of the results.
