// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

using System;
using System.Text;

/// <summary>A setting in a configuration file is bad.</summary>
public sealed class ConfigException : Exception
{
    public ConfigException(string message, Exception inner)
        : base(message, inner)
    {
    }
}

/// <summary>Reads a port setting and wraps the error of the parser with the place of the setting.</summary>
public static class SettingChecker
{
    /// <summary>Returns the port number in text, or throws FormatException.</summary>
    public static int ParsePort(string text)
    {
        foreach (char c in text)
        {
            if (c < '0' || c > '9')
            {
                throw new FormatException($"not a number: '{text}'");
            }
        }
        if (text.Length == 0)
        {
            throw new FormatException($"not a number: '{text}'");
        }
        int port = 0;
        foreach (char c in text)
        {
            port = port * 10 + (c - '0');
            if (port > 65535)
            {
                throw new FormatException($"out of range: {text}");
            }
        }
        if (port == 0)
        {
            throw new FormatException($"out of range: {text}");
        }
        return port;
    }

    /// <summary>Returns the port number, or throws ConfigException with the FormatException as InnerException.</summary>
    public static int ReadPort(string where, string text)
    {
        try
        {
            return ParsePort(text);
        }
        catch (FormatException e)
        {
            throw new ConfigException($"{where}: read port", e);
        }
    }

    /// <summary>Joins the message of e and the messages of its inner exceptions.</summary>
    public static string Chain(Exception e)
    {
        var sb = new StringBuilder(e.Message);
        for (Exception? inner = e.InnerException; inner != null; inner = inner.InnerException)
        {
            sb.Append(": ").Append(inner.Message);
        }
        return sb.ToString();
    }

    /// <summary>Returns "port N", or "error: " and the chain of messages.</summary>
    public static string CheckSetting(string where, string text)
    {
        try
        {
            return $"port {ReadPort(where, text)}";
        }
        catch (ConfigException e)
        {
            return $"error: {Chain(e)}";
        }
    }
}
