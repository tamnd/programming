// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

using Xunit;

public sealed class UseLinkedListTests
{
    public static TheoryData<string, string[], int[]> Cases => new()
    {
        { "deque", new string[] { "push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3" }, new int[] { 0, 1, 3 } },
        { "remove-middle", new string[] { "push_back 1", "push_back 2", "push_back 3", "remove 2" }, new int[] { 1, 3 } },
        { "empty", new string[0], new int[0] },
        { "push-pop", new string[] { "push_back 1", "pop_front" }, new int[0] },
        { "single", new string[] { "push_front 7" }, new int[] { 7 } },
        { "remove-only", new string[] { "push_back 5", "remove 5", "push_back 6" }, new int[] { 6 } },
        { "remove-head", new string[] { "push_back 1", "push_back 2", "remove 1", "push_front 0" }, new int[] { 0, 2 } },
        { "remove-tail", new string[] { "push_back 1", "push_back 2", "remove 2", "push_back 3" }, new int[] { 1, 3 } },
        { "pop-both-ends", new string[] { "push_back 1", "push_back 2", "pop_front", "pop_back", "push_front 3" }, new int[] { 3 } },
        { "remove-absent", new string[] { "push_back 1", "remove 9" }, new int[] { 1 } },
        { "duplicates", new string[] { "push_back 4", "push_back 5", "push_back 4", "remove 4" }, new int[] { 5, 4 } },
        { "push-front-order", new string[] { "push_front 1", "push_front 2", "push_front 3" }, new int[] { 3, 2, 1 } },
        { "negative", new string[] { "push_front -1", "push_back -2", "remove -1" }, new int[] { -2 } },
        { "limit", new string[] { "push_back 2147483647", "push_front -2147483648", "remove 2147483647", "push_back 2147483647" }, new int[] { int.MinValue, 2147483647 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] ops, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, UseLinkedList.Run(ops));
    }
}
