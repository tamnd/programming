// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-json/.

using System.Globalization;
using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;

public static partial class CanonicalJson
{
    // An array index: ASCII digits, no sign, no leading zero.
    [GeneratedRegex("^(?:0|[1-9][0-9]*)$")]
    private static partial Regex Index();

    // Returns the value at path in the JSON text, as canonical JSON.
    // Throws a JsonException when the text is not JSON, and a
    // KeyNotFoundException when the path names no value.
    public static string JsonGet(string text, IReadOnlyList<string> path)
    {
        // JsonDocument keeps each number as its text, so no digit is lost.
        using JsonDocument document = JsonDocument.Parse(text);
        JsonElement value = document.RootElement;
        foreach (string step in path)
        {
            value = Step(value, step) ?? throw new KeyNotFoundException($"no value at \"{step}\"");
        }
        var output = new StringBuilder();
        Write(value, output);
        return output.ToString();
    }

    private static JsonElement? Step(JsonElement value, string step)
    {
        if (value.ValueKind == JsonValueKind.Object)
        {
            // JsonDocument keeps every member of a repeated name. The last one wins.
            JsonElement? found = null;
            foreach (JsonProperty member in value.EnumerateObject())
            {
                if (member.Name == step)
                {
                    found = member.Value;
                }
            }
            return found;
        }
        if (value.ValueKind == JsonValueKind.Array && Index().IsMatch(step)
            && int.TryParse(step, NumberStyles.None, CultureInfo.InvariantCulture, out int i)
            && i < value.GetArrayLength())
        {
            return value[i];
        }
        return null;
    }

    // UTF-8 byte order is code point order. string.CompareOrdinal uses UTF-16 code units.
    private static int ByCodePoint(string a, string b) =>
        Encoding.UTF8.GetBytes(a).AsSpan().SequenceCompareTo(Encoding.UTF8.GetBytes(b));

    // Writes the value as canonical JSON. The encoders of System.Text.Json
    // escape non-ASCII characters, so this writer escapes the strings itself.
    private static void Write(JsonElement value, StringBuilder output)
    {
        switch (value.ValueKind)
        {
            case JsonValueKind.Object:
                var members = new Dictionary<string, JsonElement>();
                foreach (JsonProperty member in value.EnumerateObject())
                {
                    members[member.Name] = member.Value;
                }
                var names = members.Keys.ToList();
                names.Sort(ByCodePoint);
                output.Append('{');
                for (int i = 0; i < names.Count; i++)
                {
                    if (i > 0)
                    {
                        output.Append(',');
                    }
                    WriteString(names[i], output);
                    output.Append(':');
                    Write(members[names[i]], output);
                }
                output.Append('}');
                break;
            case JsonValueKind.Array:
                output.Append('[');
                int n = 0;
                foreach (JsonElement element in value.EnumerateArray())
                {
                    if (n++ > 0)
                    {
                        output.Append(',');
                    }
                    Write(element, output);
                }
                output.Append(']');
                break;
            case JsonValueKind.String:
                WriteString(value.GetString()!, output);
                break;
            case JsonValueKind.Number:
                string number = value.GetRawText();
                output.Append(number == "-0" ? "0" : number);
                break;
            default:
                output.Append(value.GetRawText()); // true, false or null
                break;
        }
    }

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
}
