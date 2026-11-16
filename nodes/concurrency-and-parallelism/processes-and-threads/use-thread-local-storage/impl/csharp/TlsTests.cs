// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

using System.Linq;
using System.Threading;
using Xunit;

public sealed class TlsTests
{
    public static TheoryData<string, int[]> Cases => new()
    {
        { "three-threads", new[] { 1, 2, 3 } },
        { "uneven", new[] { 10, 0, 5, 1000 } },
        { "empty", new int[] { } },
        { "zero", new[] { 0 } },
        { "one-thread", new[] { 7 } },
        { "equal-counts", new[] { 100, 100, 100, 100 } },
        { "many-threads", Enumerable.Range(1, 16).ToArray() },
        { "large", new[] { 100000, 100000 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CountPerThread(string name, int[] counts)
    {
        Assert.NotNull(name);
        Assert.Equal(counts, Tls.CountPerThread(counts));
    }

    [Fact]
    public void CallerCounterDoesNotChange()
    {
        int seen = -1;
        // Run in a new thread, so that other tests on this thread do not matter.
        var caller = new Thread(() =>
        {
            for (int k = 0; k < 3; k++)
            {
                Tls.Bump();
            }
            Tls.CountPerThread(new[] { 5, 6 });
            seen = Tls.Current();
        });
        caller.Start();
        caller.Join();
        Assert.Equal(3, seen);
    }
}
