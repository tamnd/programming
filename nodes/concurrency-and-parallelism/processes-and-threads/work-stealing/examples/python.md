```python run id=work-stealing-model
from collections import deque

# A step-by-step model of a work-stealing scheduler with two workers.
# Each worker has a deque. The left end is the top (oldest task); the right end is the bottom (newest task).
# A task is a range [lo, hi). A range of more than 2 numbers splits into two halves; a smaller range is a leaf that adds its numbers.
WORKERS = 2
deques = [deque() for _ in range(WORKERS)]
leaves = [0] * WORKERS
total = 0

deques[0].append((0, 8))
round_number = 0
while any(deques):
    round_number += 1
    for w in range(WORKERS):
        if deques[w]:
            lo, hi = deques[w].pop()  # the owner takes the newest task (LIFO)
            how, source = "pops", ""
        else:
            victim = next((v for v in range(WORKERS) if v != w and deques[v]), None)
            if victim is None:
                print(f"round {round_number}: w{w} is idle")
                continue
            lo, hi = deques[victim].popleft()  # a thief takes the oldest task (FIFO)
            how, source = "steals", f" from w{victim}"
        if hi - lo > 2:
            mid = (lo + hi) // 2
            deques[w].append((lo, mid))
            deques[w].append((mid, hi))
            print(f"round {round_number}: w{w} {how} [{lo}, {hi}){source} and splits it")
        else:
            total += sum(range(lo, hi))
            leaves[w] += 1
            print(f"round {round_number}: w{w} {how} [{lo}, {hi}){source} and runs it")

print("leaves run:", ", ".join(f"w{w} {n}" for w, n in enumerate(leaves)))
print("sum:", total)
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
```

- Verdict: absent. The Python standard library has no work-stealing scheduler. This file is a model: it shows the rules step by step, with two lists in place of two threads.
- Each worker takes its own newest task with `pop`, from the right end. A thief takes the oldest task of another worker with `popleft`, from the left end.
- In round 1, worker w1 has no work, so it steals `[0, 4)`, the oldest and largest task of w0. After that, both workers split and run their own tasks, and they do not touch the same deque.
