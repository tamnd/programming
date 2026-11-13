// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

using System.Text;
using Xunit;

public sealed class WriteCsvTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("write-csv-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    // Each record is written with its fields joined by "|", so that TheoryData can hold it.
    public static TheoryData<string, string[], string> Cases => new()
    {
        { "simple", ["name|age", "Ada|36", "Alan|41"], "name,age\r\nAda,36\r\nAlan,41\r\n" },
        { "comma", ["name|born", "Lovelace, Ada|1815"], "name,born\r\n\"Lovelace, Ada\",1815\r\n" },
        { "double-quote", ["id|quote", "1|She said \"hi\"."], "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n" },
        { "only-a-quote", ["q", "\""], "q\r\n\"\"\"\"\r\n" },
        { "header-only", ["name|age"], "name,age\r\n" },
        { "newline-in-field", ["id|address", "1|12 High St\nLeeds"], "id,address\r\n1,\"12 High St\nLeeds\"\r\n" },
        { "crlf-in-field", ["id|address", "1|12 High St\r\nLeeds"], "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n" },
        { "cr-in-field", ["id|text", "1|a\rb"], "id,text\r\n1,\"a\rb\"\r\n" },
        { "empty-fields", ["a|b|c", "||"], "a,b,c\r\n,,\r\n" },
        { "one-empty-field", ["note", "", "x"], "note\r\n\"\"\r\nx\r\n" },
        { "spaces", ["a|b", " x | y"], "a,b\r\n x , y\r\n" },
        { "no-records", [], "" },
        { "non-ascii", ["city", "Zürich", "東京"], "city\r\nZürich\r\n東京\r\n" },
    };

    private static List<IReadOnlyList<string>> Records(string[] rows) => rows.Select(r => (IReadOnlyList<string>)r.Split('|')).ToList();

    [Theory]
    [MemberData(nameof(Cases))]
    public void FormatAndWrite(string name, string[] rows, string want)
    {
        var records = Records(rows);
        Assert.Equal(want, WriteCsv.Format(records));
        string path = Path.Combine(dir, name + ".csv");
        WriteCsv.Write(path, records);
        Assert.Equal(Encoding.UTF8.GetBytes(want), File.ReadAllBytes(path));
    }

    [Theory]
    [InlineData("too-many-fields", new[] { "a|b", "1|2|3" })]
    [InlineData("too-few-fields", new[] { "a|b", "1" })]
    public void FieldCount(string name, string[] rows)
    {
        var records = Records(rows);
        Assert.Equal("field-count", Assert.Throws<CsvException>(() => WriteCsv.Format(records)).Kind);
        string path = Path.Combine(dir, name + ".csv");
        Assert.Throws<CsvException>(() => WriteCsv.Write(path, records));
        Assert.False(File.Exists(path));
    }
}
