// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

import java.io.ByteArrayOutputStream;
import java.io.Closeable;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;

public final class FollowFile {
  private FollowFile() {}

  /**
   * Reads the lines that are added to a file, one poll at a time, as tail -F does. The follower
   * keeps the file open, so it can read the rest of the file after a rename or a delete.
   */
  public static final class Follower implements Closeable {
    private final Path path;
    private FileChannel channel; // null if no file is open
    private Object key; // the identity of the open file
    private long offset;
    private ByteArrayOutputStream pending = new ByteArrayOutputStream(); // the bytes after the last LF

    /** Opens the file at path and starts at its end. Throws NoSuchFileException if no file is at path. */
    public Follower(Path path) throws IOException {
      this.path = path;
      open();
      offset = channel.size();
    }

    private void open() throws IOException {
      channel = FileChannel.open(path, StandardOpenOption.READ);
      // Java cannot get the attributes of an open channel, so the key comes from the path just
      // after the open. On Linux and macOS, the key holds the device and the inode number.
      try {
        key = Files.readAttributes(path, BasicFileAttributes.class).fileKey();
      } catch (IOException e) {
        close();
        throw e;
      }
    }

    /** Returns the complete lines that were added since the last poll. */
    public List<String> poll() throws IOException {
      List<String> lines = new ArrayList<>();
      if (channel != null) {
        // Look at the path before the read. Then the read gets each byte that was written before a
        // rename or a delete.
        Object now;
        try {
          now = Files.readAttributes(path, BasicFileAttributes.class).fileKey();
        } catch (NoSuchFileException e) {
          now = null;
        }
        if (channel.size() < offset) {
          // The file is shorter than the offset, so it was truncated.
          flush(lines);
          offset = 0;
        }
        read(lines);
        if (now == null || !Objects.equals(now, key)) {
          // The file was deleted or renamed. Its rest is read.
          flush(lines);
          close();
        }
      }
      if (channel == null) {
        try {
          open();
        } catch (NoSuchFileException e) {
          return lines;
        }
        offset = 0;
        read(lines);
      }
      return lines;
    }

    /** Calls emit with each new line. Polls the file each interval, and returns when stop is true. */
    public void follow(Duration interval, BooleanSupplier stop, Consumer<String> emit)
        throws IOException, InterruptedException {
      for (;;) {
        poll().forEach(emit);
        if (stop.getAsBoolean()) {
          return;
        }
        Thread.sleep(interval);
      }
    }

    @Override
    public void close() throws IOException {
      if (channel != null) {
        FileChannel c = channel;
        channel = null;
        c.close();
      }
    }

    private void read(List<String> lines) throws IOException {
      ByteBuffer buf = ByteBuffer.allocate(65536);
      for (int n; (n = channel.read(buf.clear(), offset)) > 0; ) {
        offset += n;
        // Split the bytes at each LF first, and decode each line after. A read can end in the
        // middle of a UTF-8 character.
        byte[] data = buf.array();
        int start = 0;
        for (int i = 0; i < n; i++) {
          if (data[i] == '\n') {
            pending.write(data, start, i - start);
            lines.add(pending.toString(StandardCharsets.UTF_8));
            pending.reset();
            start = i + 1;
          }
        }
        pending.write(data, start, n - start);
      }
    }

    private void flush(List<String> lines) {
      if (pending.size() > 0) {
        lines.add(pending.toString(StandardCharsets.UTF_8));
        pending.reset();
      }
    }
  }

  /** Writes initial to path, makes a follower, does the steps and returns the lines of each poll. */
  public static List<List<String>> followFile(Path path, String initial, List<String> steps)
      throws IOException {
    Files.writeString(path, initial);
    Path old = path.resolveSibling(path.getFileName() + ".1");
    List<List<String>> polls = new ArrayList<>();
    try (Follower follower = new Follower(path)) {
      for (String step : steps) {
        int colon = step.indexOf(':');
        String op = colon < 0 ? step : step.substring(0, colon);
        String text = colon < 0 ? "" : step.substring(colon + 1);
        switch (op) {
          case "poll" -> polls.add(follower.poll());
          case "append" -> append(path, text);
          case "append-old" -> append(old, text);
          case "truncate" -> Files.writeString(path, text);
          case "rotate" -> {
            Files.move(path, old, StandardCopyOption.REPLACE_EXISTING);
            Files.createFile(path);
          }
          case "delete" -> Files.delete(path);
          case "create" -> Files.writeString(path, text, StandardOpenOption.CREATE_NEW, StandardOpenOption.WRITE);
          default -> throw new IllegalArgumentException("bad step: " + step);
        }
      }
    }
    return polls;
  }

  private static void append(Path path, String text) throws IOException {
    Files.writeString(path, text, StandardOpenOption.CREATE, StandardOpenOption.APPEND);
  }
}
