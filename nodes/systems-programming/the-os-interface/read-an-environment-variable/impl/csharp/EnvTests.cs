// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

using Xunit;

// xUnit runs the tests of one class one after the other, so the tests can
// change the environment of the process.
public sealed class EnvTests
{
    public static TheoryData<string, string?, string?, string, string, string> Cases => new()
    {
        { "present", "APP_PORT", "8080", "APP_PORT", "80", "8080" },
        { "path-value", "APP_HOME", "/srv/app", "APP_HOME", "/opt/app", "/srv/app" },
        { "missing", null, null, "APP_PORT", "80", "80" },
        { "empty-value", "APP_MODE", "", "APP_MODE", "production", "" },
        { "empty-default", null, null, "APP_MODE", "", "" },
        { "spaces", "APP_TITLE", "  two  spaces  ", "APP_TITLE", "none", "  two  spaces  " },
        { "equals", "APP_QUERY", "key=value=more", "APP_QUERY", "none", "key=value=more" },
        { "newline", "APP_TEXT", "first\nsecond", "APP_TEXT", "none", "first\nsecond" },
        { "non-ascii", "APP_GREETING", "xin chào, 日本語, 🙂", "APP_GREETING", "none", "xin chào, 日本語, 🙂" },
        { "case", "APP_PORT", "8080", "app_port", "80", "80" },
        { "inherited-name", null, null, "toString", "none", "none" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ReturnsTheValueOrTheDefault(string label, string? key, string? value, string name, string defaultValue, string want)
    {
        // A null value removes the variable.
        Environment.SetEnvironmentVariable(name, null);
        if (key is not null)
        {
            Environment.SetEnvironmentVariable(key, value);
        }
        try
        {
            Assert.True(want == Env.GetEnv(name, defaultValue), label);
        }
        finally
        {
            if (key is not null)
            {
                Environment.SetEnvironmentVariable(key, null);
            }
        }
    }
}
