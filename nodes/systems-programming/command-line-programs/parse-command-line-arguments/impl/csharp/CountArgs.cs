// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

using System.Globalization;

/// <summary>A usage error of the command line.</summary>
public sealed class UsageException(string message) : Exception(message);

/// <summary>The parsed arguments of the tool "count [-v] [-n N] [--] [FILE...]".</summary>
public sealed record CountArgs(bool Verbose, int Limit, IReadOnlyList<string> Files)
{
    /// <summary>Parses the arguments that Main gets. They do not hold the program name.</summary>
    public static CountArgs Parse(IReadOnlyList<string> args)
    {
        bool verbose = false;
        int limit = 10;
        int i = 0;
        for (; i < args.Count; i++)
        {
            string arg = args[i];
            if (arg == "--")
            {
                i++;
                break;
            }
            else if (arg is "-v" or "--verbose")
            {
                verbose = true;
            }
            else if (arg is "-n" or "--limit")
            {
                if (i + 1 == args.Count)
                {
                    throw new UsageException($"option {arg} needs a value");
                }
                limit = Decimal(args[++i]);
            }
            else if (arg.StartsWith("--limit=", StringComparison.Ordinal))
            {
                limit = Decimal(arg["--limit=".Length..]);
            }
            else if (arg.StartsWith('-') && arg != "-")
            {
                throw new UsageException($"unknown option {arg}");
            }
            else
            {
                break; // The first operand ends the options.
            }
        }
        return new CountArgs(verbose, limit, args.Skip(i).ToArray());
    }

    // NumberStyles.None allows decimal digits only: no sign and no white space.
    private static int Decimal(string s) =>
        int.TryParse(s, NumberStyles.None, CultureInfo.InvariantCulture, out int n)
            ? n
            : throw new UsageException($"limit '{s}' is not a decimal number");

    // A record compares a list by reference, so equality compares the items.
    public bool Equals(CountArgs? other) =>
        other is not null && Verbose == other.Verbose && Limit == other.Limit && Files.SequenceEqual(other.Files);

    public override int GetHashCode() => HashCode.Combine(Verbose, Limit, Files.Count);
}
