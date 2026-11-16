// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

using System.Collections.Generic;
using System.Linq;

// A positional record gets init-only properties, value equality and a
// ToString that prints the properties.
public sealed record Person(string Name, int Age);

public static class Birthday
{
    // Returns a new list where each person called name is one year older. The
    // with expression copies the record and changes one property.
    public static List<Person> Apply(IEnumerable<Person> people, string name) =>
        people.Select(p => p.Name == name ? p with { Age = p.Age + 1 } : p).ToList();
}
