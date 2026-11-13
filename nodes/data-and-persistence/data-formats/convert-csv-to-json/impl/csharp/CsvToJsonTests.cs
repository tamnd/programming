// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-csv-to-json/.

using System.Runtime.CompilerServices;
using System.Text.Json;
using Xunit;

public sealed class CsvToJsonTests
{
    private static string CasesDir([CallerFilePath] string file = "") =>
        Path.Combine(Path.GetDirectoryName(file)!, "..", "..", "cases");

    private static IEnumerable<JsonElement> Load(string name) =>
        File.ReadLines(Path.Combine(CasesDir(), name))
            .Where(line => line.Trim().Length > 0)
            .Select(line => JsonDocument.Parse(line).RootElement);

    public static TheoryData<string, string, string> Cases()
    {
        var data = new TheoryData<string, string, string>();
        foreach (JsonElement c in Load("basic.jsonl").Concat(Load("edge.jsonl")))
        {
            data.Add(c.GetProperty("name").GetString()!, c.GetProperty("args").GetProperty("text").GetString()!,
                c.GetProperty("expect").GetString()!);
        }
        return data;
    }

    public static TheoryData<string, string, string> Errors()
    {
        var data = new TheoryData<string, string, string>();
        foreach (JsonElement c in Load("errors.jsonl"))
        {
            data.Add(c.GetProperty("name").GetString()!, c.GetProperty("args").GetProperty("text").GetString()!,
                c.GetProperty("error").GetString()!);
        }
        return data;
    }

    [Theory]
    [MemberData(nameof(Cases))]
    public void Converts(string name, string text, string expect)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(expect, CsvToJson.Convert(text));
    }

    [Theory]
    [MemberData(nameof(Errors))]
    public void Rejects(string name, string text, string kind)
    {
        Assert.False(string.IsNullOrEmpty(name));
        var e = Assert.Throws<CsvException>(() => CsvToJson.Convert(text));
        Assert.Equal(kind, e.Kind);
    }

    [Fact]
    public void OutputIsJson()
    {
        string json = CsvToJson.Convert("id,note\n1,\"She said \"\"hi\"\"\nand left\"\n");
        JsonElement record = JsonDocument.Parse(json).RootElement[0];
        Assert.Equal("She said \"hi\"\nand left", record.GetProperty("note").GetString());
    }
}
