// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

using System;

/// <summary>Reports that a text is not a valid TCP port number.</summary>
public sealed class PortException : Exception
{
    public PortException(string text, string reason)
        : base($"invalid port '{text}': {reason}")
    {
        Text = text;
        Reason = reason;
    }

    public string Text { get; }

    public string Reason { get; }
}

/// <summary>Reads a TCP port number and reports a bad one with PortException.</summary>
public static class PortChecker
{
    /// <summary>Returns the port number in text, or throws PortException.</summary>
    public static int ParsePort(string text)
    {
        if (text.Length == 0)
        {
            throw new PortException(text, "empty");
        }
        foreach (char c in text)
        {
            if (c < '0' || c > '9')
            {
                throw new PortException(text, "not a number");
            }
        }
        int port = 0;
        foreach (char c in text)
        {
            port = port * 10 + (c - '0');
            if (port > 65535)
            {
                throw new PortException(text, "out of range");
            }
        }
        if (port == 0)
        {
            throw new PortException(text, "out of range");
        }
        return port;
    }

    /// <summary>Returns "port N", or the message of the PortException.</summary>
    public static string CheckPort(string text)
    {
        try
        {
            return $"port {ParsePort(text)}";
        }
        catch (PortException e)
        {
            return e.Message;
        }
    }
}
