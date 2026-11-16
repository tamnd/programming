// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

using System.Linq;
using Xunit;

public sealed class BirthdayTests
{
    public static TheoryData<string, string[], int[], string, int[]> Cases => new()
    {
        { "first", new[] { "Ada", "Alan" }, new[] { 36, 41 }, "Ada", new[] { 37, 41 } },
        { "second", new[] { "Ada", "Alan" }, new[] { 36, 41 }, "Alan", new[] { 36, 42 } },
        { "empty", new string[0], new int[0], "Ada", new int[0] },
        { "not-found", new[] { "Ada" }, new[] { 36 }, "Grace", new[] { 36 } },
        { "duplicate-name", new[] { "Ada", "Alan", "Ada" }, new[] { 36, 41, 9 }, "Ada", new[] { 37, 41, 10 } },
        { "zero-age", new[] { "Baby" }, new[] { 0 }, "Baby", new[] { 1 } },
        { "non-ascii", new[] { "Zoë", "Zoe" }, new[] { 20, 30 }, "Zoë", new[] { 21, 30 } },
        { "case", new[] { "ada" }, new[] { 36 }, "Ada", new[] { 36 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] names, int[] ages, string who, int[] wantAges)
    {
        Assert.NotNull(name);
        var people = names.Zip(ages, (n, a) => new Person(n, a)).ToList();
        var before = people.ToList();
        var got = Birthday.Apply(people, who);
        Assert.Equal(names, got.Select(p => p.Name));
        Assert.Equal(wantAges, got.Select(p => p.Age));
        Assert.Equal(before, people);
    }

    [Fact]
    public void EqualityAndPrinting()
    {
        Assert.Equal(new Person("Ada", 36), new Person("Ada", 36));
        Assert.NotEqual(new Person("Ada", 36), new Person("Ada", 37));
        Assert.Equal("Person { Name = Ada, Age = 36 }", new Person("Ada", 36).ToString());
    }
}
