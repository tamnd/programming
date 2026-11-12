```python run id=append-depth-and-rebalance
# Appending one piece at a time makes a deep, thin tree. A rebalance rebuilds it.
class Leaf:
    def __init__(self, s):
        self.s, self.n, self.depth = s, len(s), 0

class Node:
    def __init__(self, left, right):
        self.left, self.right = left, right
        self.n = left.n + right.n
        self.depth = 1 + max(left.depth, right.depth)

def leaves(r):
    if isinstance(r, Leaf):
        yield r
    else:
        yield from leaves(r.left)
        yield from leaves(r.right)

def balance(r):
    # Collect the leaves and join them in pairs, from the bottom up.
    level = list(leaves(r))
    while len(level) > 1:
        pairs = [Node(level[k], level[k + 1]) for k in range(0, len(level) - 1, 2)]
        if len(level) % 2 == 1:
            pairs.append(level[-1])
        level = pairs
    return level[0]

def index(r, i):
    # Return the character and the number of nodes on the path.
    steps = 0
    while isinstance(r, Node):
        steps += 1
        if i < r.left.n:
            r = r.left
        else:
            i, r = i - r.left.n, r.right
    return r.s[i], steps

pieces = [chr(ord("a") + k % 26) for k in range(64)]
r = Leaf(pieces[0])
for p in pieces[1:]:
    r = Node(r, Leaf(p))
print(r.n, r.depth, *index(r, 0))

b = balance(r)
print(b.n, b.depth, *index(b, 0))
print("".join(leaf.s for leaf in leaves(b)) == "".join(pieces), index(b, 40)[0])
```

```output for=append-depth-and-rebalance
64 63 a 63
64 6 a 6
True o
```

- Verdict: idiom. Python has no rope in its standard library. Each of the 64 appends puts the old rope on the left of a new node, so the depth is 63, and `index(r, 0)` passes 63 nodes. `balance` collects the leaves and joins them in pairs, so the depth falls to 6 and the same index passes 6 nodes. The leaves and their order do not change.
