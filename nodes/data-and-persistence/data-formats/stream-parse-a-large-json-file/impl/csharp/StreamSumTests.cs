// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

using System.Text;
using System.Text.Json;
using Xunit;

public sealed class StreamSumTests : IDisposable
{
    private readonly string dir = Directory.CreateTempSubdirectory("stream-sum-").FullName;

    public void Dispose() => Directory.Delete(dir, recursive: true);

    private string Write(string text)
    {
        string path = Path.Combine(dir, "input.json");
        File.WriteAllBytes(path, Encoding.UTF8.GetBytes(text));
        return path;
    }

    // 2500 records. The second space in the first record puts the digits of one amount across the offset 65536, so they come in two chunks.
    private static string ManyRecords() =>
        "[" + string.Join(",\n", Enumerable.Range(0, 2500).Select(i => $"{{\"id\": {i},{(i == 0 ? "  " : " ")}\"amount\": {i * 37 % 1000 - 400}}}")) + "]\n";

    public static TheoryData<string, string, long, long> Cases => new()
    {
        { "three-records", "[{\"id\": 1, \"amount\": 10}, {\"id\": 2, \"amount\": 25}, {\"id\": 3, \"amount\": -5}]\n", 3, 30 },
        { "pretty", "[\n  {\n    \"id\": 1,\n    \"amount\": 100\n  },\n  {\n    \"id\": 2,\n    \"amount\": 250\n  }\n]\n", 2, 350 },
        { "empty-array", "[]\n", 0, 0 },
        { "one-record", "[{\"amount\": 7}]", 1, 7 },
        { "extra-members", "[{\"id\": 1, \"tags\": [\"a\", \"b\"], \"meta\": {\"amount\": 999, \"x\": [1, [2]]}, \"amount\": 5}, {\"amount\": 6, \"ok\": true, \"none\": null}]\n", 2, 11 },
        { "strings-with-brackets", "[{\"note\": \"a ] b , c } \\\" [ {\", \"amount\": 1}, {\"note\": \"\\\\\", \"amount\": 2}, {\"note\": \"]\", \"amount\": 3}]\n", 3, 6 },
        { "non-ascii", "[{\"name\": \"Zürich 😀\", \"amount\": 3}, {\"name\": \"日本語\", \"amount\": 4}]\n", 2, 7 },
        { "crlf", "[\r\n{\"amount\": 1},\r\n{\"amount\": 2}\r\n]\r\n", 2, 3 },
        { "bom", "\uFEFF[{\"amount\": 4}]\n", 1, 4 },
        { "big-total", "[{\"amount\": 4503599627370496}, {\"amount\": 4503599627370495}, {\"amount\": 0}]\n", 3, 9007199254740991 },
        { "negative-total", "[{\"amount\": -9007199254740991}, {\"amount\": 0}]\n", 2, -9007199254740991 },
        { "whitespace", " \n\t[ {\"amount\" : 1} , {\"amount\":2} ]  \n\n", 2, 3 },
        // A string of 70000 characters is longer than one chunk.
        { "long-string", "[{\"note\": \"" + new string('x', 70000) + "\", \"amount\": 8}, {\"amount\": 9}]\n", 2, 17 },
        { "many-records", ManyRecords(), 2500, 249750 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Sums(string name, string text, long count, long total)
    {
        Assert.False(string.IsNullOrEmpty(name));
        Assert.Equal([count, total], StreamSum.Sum(Write(text)));
    }

    [Theory]
    [InlineData("not-array", "{\"amount\": 1}\n")]
    [InlineData("trailing-comma", "[{\"amount\": 1},]\n")]
    [InlineData("missing-comma", "[{\"amount\": 1} {\"amount\": 2}]\n")]
    [InlineData("unclosed", "[{\"amount\": 1},\n")]
    [InlineData("unclosed-element", "[{\"amount\": 1}, {\"amount\": 2\n")]
    [InlineData("data-after", "[{\"amount\": 1}] x\n")]
    [InlineData("two-arrays", "[] []\n")]
    [InlineData("empty-file", "")]
    [InlineData("bad-element", "[{\"amount\": 1}, {\"amount\": }]\n")]
    [InlineData("nan", "[{\"amount\": NaN}]\n")]
    public void Invalid(string name, string text)
    {
        Assert.False(string.IsNullOrEmpty(name));
        string path = Write(text);
        Assert.ThrowsAny<JsonException>(() => StreamSum.Sum(path));
    }

    [Theory]
    [InlineData("not-object", "[1, 2]\n")]
    [InlineData("missing-amount", "[{\"amount\": 1}, {\"id\": 2}]\n")]
    [InlineData("string-amount", "[{\"amount\": \"5\"}]\n")]
    [InlineData("null-amount", "[{\"amount\": null}]\n")]
    [InlineData("bool-amount", "[{\"amount\": true}]\n")]
    [InlineData("nested-array", "[[{\"amount\": 1}]]\n")]
    public void BadRecord(string name, string text)
    {
        Assert.False(string.IsNullOrEmpty(name));
        string path = Write(text);
        Assert.Throws<BadRecordException>(() => StreamSum.Sum(path));
    }

    [Fact]
    public void Missing() =>
        Assert.Throws<FileNotFoundException>(() => StreamSum.Sum(Path.Combine(dir, "missing.json")));
}
