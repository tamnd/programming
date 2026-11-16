// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

// Run one job for each number on the .NET thread pool, at most `workers` at a time,
// and collect the results in order.

using System.Threading.Tasks;

public static class Pool
{
    public static int CollatzSteps(long n)
    {
        int steps = 0;
        while (n != 1)
        {
            n = n % 2 == 0 ? n / 2 : 3 * n + 1;
            steps++;
        }
        return steps;
    }

    public static int[] StepsOnPool(int workers, int[] numbers)
    {
        var results = new int[numbers.Length];
        var options = new ParallelOptions { MaxDegreeOfParallelism = workers };
        // Parallel.For returns when every iteration has finished. Each iteration writes its own slot.
        Parallel.For(0, numbers.Length, options, i => results[i] = CollatzSteps(numbers[i]));
        return results;
    }
}
