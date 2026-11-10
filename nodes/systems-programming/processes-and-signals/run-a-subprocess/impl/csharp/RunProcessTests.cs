// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

using System.ComponentModel;
using Xunit;

public sealed class RunProcessTests
{
    public static TheoryData<string, string[], int, string, string> Cases => new()
    {
        { "hello", ["printf", "hello"], 0, "hello", "" },
        { "two-arguments", ["echo", "one", "two"], 0, "one two\n", "" },
        { "exit-status", ["sh", "-c", "exit 3"], 3, "", "" },
        { "both-streams", ["sh", "-c", "echo out; echo err >&2; exit 2"], 2, "out\n", "err\n" },
        { "metacharacters", ["printf", "%s|", "a b", "$HOME", "*", "x;y", "\"q\"", "'s'"], 0, "a b|$HOME|*|x;y|\"q\"|'s'|", "" },
        { "empty-argument", ["printf", "[%s]", ""], 0, "[]", "" },
        { "newline-argument", ["printf", "%s", "line one\nline two"], 0, "line one\nline two", "" },
        { "trailing-newlines", ["printf", "a\\n\\n"], 0, "a\n\n", "" },
        { "non-ascii", ["printf", "%s\\n", "xin chào, 日本語, 🙂"], 0, "xin chào, 日本語, 🙂\n", "" },
        { "stderr-only", ["sh", "-c", "echo warning >&2"], 0, "", "warning\n" },
        { "no-output", ["true"], 0, "", "" },
        { "false", ["false"], 1, "", "" },
        { "status-255", ["sh", "-c", "exit 255"], 255, "", "" },
        { "empty-stdin", ["sh", "-c", "if read -r line; then echo got; else echo eof; fi"], 0, "eof\n", "" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void RunsTheProgram(string label, string[] argv, int status, string stdout, string stderr)
    {
        Assert.True((status, stdout, stderr) == RunProcess.Run(argv), label);
    }

    [Fact]
    public void LargeOutputOnBothStreams()
    {
        var script = "head -c 1048576 /dev/zero | tr '\\0' x; head -c 1048576 /dev/zero | tr '\\0' y >&2";
        var (status, stdout, stderr) = RunProcess.Run(["sh", "-c", script]);
        Assert.Equal(0, status);
        Assert.Equal(new string('x', 1048576), stdout);
        Assert.Equal(new string('y', 1048576), stderr);
    }

    [Fact]
    public void MissingProgram()
    {
        Assert.Throws<Win32Exception>(() => RunProcess.Run(["no-such-program-7f3a"]));
    }
}
