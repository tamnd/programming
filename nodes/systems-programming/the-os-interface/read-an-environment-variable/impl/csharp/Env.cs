// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

public static class Env
{
    // Returns the value of the environment variable name, or defaultValue when
    // it is not set. An empty value is a value, so it does not give defaultValue.
    public static string GetEnv(string name, string defaultValue) =>
        // GetEnvironmentVariable returns null when the variable is not set.
        Environment.GetEnvironmentVariable(name) ?? defaultValue;
}
