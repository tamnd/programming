// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

using System.Text;

/// <summary>A record has more or fewer fields than the first record (the header).</summary>
public sealed class CsvException(string message) : FormatException(message)
{
    public string Kind => "field-count";
}

/// <summary>Writes records as a CSV file of RFC 4180. .NET has no CSV writer, so this class has a small one.</summary>
public static class WriteCsv
{
    private static readonly char[] Special = [',', '"', '\r', '\n'];

    /// <summary>Returns the CSV text of the records. The first record is the header.</summary>
    /// <exception cref="CsvException">a record with the wrong number of fields.</exception>
    public static string Format(IReadOnlyList<IReadOnlyList<string>> records)
    {
        var out_ = new StringBuilder();
        for (int n = 0; n < records.Count; n++)
        {
            IReadOnlyList<string> record = records[n];
            if (record.Count != records[0].Count)
            {
                throw new CsvException($"record {n} has {record.Count} fields, the header has {records[0].Count}");
            }
            for (int i = 0; i < record.Count; i++)
            {
                if (i > 0)
                {
                    out_.Append(',');
                }
                string field = record[i];
                // A record with one empty field needs quotes, or the line is empty and a reader skips it.
                if (field.IndexOfAny(Special) >= 0 || (record.Count == 1 && field.Length == 0))
                {
                    out_.Append('"').Append(field.Replace("\"", "\"\"")).Append('"');
                }
                else
                {
                    out_.Append(field);
                }
            }
            out_.Append("\r\n");
        }
        return out_.ToString();
    }

    /// <summary>Writes the records to the file at path as UTF-8, with no byte order mark.</summary>
    public static void Write(string path, IReadOnlyList<IReadOnlyList<string>> records)
    {
        // WriteAllText(string, string) writes UTF-8 without a byte order mark.
        File.WriteAllText(path, Format(records));
    }
}
