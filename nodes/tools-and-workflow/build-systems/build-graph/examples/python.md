```python run id=graph
# A build graph: a topological order, the waves of a parallel build, and a cycle check.
import heapq

RULES = {
    "app": ["main.o", "parse.o", "lex.o"],
    "main.o": ["main.c", "parse.h"],
    "parse.o": ["parse.c", "parse.h", "lex.h"],
    "lex.o": ["lex.c", "lex.h"],
    "parse.c": ["grammar.y"],
    "parse.h": ["grammar.y"],
}


def nodes(rules):
    names = set(rules)
    for deps in rules.values():
        names.update(deps)
    return sorted(names)


def topological_order(rules):
    # Kahn's algorithm: a node is ready when all its dependencies are done.
    waiting = {n: len(rules.get(n, [])) for n in nodes(rules)}
    users = {n: [] for n in waiting}
    for target, deps in rules.items():
        for dep in deps:
            users[dep].append(target)
    ready = [n for n, count in waiting.items() if count == 0]
    heapq.heapify(ready)
    order = []
    while ready:
        n = heapq.heappop(ready)
        order.append(n)
        for user in users[n]:
            waiting[user] -= 1
            if waiting[user] == 0:
                heapq.heappush(ready, user)
    return order


def waves(rules):
    level = {}
    for n in topological_order(rules):
        deps = rules.get(n, [])
        level[n] = 1 + max(level[d] for d in deps) if deps else 0
    out = {}
    for n, k in level.items():
        if k > 0:
            out.setdefault(k, []).append(n)
    return [sorted(out[k]) for k in sorted(out)]


def find_cycle(rules):
    state = {}  # absent: not seen, 1: on the path, 2: done
    path = []

    def visit(n):
        state[n] = 1
        path.append(n)
        for dep in rules.get(n, []):
            if state.get(dep) == 1:
                return path[path.index(dep):] + [dep]
            if dep not in state:
                found = visit(dep)
                if found:
                    return found
        path.pop()
        state[n] = 2
        return None

    for n in nodes(rules):
        if n not in state:
            found = visit(n)
            if found:
                return found
    return None


print("order:", " ".join(topological_order(RULES)))
for i, wave in enumerate(waves(RULES), 1):
    print("wave " + str(i) + ":", " ".join(wave))
found = find_cycle(RULES)
print("cycle:", " -> ".join(found) if found else "none")
bad = dict(RULES, **{"lex.h": ["lex.o"]})
print("after lex.h <- lex.o:")
print("order:", " ".join(topological_order(bad)), "(" + str(len(topological_order(bad))) + " of " + str(len(nodes(bad))) + " nodes)")
print("cycle:", " -> ".join(find_cycle(bad)))
```

```output
order: grammar.y lex.c lex.h lex.o main.c parse.c parse.h main.o parse.o app
wave 1: lex.o parse.c parse.h
wave 2: main.o parse.o
wave 3: app
cycle: none
after lex.h <- lex.o:
order: grammar.y lex.c main.c parse.c parse.h main.o (6 of 10 nodes)
cycle: lex.h -> lex.o -> lex.h
```

- Verdict: idiom. A dict maps each target to the list of its dependencies, and `heapq` keeps the ready nodes, so the node with the smallest name comes out first.
- `waves` gives each node a level: 0 for an input, and one more than the deepest dependency for a target. The targets with the same level form one wave.
- `find_cycle` marks a node 1 while it is on the current path and 2 when it is done. An edge to a node marked 1 closes a cycle.
