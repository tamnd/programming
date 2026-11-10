// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

using System.Text;

/// <summary>The error for a file that is not valid CSV. Kind is "field-count" or "bad-quote".</summary>
public sealed class CsvException(string kind, string message) : FormatException(message)
{
    public string Kind { get; } = kind;
}

/// <summary>Reads a CSV file of RFC 4180 with a header line.</summary>
public static class ReadCsv
{
    /// <summary>Returns one dictionary for each record after the header, from each name to its field.</summary>
    /// <exception cref="CsvException">a bad quote, or a record with the wrong number of fields.</exception>
    public static List<Dictionary<string, string>> Read(string path)
    {
        // ReadAllText detects and removes a UTF-8 byte order mark.
        string text = File.ReadAllText(path, Encoding.UTF8);
        List<List<string>> rows = Parse(text);
        var records = new List<Dictionary<string, string>>();
        if (rows.Count == 0)
        {
            return records;
        }
        List<string> header = rows[0];
        for (int n = 1; n < rows.Count; n++)
        {
            List<string> row = rows[n];
            if (row.Count != header.Count)
            {
                throw new CsvException("field-count", $"record {n} has {row.Count} fields, want {header.Count}");
            }
            var record = new Dictionary<string, string>(header.Count);
            for (int k = 0; k < row.Count; k++)
            {
                record.Add(header[k], row[k]);
            }
            records.Add(record);
        }
        return records;
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
