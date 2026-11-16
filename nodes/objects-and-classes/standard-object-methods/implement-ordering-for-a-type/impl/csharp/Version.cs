// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;

/// <summary>A version MAJOR.MINOR.PATCH. The record gives value equality, and IComparable gives the natural order.</summary>
public sealed record Version(int Major, int Minor, int Patch) : IComparable<Version>
{
    public static Version Parse(string text)
    {
        var p = text.Split('.');
        return new Version(
            int.Parse(p[0], CultureInfo.InvariantCulture),
            int.Parse(p[1], CultureInfo.InvariantCulture),
            int.Parse(p[2], CultureInfo.InvariantCulture));
    }

    /// <summary>Compares the numbers, most important first. Any version is greater than null.</summary>
    public int CompareTo(Version? other)
    {
        if (other is null) return 1;
        int c = Major.CompareTo(other.Major);
        if (c != 0) return c;
        c = Minor.CompareTo(other.Minor);
        return c != 0 ? c : Patch.CompareTo(other.Patch);
    }

    public static bool operator <(Version a, Version b) => a.CompareTo(b) < 0;

    public static bool operator >(Version a, Version b) => a.CompareTo(b) > 0;

    public static bool operator <=(Version a, Version b) => a.CompareTo(b) <= 0;

    public static bool operator >=(Version a, Version b) => a.CompareTo(b) >= 0;

    public override string ToString() => $"{Major}.{Minor}.{Patch}";

    public static List<string> SortVersions(IReadOnlyList<string> versions) =>
        versions.Select(Parse).Order().Select(v => v.ToString()).ToList();
}
