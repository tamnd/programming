// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

using System.Linq;
using System.Threading.Tasks;
using Xunit;

public sealed class IdGeneratorTests
{
    public static TheoryData<string, int, long[]> Cases => new()
    {
        { "three-calls", 3, new long[] { 2, 1, 1 } },
        { "ten-calls", 10, new long[] { 9, 1, 1 } },
        { "single-call", 1, new long[] { 0, 1, 1 } },
        { "pair", 2, new long[] { 1, 1, 1 } },
        { "limit", 1000000, new long[] { 999999, 1, 1 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Report(string name, int calls, long[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, IdGenerator.Report(calls));
    }

    [Fact]
    public async Task ThreadsShareOneInstance()
    {
        var got = await Task.WhenAll(Enumerable.Range(0, 8).Select(_ => Task.Run(() => IdGenerator.Instance)));
        Assert.All(got, g => Assert.Same(IdGenerator.Instance, g));
        Assert.Equal(1, IdGenerator.Constructions);
    }
}
