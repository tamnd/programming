// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

public static class PointClassifier
{
    // Names the place of the point (x, y): origin, x-axis, y-axis, diagonal or other.
    public static string Classify(int x, int y) => (x, y) switch
    {
        (0, 0) => "origin",
        (_, 0) => "x-axis",
        (0, _) => "y-axis",
        var (a, b) when a == b => "diagonal",
        _ => "other",
    };
}
