// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

using System.Text;
using Xunit;

public sealed class ReadCsvTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("read-csv-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(string text)
    {
        string path = Path.Combine(dir, "input.csv");
        File.WriteAllBytes(path, Encoding.UTF8.GetBytes(text));
        return path;
    }

    // Each record is written as "name=field" pairs joined by "|", so that TheoryData can hold it.
    public static TheoryData<string, string, string[]> Cases => new()
    {
        { "simple", "name,age\nAda,36\nAlan,41\n", ["name=Ada|age=36", "name=Alan|age=41"] },
        { "crlf", "name,age\r\nAda,36\r\nAlan,41\r\n", ["name=Ada|age=36", "name=Alan|age=41"] },
        { "no-final-newline", "name,age\nAda,36\nAlan,41", ["name=Ada|age=36", "name=Alan|age=41"] },
        { "quoted-comma", "name,born\n\"Lovelace, Ada\",1815\n", ["name=Lovelace, Ada|born=1815"] },
        { "escaped-quote", "id,quote\n1,\"She said \"\"hi\"\".\"\n", ["id=1|quote=She said \"hi\"."] },
        { "quoted-header", "\"first name\",\"last name\"\nAda,Lovelace\n", ["first name=Ada|last name=Lovelace"] },
        { "newline-in-field", "id,address\n1,\"12 High St\nLeeds\"\n", ["id=1|address=12 High St\nLeeds"] },
        { "crlf-in-field", "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n", ["id=1|address=12 High St\nLeeds"] },
        { "blank-line-in-field", "id,note\n1,\"one\n\ntwo\"\n", ["id=1|note=one\n\ntwo"] },
        { "empty-last-line", "a,b\n1,2\n\n", ["a=1|b=2"] },
        { "blank-lines", "a,b\n\n1,2\n\n\n3,4\n", ["a=1|b=2", "a=3|b=4"] },
        { "empty-fields", "a,b,c\n,,\n", ["a=|b=|c="] },
        { "quoted-empty", "name\n\"\"\n", ["name="] },
        { "spaces", "a,b\n x , y \n", ["a= x |b= y "] },
        { "header-only", "a,b\n", [] },
        { "empty-file", "", [] },
        { "bom", "\uFEFFname,age\nAda,36\n", ["name=Ada|age=36"] },
        { "non-ascii", "city,country\nZürich,Schweiz\n東京,日本\n", ["city=Zürich|country=Schweiz", "city=東京|country=日本"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Reads(string name, string text, string[] want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        var got = ReadCsv.Read(Write(text)).Select(r => string.Join("|", r.Select(kv => $"{kv.Key}={kv.Value}")));
        Assert.Equal(want, got);
    }

    [Theory]
    [InlineData("too-few-fields", "a,b\n1\n", "field-count")]
    [InlineData("too-many-fields", "a,b\n1,2,3\n", "field-count")]
    [InlineData("unclosed-quote", "a,b\n1,\"two\n", "bad-quote")]
    [InlineData("text-after-quote", "a,b\n1,\"two\"x\n", "bad-quote")]
    public void Invalid(string name, string text, string kind)
    {
        Assert.False(string.IsNullOrEmpty(name));
        string path = Write(text);
        Assert.Equal(kind, Assert.Throws<CsvException>(() => ReadCsv.Read(path)).Kind);
    }

    [Fact]
    public void Missing() =>
        Assert.Throws<FileNotFoundException>(() => ReadCsv.Read(Path.Combine(dir, "missing.csv")));
}
