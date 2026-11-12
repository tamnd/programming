// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

using System.Collections.Generic;
using System.Globalization;
using System.Linq;

public static class Join
{
    /// <summary>Returns the items with sep between each two.</summary>
    public static string Strings(IEnumerable<string> items, string sep) =>
        string.Join(sep, items);

    /// <summary>Returns the decimal form of each item, with sep between each two.</summary>
    public static string Numbers(IEnumerable<int> items, string sep) =>
        // string.Join<T> calls ToString with the current culture, so the code
        // converts each item with the invariant culture first.
        string.Join(sep, items.Select(n => n.ToString(CultureInfo.InvariantCulture)));
}
