// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

using Xunit;

public sealed class UniqueTests
{
    public static TheoryData<string, string[], string[]> Cases => new()
    {
        { "repeats", ["apple", "pear", "apple", "fig", "pear"], ["apple", "pear", "fig"] },
        { "words", ["to", "be", "or", "not", "to", "be"], ["to", "be", "or", "not"] },
        { "empty", [], [] },
        { "single", ["a"], ["a"] },
        { "all-same", ["x", "x", "x"], ["x"] },
        { "no-duplicates", ["c", "a", "b"], ["c", "a", "b"] },
        { "not-adjacent", ["a", "b", "a", "b"], ["a", "b"] },
        { "adjacent", ["a", "a", "b", "b", "a"], ["a", "b"] },
        { "empty-string", ["", "a", "", "b"], ["", "a", "b"] },
        { "case", ["a", "A", "a"], ["a", "A"] },
        { "normalization", ["\u00e9", "e\u0301", "\u00e9"], ["\u00e9", "e\u0301"] },
        { "whitespace", [" a", "a", "a ", " a"], [" a", "a", "a "] },
        { "number-like", ["1", "01", "1.0", "1"], ["1", "01", "1.0"] },
        { "non-ascii", ["日本", "🙂", "日本", "🙂", "ü"], ["日本", "🙂", "ü"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void KeepsTheFirstCopies(string name, string[] given, string[] want)
    {
        string[] input = [.. given];
        Assert.True(want.SequenceEqual(Dedupe.Unique(input)), name);
        Assert.Equal(given, input);
    }

    private static User[] U(params (string Name, string Email)[] rows) =>
        rows.Select(r => new User(r.Name, r.Email)).ToArray();

    // The cases of cases/by-key.jsonl.
    public static TheoryData<string, User[], User[]> ByKey => new()
    {
        { "case-insensitive",
          U(("Ada", "ada@example.com"), ("Bob", "bob@example.com"), ("Ada L.", "ADA@Example.com")),
          U(("Ada", "ada@example.com"), ("Bob", "bob@example.com")) },
        { "empty", U(), U() },
        { "same-name-other-email",
          U(("Sam", "sam@a.example"), ("Sam", "sam@b.example")),
          U(("Sam", "sam@a.example"), ("Sam", "sam@b.example")) },
        { "first-record-wins",
          U(("Old", "x@example.com"), ("New", "X@EXAMPLE.COM")),
          U(("Old", "x@example.com")) },
        { "dots-differ",
          U(("Al", "a.b@example.com"), ("Bo", "ab@example.com")),
          U(("Al", "a.b@example.com"), ("Bo", "ab@example.com")) },
        { "not-adjacent",
          U(("A", "a@example.com"), ("B", "b@example.com"), ("C", "A@example.com"), ("D", "B@EXAMPLE.com")),
          U(("A", "a@example.com"), ("B", "b@example.com")) },
    };

    [Theory]
    [MemberData(nameof(ByKey))]
    public void KeepsTheFirstUserForEachEmail(string name, User[] given, User[] want)
    {
        Assert.True(want.SequenceEqual(Dedupe.UniqueByEmail(given)), name);
    }
}
