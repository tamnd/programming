// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

import java.io.IOException;
import java.io.InputStream;
import java.io.UncheckedIOException;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionException;

/** Runs a program with an argument list and no shell. */
public final class RunProcess {
  private RunProcess() {}

  /** The exit status and the text of the two output streams. */
  public record Result(int status, String stdout, String stderr) {}

  /**
   * Runs argv and returns its result. A missing program throws an IOException. A status that is
   * not 0 is a result, not an exception.
   */
  public static Result run(List<String> argv) throws IOException, InterruptedException {
    Process process = new ProcessBuilder(argv).start();
    // Close the standard input of the child, so a read gets end of file.
    process.getOutputStream().close();
    // Read standard error on a second thread. If this thread reads one
    // stream to its end first, a child that fills the other pipe blocks.
    CompletableFuture<String> stderr =
        CompletableFuture.supplyAsync(() -> readAll(process.getErrorStream()));
    String stdout = readAll(process.getInputStream());
    int status = process.waitFor();
    try {
      return new Result(status, stdout, stderr.join());
    } catch (CompletionException e) {
      throw new IOException("cannot read standard error", e.getCause());
    }
  }

  private static String readAll(InputStream in) {
    try (in) {
      return new String(in.readAllBytes(), StandardCharsets.UTF_8);
    } catch (IOException e) {
      throw new UncheckedIOException(e);
    }
  }
}
