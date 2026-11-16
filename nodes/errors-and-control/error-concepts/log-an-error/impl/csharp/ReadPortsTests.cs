// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

using System.IO;
using Xunit;

public sealed class ReadPortsTests
{
    public static TheoryData<string, string[], int[], string> Cases => new()
    {
        { "one-bad", new string[] { "8080", "x", "443" }, new int[] { 8080, 443 }, "level=ERROR msg=\"read port failed\" line=2 err=\"not a number: 'x'\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n" },
        { "all-good", new string[] { "80", "443" }, new int[] { 80, 443 }, "level=INFO msg=\"ports read\" ok=2 total=2\n" },
        { "out-of-range", new string[] { "70000" }, new int[0], "level=ERROR msg=\"read port failed\" line=1 err=\"out of range: 70000\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n" },
        { "empty-list", new string[0], new int[0], "level=INFO msg=\"ports read\" ok=0 total=0\n" },
        { "empty-text", new string[] { "" }, new int[0], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: ''\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n" },
        { "boundary", new string[] { "1", "65535", "0" }, new int[] { 1, 65535 }, "level=ERROR msg=\"read port failed\" line=3 err=\"out of range: 0\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n" },
        { "newline", new string[] { "80\nlevel=INFO msg=\"all good\"" }, new int[0], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\nlevel=INFO msg=\\\"all good\\\"'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n" },
        { "backslash", new string[] { "C:\\port" }, new int[0], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: 'C:\\\\port'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n" },
        { "carriage-return", new string[] { "80\r" }, new int[0], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\r'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n" },
        { "tab", new string[] { "8\t0" }, new int[0], "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '8\\t0'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Read(string name, string[] texts, int[] wantPorts, string wantLog)
    {
        Assert.NotNull(name);
        var log = new StringWriter();
        Assert.Equal(wantPorts, PortReader.ReadPorts(texts, log));
        Assert.Equal(wantLog, log.ToString());
    }
}
