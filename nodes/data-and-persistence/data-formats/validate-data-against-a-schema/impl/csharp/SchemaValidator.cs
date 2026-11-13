// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/validate-data-against-a-schema/.

using System.Text.Json;

public static class SchemaValidator
{
    // RFC 6901: "~" becomes "~0" first, then "/" becomes "~1".
    static string Pointer(string name) => "/" + name.Replace("~", "~0").Replace("/", "~1");

    static string[] Kinds(JsonElement value) => value.ValueKind switch
    {
        JsonValueKind.Null => ["null"],
        JsonValueKind.True or JsonValueKind.False => ["boolean"],
        JsonValueKind.Number => Math.Floor(value.GetDouble()) == value.GetDouble() ? ["integer", "number"] : ["number"],
        JsonValueKind.String => ["string"],
        JsonValueKind.Array => ["array"],
        _ => ["object"],
    };

    static void Check(JsonElement schema, JsonElement value, string here, string at, List<(string, string)> errors)
    {
        if (schema.ValueKind == JsonValueKind.True)
        {
            return;
        }
        if (schema.ValueKind == JsonValueKind.False)
        {
            errors.Add((here, at));
            return;
        }
        string[] kinds = Kinds(value);
        if (schema.TryGetProperty("type", out JsonElement type))
        {
            string?[] allowed = type.ValueKind == JsonValueKind.String
                ? [type.GetString()]
                : type.EnumerateArray().Select(t => t.GetString()).ToArray();
            if (!kinds.Any(allowed.Contains))
            {
                errors.Add((here, at + "/type"));
            }
        }
        switch (value.ValueKind)
        {
            case JsonValueKind.Number:
                double number = value.GetDouble();
                if (schema.TryGetProperty("minimum", out JsonElement minimum) && number < minimum.GetDouble())
                {
                    errors.Add((here, at + "/minimum"));
                }
                if (schema.TryGetProperty("maximum", out JsonElement maximum) && number > maximum.GetDouble())
                {
                    errors.Add((here, at + "/maximum"));
                }
                break;
            case JsonValueKind.String:
                // Length counts UTF-16 code units. EnumerateRunes counts code points.
                int length = value.GetString()!.EnumerateRunes().Count();
                if (schema.TryGetProperty("minLength", out JsonElement minLength) && length < minLength.GetDouble())
                {
                    errors.Add((here, at + "/minLength"));
                }
                if (schema.TryGetProperty("maxLength", out JsonElement maxLength) && length > maxLength.GetDouble())
                {
                    errors.Add((here, at + "/maxLength"));
                }
                break;
            case JsonValueKind.Object:
                if (schema.TryGetProperty("required", out JsonElement required)
                    && required.EnumerateArray().Any(name => !value.TryGetProperty(name.GetString()!, out _)))
                {
                    errors.Add((here, at + "/required"));
                }
                bool hasNamed = schema.TryGetProperty("properties", out JsonElement named);
                bool hasExtra = schema.TryGetProperty("additionalProperties", out JsonElement extra);
                foreach (JsonProperty member in value.EnumerateObject())
                {
                    if (hasNamed && named.TryGetProperty(member.Name, out JsonElement sub))
                    {
                        Check(sub, member.Value, here + Pointer(member.Name), at + "/properties" + Pointer(member.Name), errors);
                    }
                    else if (hasExtra)
                    {
                        Check(extra, member.Value, here + Pointer(member.Name), at + "/additionalProperties", errors);
                    }
                }
                break;
            case JsonValueKind.Array:
                if (schema.TryGetProperty("items", out JsonElement items))
                {
                    int index = 0;
                    foreach (JsonElement item in value.EnumerateArray())
                    {
                        Check(items, item, $"{here}/{index++}", at + "/items", errors);
                    }
                }
                break;
        }
    }

    // Returns the sorted (instance location, keyword location) pairs of the failed assertions.
    public static List<(string Instance, string Keyword)> Validate(string schema, string instance)
    {
        using JsonDocument s = JsonDocument.Parse(schema);
        using JsonDocument v = JsonDocument.Parse(instance);
        var errors = new List<(string, string)>();
        Check(s.RootElement, v.RootElement, "", "", errors);
        // CompareOrdinal compares UTF-16 code units, which is code point order for the names that the contract allows.
        errors.Sort((a, b) => string.CompareOrdinal(a.Item1, b.Item1) is int c && c != 0 ? c : string.CompareOrdinal(a.Item2, b.Item2));
        return errors;
    }
}
