// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

import java.util.List;

/** The parsed arguments of the tool "count [-v] [-n N] [--] [FILE...]". */
public record CountArgs(boolean verbose, int limit, List<String> files) {
  public CountArgs {
    files = List.copyOf(files);
  }

  /** A usage error of the command line. */
  public static final class UsageException extends Exception {
    private static final long serialVersionUID = 1L;

    public UsageException(String message) {
      super(message);
    }
  }

  /** Parses the arguments that {@code main} gets. They do not hold the program name. */
  public static CountArgs parse(String... args) throws UsageException {
    boolean verbose = false;
    int limit = 10;
    int i = 0;
    loop:
    for (; i < args.length; i++) {
      String arg = args[i];
      switch (arg) {
        case "--" -> {
          i++;
          break loop;
        }
        case "-v", "--verbose" -> verbose = true;
        case "-n", "--limit" -> {
          if (i + 1 == args.length) {
            throw new UsageException("option " + arg + " needs a value");
          }
          limit = decimal(args[++i]);
        }
        default -> {
          if (arg.startsWith("--limit=")) {
            limit = decimal(arg.substring("--limit=".length()));
          } else if (arg.startsWith("-") && !arg.equals("-")) {
            throw new UsageException("unknown option " + arg);
          } else {
            break loop; // The first operand ends the options.
          }
        }
      }
    }
    return new CountArgs(verbose, limit, List.of(args).subList(i, args.length));
  }

  // Integer.parseInt also accepts a sign and the digits of other scripts.
  private static int decimal(String s) throws UsageException {
    if (s.isEmpty() || !s.chars().allMatch(c -> c >= '0' && c <= '9')) {
      throw new UsageException("limit '" + s + "' is not a decimal number");
    }
    try {
      return Integer.parseInt(s);
    } catch (NumberFormatException e) {
      throw new UsageException("limit '" + s + "' is too large");
    }
  }
}
