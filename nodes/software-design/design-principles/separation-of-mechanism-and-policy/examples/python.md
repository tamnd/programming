```python run id=task-policy
def run_all(tasks, policy):
    """Mechanism: runs the tasks; it does not decide the order."""
    queue = list(tasks)
    done = []
    while queue:
        done.append(queue.pop(policy(queue))["name"])
    return " ".join(done)


# Policies: each one decides which task runs next.
def first_come(queue):
    return 0


def shortest_first(queue):
    return min(range(len(queue)), key=lambda i: queue[i]["time"])


tasks = [
    {"name": "backup", "time": 30},
    {"name": "email", "time": 2},
    {"name": "report", "time": 10},
]
print(f"first come: {run_all(tasks, first_come)}")
print(f"shortest first: {run_all(tasks, shortest_first)}")
```

```output
first come: backup email report
shortest first: email report backup
```
