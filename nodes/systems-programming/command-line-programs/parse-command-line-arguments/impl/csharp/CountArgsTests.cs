// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

using Xunit;

public sealed class CountArgsTests
{
    public static TheoryData<string, string[], bool, int, string[]> Cases => new()
    {
        { "files-only", ["a.txt", "b.txt"], false, 10, ["a.txt", "b.txt"] },
        { "short-options", ["-v", "-n", "3", "a.txt"], true, 3, ["a.txt"] },
        { "long-options", ["--verbose", "--limit", "5", "a.txt", "b.txt"], true, 5, ["a.txt", "b.txt"] },
        { "no-args", [], false, 10, [] },
        { "long-equals", ["--limit=7", "notes.md"], false, 7, ["notes.md"] },
        { "repeated", ["-n", "3", "--limit", "5", "a.txt"], false, 5, ["a.txt"] },
        { "zero", ["-n", "0", "a.txt"], false, 0, ["a.txt"] },
        { "end-of-options", ["-v", "--", "-n", "--limit=4"], true, 10, ["-n", "--limit=4"] },
        { "only-terminator", ["--"], false, 10, [] },
        { "second-terminator", ["--", "--"], false, 10, ["--"] },
        { "dash", ["-"], false, 10, ["-"] },
        { "dash-after-option", ["-n", "2", "-", "b.txt"], false, 2, ["-", "b.txt"] },
        { "empty-arg", [""], false, 10, [""] },
        { "space", ["-v", "my file.txt"], true, 10, ["my file.txt"] },
        { "non-ascii", ["--limit=4", "日本語.txt", "café.md"], false, 4, ["日本語.txt", "café.md"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Parses(string name, string[] args, bool verbose, int limit, string[] files)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(new CountArgs(verbose, limit, files), CountArgs.Parse(args));
    }

    public static TheoryData<string, string[]> Errors => new()
    {
        { "unknown-short", ["-x", "a.txt"] },
        { "unknown-long", ["--color", "a.txt"] },
        { "missing-value", ["-n"] },
        { "missing-value-long", ["--limit"] },
        { "not-a-number", ["-n", "ten", "a.txt"] },
        { "not-a-number-long", ["--limit=ten"] },
        { "empty-value", ["--limit=", "a.txt"] },
        { "verbose-with-value", ["--verbose=yes"] },
    };

    [Theory]
    [MemberData(nameof(Errors))]
    public void Rejects(string name, string[] args)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Throws<UsageException>(() => CountArgs.Parse(args));
    }
}
