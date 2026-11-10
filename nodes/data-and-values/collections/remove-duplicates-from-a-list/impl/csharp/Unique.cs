// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

/// <summary>A record of a user. A record gets Equals and GetHashCode from its members.</summary>
public sealed record User(string Name, string Email);

public static class Dedupe
{
    /// <summary>Returns a new list of the items without later copies. The first copies keep their order.</summary>
    public static List<T> Unique<T>(IEnumerable<T> items) => UniqueBy(items, x => x);

    /// <summary>Returns a new list of the first item for each key, in input order.</summary>
    public static List<T> UniqueBy<T, TKey>(IEnumerable<T> items, Func<T, TKey> key)
    {
        // Not Distinct or DistinctBy: their documentation calls the result unordered.
        var seen = new HashSet<TKey>();
        var result = new List<T>();
        foreach (var item in items)
        {
            // Add returns false if the set already has the key.
            if (seen.Add(key(item)))
            {
                result.Add(item);
            }
        }
        return result;
    }

    /// <summary>Returns the first user for each email. Case does not matter in the emails.</summary>
    public static List<User> UniqueByEmail(IEnumerable<User> users) =>
        // ToLowerInvariant does not use the rules of the current culture.
        UniqueBy(users, u => u.Email.ToLowerInvariant());
}
