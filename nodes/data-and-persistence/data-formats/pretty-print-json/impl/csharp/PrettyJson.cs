// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pretty-print-json/.

using System.Text;
using System.Text.Json;

public static class PrettyJson
{
    // Returns the JSON text indented by two spaces, with the names of each object sorted.
    // Throws a JsonException when the text is not JSON.
    public static string Format(string text)
    {
        // JsonDocument keeps each number as its text, so no digit is lost.
        using JsonDocument document = JsonDocument.Parse(text);
        var output = new StringBuilder();
        Write(document.RootElement, "", output);
        return output.ToString();
    }

    // UTF-8 byte order is code point order. string.CompareOrdinal uses UTF-16 code units.
    private static int ByCodePoint(string a, string b) =>
        Encoding.UTF8.GetBytes(a).AsSpan().SequenceCompareTo(Encoding.UTF8.GetBytes(b));

    // Writes the value with an indent of two spaces for each level. The encoders of
    // System.Text.Json escape non-ASCII characters, so this writer escapes the strings itself.
    private static void Write(JsonElement value, string indent, StringBuilder output)
    {
        string inner = indent + "  ";
        switch (value.ValueKind)
        {
            case JsonValueKind.Object:
                // JsonDocument keeps every member of a repeated name. The last one wins.
                var members = new Dictionary<string, JsonElement>();
                foreach (JsonProperty member in value.EnumerateObject())
                {
                    members[member.Name] = member.Value;
                }
                if (members.Count == 0)
                {
                    output.Append("{}");
                    break;
                }
                var names = members.Keys.ToList();
                names.Sort(ByCodePoint);
                output.Append('{');
                for (int i = 0; i < names.Count; i++)
                {
                    output.Append(i > 0 ? ",\n" : "\n").Append(inner);
                    WriteString(names[i], output);
                    output.Append(": ");
                    Write(members[names[i]], inner, output);
                }
                output.Append('\n').Append(indent).Append('}');
                break;
            case JsonValueKind.Array:
                if (value.GetArrayLength() == 0)
                {
                    output.Append("[]");
                    break;
                }
                output.Append('[');
                int n = 0;
                foreach (JsonElement element in value.EnumerateArray())
                {
                    output.Append(n++ > 0 ? ",\n" : "\n").Append(inner);
                    Write(element, inner, output);
                }
                output.Append('\n').Append(indent).Append(']');
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
