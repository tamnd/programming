```csharp run id=memberwise-clone-and-with
using System;
using System.Collections.Generic;

var a = new Team("red", new List<string> { "Ann", "Bo" });
var shallow = a.ShallowCopy();
var deep = a.DeepCopy();

a.Members.Add("Cy");
Console.WriteLine(string.Join(", ", shallow.Members));
Console.WriteLine(string.Join(", ", deep.Members));
Console.WriteLine(ReferenceEquals(shallow.Members, a.Members));

var p = new Player("Ann", new List<int> { 3 });
var q = p with { Name = "Bo" };
q.Scores.Add(5);
Console.WriteLine($"{p.Name} {p.Scores.Count}");

sealed class Team
{
    public string Name;
    public List<string> Members;

    public Team(string name, List<string> members)
    {
        Name = name;
        Members = members;
    }

    // MemberwiseClone copies each field; the list is shared.
    public Team ShallowCopy() => (Team)MemberwiseClone();

    public Team DeepCopy() => new Team(Name, new List<string>(Members));
}

record Player(string Name, List<int> Scores);
```

```output for=memberwise-clone-and-with
Ann, Bo, Cy
Ann, Bo
True
Ann 2
```

- Verdict: native. `Object.MemberwiseClone` is protected, so a class wraps it in a public method; the copy shares the objects that the fields refer to.
- The deep copy is ordinary code that makes a new list.
- A `with` expression on a record is also a shallow copy: `q` has a new name, but it shares the list of scores with `p`.
