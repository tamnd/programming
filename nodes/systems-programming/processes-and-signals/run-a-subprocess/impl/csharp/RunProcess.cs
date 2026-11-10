// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

using System.Diagnostics;
using System.Text;

public static class RunProcess
{
    /// <summary>
    /// Runs argv with no shell and returns the exit status and the text of
    /// the two output streams. A missing program throws a Win32Exception.
    /// </summary>
    public static (int Status, string Stdout, string Stderr) Run(IReadOnlyList<string> argv)
    {
        var info = new ProcessStartInfo(argv[0])
        {
            UseShellExecute = false,
            RedirectStandardInput = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            StandardOutputEncoding = Encoding.UTF8,
            StandardErrorEncoding = Encoding.UTF8,
        };
        // ArgumentList passes each element as one argument, with no quoting.
        foreach (var arg in argv.Skip(1))
        {
            info.ArgumentList.Add(arg);
        }
        using var process = new Process { StartInfo = info };
        process.Start();
        // Close the standard input of the child, so a read gets end of file.
        process.StandardInput.Close();
        // Read standard error asynchronously while this thread reads
        // standard output. Two synchronous reads can deadlock.
        var stderr = process.StandardError.ReadToEndAsync();
        var stdout = process.StandardOutput.ReadToEnd();
        process.WaitForExit();
        return (process.ExitCode, stdout, stderr.GetAwaiter().GetResult());
    }
}
