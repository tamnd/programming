// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

using System.Linq;
using Xunit;

public sealed class ServeOrderTests
{
    public static TheoryData<string, int[], int[]> Cases => new()
    {
        { "example", new[] { 2, 5, 1, 5 }, new[] { 1, 3, 0, 2 } },
        { "longer", new[] { 3, 1, 4, 1, 5, 9, 2, 6 }, new[] { 5, 7, 4, 2, 0, 6, 1, 3 } },
        { "empty", new int[0], new int[0] },
        { "one", new[] { 7 }, new[] { 0 } },
        { "ties", new[] { 4, 4, 4, 4 }, new[] { 0, 1, 2, 3 } },
        { "negative", new[] { -3, -1, -2 }, new[] { 1, 2, 0 } },
        { "int32-extremes", new[] { int.MinValue, int.MaxValue, 0, int.MinValue }, new[] { 1, 2, 0, 3 } },
        { "sorted", new[] { 1, 2, 3 }, new[] { 2, 1, 0 } },
        { "reversed", new[] { 3, 2, 1 }, new[] { 0, 1, 2 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] priorities, int[] want)
    {
        Assert.NotNull(name);
        var before = priorities.ToArray();
        Assert.Equal(want, ServeOrder.Of(priorities));
        Assert.Equal(before, priorities);
    }
}
