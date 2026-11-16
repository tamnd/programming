// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

using System.Collections.Generic;
using System.Threading;

/// <summary>Starts one thread per name; each thread builds the greeting for its name.</summary>
public static class Greetings
{
    public static string[] ThreadGreetings(IReadOnlyList<string> names)
    {
        var results = new string[names.Count];
        var threads = new Thread[names.Count];
        for (int i = 0; i < names.Count; i++)
        {
            // A copy for each iteration: the lambda must not capture the loop variable i.
            int index = i;
            string name = names[i];
            threads[i] = new Thread(() => results[index] = "hello from " + name) { Name = "greeter-" + i };
            threads[i].Start();
        }
        // Start all threads first, then wait for each one.
        foreach (var thread in threads)
        {
            thread.Join();
        }
        return results;
    }
}
