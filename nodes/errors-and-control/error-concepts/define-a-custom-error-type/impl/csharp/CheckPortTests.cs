// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

using Xunit;

public sealed class CheckPortTests
{
    public static TheoryData<string, string, string> Cases => new()
    {
        { "valid", "8080", "port 8080" },
        { "not-a-number", "x80", "invalid port 'x80': not a number" },
        { "too-large", "65536", "invalid port '65536': out of range" },
        { "empty", "", "invalid port '': empty" },
        { "zero", "0", "invalid port '0': out of range" },
        { "one", "1", "port 1" },
        { "largest", "65535", "port 65535" },
        { "leading-zeros", "0080", "port 80" },
        { "negative", "-1", "invalid port '-1': not a number" },
        { "leading-space", " 80", "invalid port ' 80': not a number" },
        { "overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string text, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, PortChecker.CheckPort(text));
    }

    [Fact]
    public void Fields()
    {
        var e = Assert.Throws<PortException>(() => PortChecker.ParsePort("x80"));
        Assert.Equal("x80", e.Text);
        Assert.Equal("not a number", e.Reason);
    }
}
