// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

/// <summary>Reads port settings and logs the bad ones.</summary>
public static class PortReader
{
    /// <summary>Returns the port number in text, or throws FormatException.</summary>
    public static int ParsePort(string text)
    {
        if (text.Length == 0)
        {
            throw new FormatException($"not a number: '{text}'");
        }
        foreach (char c in text)
        {
            if (c < '0' || c > '9')
            {
                throw new FormatException($"not a number: '{text}'");
            }
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

    /// <summary>Puts value in double quotes and escapes the characters that could end the line or the value.</summary>
    public static string Quote(string value)
    {
        var sb = new StringBuilder("\"");
        foreach (char c in value)
        {
            sb.Append(c switch
            {
                '\\' => "\\\\",
                '"' => "\\\"",
                '\n' => "\\n",
                '\r' => "\\r",
                '\t' => "\\t",
                _ => c.ToString(),
            });
        }
        return sb.Append('"').ToString();
    }

    /// <summary>
    /// Returns the good ports. Writes a log line to log for each bad text, then a summary.
    /// Each line ends with "\n" on each platform.
    /// </summary>
    public static List<int> ReadPorts(IReadOnlyList<string> texts, TextWriter log)
    {
        var ports = new List<int>();
        for (int i = 0; i < texts.Count; i++)
        {
            try
            {
                ports.Add(ParsePort(texts[i]));
            }
            catch (FormatException e)
            {
                // The error is handled here: it is logged once and not thrown again.
                log.Write($"level=ERROR msg=\"read port failed\" line={i + 1} err={Quote(e.Message)}\n");
            }
        }
        log.Write($"level=INFO msg=\"ports read\" ok={ports.Count} total={texts.Count}\n");
        return ports;
    }
}
