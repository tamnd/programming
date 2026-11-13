// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

using System.Diagnostics;
using System.Text;

/// <summary>Watch a file for changes: poll its size and modification time.</summary>
public static class WatchFile
{
    /// <summary>Seconds; the cases set the mtime to BaseMtime + step.</summary>
    public const long BaseMtime = 1_700_000_000;

    /// <summary>The size and the mtime of a file.</summary>
    private readonly record struct Signature(long Size, DateTime Mtime);

    /// <summary>The signature of the file, or null when it does not exist.</summary>
    private static Signature? Take(string path)
    {
        if (Directory.Exists(path))
        {
            throw new IOException($"{path} is a directory");
        }
        var info = new FileInfo(path);
        if (!info.Exists)
        {
            return null;
        }
        return new Signature(info.Length, info.LastWriteTimeUtc);
    }

    /// <summary>Remembers the last signature of a file and tells what changed since then.</summary>
    public sealed class FileWatcher
    {
        private readonly string path;
        private Signature? last;

        public FileWatcher(string path)
        {
            this.path = path;
            last = Take(path);
        }

        /// <summary>created, deleted, modified or none, for the time since the last poll.</summary>
        public string Poll()
        {
            Signature? now = Take(path);
            Signature? before = last;
            last = now;
            return (before, now) switch
            {
                (null, null) => "none",
                (null, _) => "created",
                (_, null) => "deleted",
                _ => before == now ? "none" : "modified",
            };
        }
    }

    /// <summary>Calls onChange once for each burst of changes, after the file is quiet.</summary>
    public static void Watch(string path, Action<string> onChange, TimeSpan interval, TimeSpan quiet, CancellationToken stop)
    {
        var watcher = new FileWatcher(path);
        long? pending = null; // the time of the last change that onChange has not seen
        while (!stop.IsCancellationRequested)
        {
            if (watcher.Poll() != "none")
            {
                pending = Stopwatch.GetTimestamp();
            }
            else if (pending is long t && Stopwatch.GetElapsedTime(t) >= quiet)
            {
                pending = null;
                onChange(path);
            }
            Thread.Sleep(interval);
        }
    }

    /// <summary>Makes the change of one step of a case (the entry of the cases uses it).</summary>
    private static void ApplyStep(string path, string step, int number)
    {
        int colon = step.IndexOf(':');
        string kind = colon < 0 ? step : step[..colon];
        string text = colon < 0 ? "" : step[(colon + 1)..];
        var utf8 = new UTF8Encoding(false);
        switch (kind)
        {
            case "create":
            case "write":
                File.WriteAllText(path, text, utf8);
                break;
            case "append":
                if (!File.Exists(path))
                {
                    throw new FileNotFoundException("no file to append to", path);
                }
                File.AppendAllText(path, text, utf8);
                break;
            case "replace":
                File.WriteAllText(path + ".tmp", text, utf8);
                File.Move(path + ".tmp", path, overwrite: true);
                break;
            case "recreate":
                File.Delete(path);
                File.WriteAllText(path, text, utf8);
                break;
            case "delete":
                File.Delete(path);
                break;
            case "mkdir":
                Directory.CreateDirectory(path);
                break;
            case "touch":
            case "none":
                break;
            default:
                throw new ArgumentException($"unknown step {step}");
        }
        if (kind != "none" && File.Exists(path))
        {
            File.SetLastWriteTimeUtc(path, DateTime.UnixEpoch.AddSeconds(BaseMtime + number));
        }
    }

    /// <summary>The entry of the cases: polls after each step and returns the results.</summary>
    public static List<string> WatchChanges(string path, IReadOnlyList<string> steps)
    {
        var watcher = new FileWatcher(path);
        var results = new List<string>();
        for (int i = 0; i < steps.Count; i++)
        {
            ApplyStep(path, steps[i], i + 1);
            results.Add(watcher.Poll());
        }
        return results;
    }
}
