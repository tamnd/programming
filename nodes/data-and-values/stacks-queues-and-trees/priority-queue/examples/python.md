```python run id=pq
import heapq

tasks = []
for priority, name in [(3, "update docs"), (1, "fix outage"), (2, "review patch"), (4, "plan sprint")]:
    heapq.heappush(tasks, (priority, name))

print("peek:", tasks[0][1])
while tasks:
    priority, name = heapq.heappop(tasks)
    print(priority, name)

max_heap = [-n for n in [3, 1, 2, 4]]
heapq.heapify(max_heap)
order = []
while max_heap:
    order.append(-heapq.heappop(max_heap))
print("largest first:", *order)
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: stdlib. `heapq` keeps a plain list in min-heap order. `heappush` and `heappop` take logarithmic time, and `tasks[0]` is the smallest item.
- A tuple sorts by its first item, so `(priority, name)` puts the lowest priority first.
- This example negates the numbers for a max-heap, because the max-heap functions need Python 3.14.
