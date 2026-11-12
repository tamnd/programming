```csharp run id=leaf-size-and-report
using System.Text;

// Build a balanced rope from a long string: cut it into leaves of at most 8 chars.
const int MaxLeaf = 8;
var s = string.Concat(Enumerable.Range(0, 100).Select(k => (char)('a' + k % 26)));
Rope r = Build(s, 0, s.Length);
Console.WriteLine($"{r.Length} {Leaves(r)} {r.Depth}");

// Report(i, j) copies only the leaves that hold characters i to j - 1.
var sb = new StringBuilder();
int visited = Report(r, 40, 50, sb);
Console.WriteLine($"{sb} {visited} {sb.ToString() == s[40..50]}");

Rope Build(string text, int a, int b) =>
    b - a <= MaxLeaf ? new Leaf(text[a..b]) : new Node(Build(text, a, (a + b) / 2), Build(text, (a + b) / 2, b));

int Leaves(Rope t) => t is Node n ? Leaves(n.L) + Leaves(n.R) : 1;

int Report(Rope t, int i, int j, StringBuilder output)
{
    if (j <= 0 || i >= t.Length) return 0;
    if (t is Leaf leaf)
    {
        int a = Math.Max(i, 0), b = Math.Min(j, leaf.S.Length);
        output.Append(leaf.S, a, b - a);
        return 1;
    }
    var node = (Node)t;
    return Report(node.L, i, j, output) + Report(node.R, i - node.L.Length, j - node.L.Length, output);
}

abstract record Rope(int Length, int Depth);
record Leaf(string S) : Rope(S.Length, 0);
record Node(Rope L, Rope R) : Rope(L.Length + R.Length, 1 + Math.Max(L.Depth, R.Depth));
```

```output for=leaf-size-and-report
100 16 4
opqrstuvwx 2 True
```

- Verdict: idiom. .NET has no rope type. `Build` cuts 100 characters into 16 leaves of at most 8 characters, in a tree of depth 4. `Report(40, 50)` skips every subtree outside the range and copies from only 2 leaves.
