// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-csv-to-json/.

using System.Text;

/// <summary>The error for a text that is not valid CSV. Kind is "field-count" or "bad-quote".</summary>
public sealed class CsvException(string kind, string message) : FormatException(message)
{
    public string Kind { get; } = kind;
}

/// <summary>Converts CSV text of RFC 4180 with a header line to a JSON array of objects.</summary>
public static class CsvToJson
{
    /// <summary>Returns a JSON array with one object for each record after the header.</summary>
    /// <exception cref="CsvException">a bad quote, or a record with the wrong number of fields.</exception>
    public static string Convert(string text)
    {
        List<List<string>> rows = Parse(text);
        var output = new StringBuilder("[");
        for (int n = 1; n < rows.Count; n++)
        {
            List<string> row = rows[n];
            if (row.Count != rows[0].Count)
            {
                throw new CsvException("field-count", $"record {n} has {row.Count} fields, want {rows[0].Count}");
            }
            output.Append(n > 1 ? ",{" : "{");
            for (int k = 0; k < row.Count; k++)
            {
                if (k > 0)
                {
                    output.Append(',');
                }
                WriteString(rows[0][k], output);
                output.Append(':');
                WriteString(row[k], output);
            }
            output.Append('}');
        }
        return output.Append(']').ToString();
    }

    /// <summary>Writes s as a JSON string.</summary>
    /// <remarks>Utf8JsonWriter escapes each character outside the BMP, also with
    /// JavaScriptEncoder.UnsafeRelaxedJsonEscaping, so the code writes the string itself.</remarks>
    private static void WriteString(string s, StringBuilder output)
    {
        output.Append('"');
        foreach (char c in s)
        {
            switch (c)
            {
                case '"': output.Append("\\\""); break;
                case '\\': output.Append("\\\\"); break;
                case '\b': output.Append("\\b"); break;
                case '\f': output.Append("\\f"); break;
                case '\n': output.Append("\\n"); break;
                case '\r': output.Append("\\r"); break;
                case '\t': output.Append("\\t"); break;
                case < ' ': output.Append($"\\u{(int)c:x4}"); break;
                default: output.Append(c); break;
            }
        }
        output.Append('"');
    }

    /// <summary>Returns the length of the line break at i: 1 for LF, 2 for CRLF, 0 for none.</summary>
    private static int LineBreak(string text, int i)
    {
        if (i < text.Length && text[i] == '\n')
        {
            return 1;
        }
        return i + 1 < text.Length && text[i] == '\r' && text[i + 1] == '\n' ? 2 : 0;
    }

    /// <summary>Splits RFC 4180 text into records of fields. An empty line gives no record.</summary>
    public static List<List<string>> Parse(string text)
    {
        var records = new List<List<string>>();
        int i = 0;
        while (i < text.Length)
        {
            int empty = LineBreak(text, i);
            if (empty > 0)
            {
                i += empty;
                continue;
            }
            var record = new List<string>();
            while (true)
            {
                var field = new StringBuilder();
                if (i < text.Length && text[i] == '"')
                {
                    i++;
                    while (true)
                    {
                        if (i >= text.Length)
                        {
                            throw new CsvException("bad-quote", "a quoted field has no closing quote");
                        }
                        if (text[i] == '"')
                        {
                            if (i + 1 >= text.Length || text[i + 1] != '"')
                            {
                                break;
                            }
                            field.Append('"');
                            i += 2;
                        }
                        else if (LineBreak(text, i) == 2)
                        {
                            field.Append('\n'); // CRLF inside quotes becomes LF.
                            i += 2;
                        }
                        else
                        {
                            field.Append(text[i++]);
                        }
                    }
                    i++; // the closing quote
                    if (i < text.Length && text[i] != ',' && LineBreak(text, i) == 0)
                    {
                        throw new CsvException("bad-quote", "text after the closing quote of a field");
                    }
                }
                else
                {
                    while (i < text.Length && text[i] != ',' && LineBreak(text, i) == 0)
                    {
                        field.Append(text[i++]);
                    }
                }
                record.Add(field.ToString());
                if (i >= text.Length || text[i] != ',')
                {
                    break;
                }
                i++;
            }
            i += LineBreak(text, i);
            records.Add(record);
        }
        return records;
    }
}
