// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

using System.Text.Json;
using System.Text.Json.Serialization;

// An element of the array is not an object with a number member amount.
public sealed class BadRecordException(string message) : Exception(message);

public static class StreamSum
{
    // Returns the number of objects in the JSON array in the file, and the sum of their member amount.
    // Throws a JsonException when the file is not a JSON array.
    public static long[] Sum(string path)
    {
        long count = 0, total = 0;
        using FileStream stream = File.OpenRead(path);
        // DeserializeAsyncEnumerable reads a root-level array one element at a time,
        // and holds only a buffer and the current element in memory.
        IAsyncEnumerable<JsonElement> elements = JsonSerializer.DeserializeAsyncEnumerable(stream, StreamJson.Default.JsonElement);
        foreach (JsonElement record in elements.ToBlockingEnumerable())
        {
            if (record.ValueKind != JsonValueKind.Object
                || !record.TryGetProperty("amount", out JsonElement amount)
                || amount.ValueKind != JsonValueKind.Number)
            {
                throw new BadRecordException($"element {count} has no number member amount");
            }
            count++;
            total += amount.GetInt64();
        }
        return [count, total];
    }
}

[JsonSerializable(typeof(JsonElement))]
internal partial class StreamJson : JsonSerializerContext;
