// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-a-markdown-text/.

using System.Runtime.CompilerServices;
using System.Text.Json;
using Xunit;

public sealed class MarkdownToHtmlTests
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

    [Theory]
    [MemberData(nameof(Cases))]
    public void Converts(string name, string text, string expect)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal(expect, MarkdownToHtml.Convert(text));
    }
}
