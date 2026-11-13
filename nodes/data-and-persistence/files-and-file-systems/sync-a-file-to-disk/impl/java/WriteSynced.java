// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

import static java.nio.file.StandardOpenOption.CREATE;
import static java.nio.file.StandardOpenOption.READ;
import static java.nio.file.StandardOpenOption.TRUNCATE_EXISTING;
import static java.nio.file.StandardOpenOption.WRITE;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;

/** Writes a file and returns when its data is on the storage device. */
public final class WriteSynced {
  private WriteSynced() {}

  /** Writes text to path as UTF-8, and syncs the file and its directory. */
  public static void writeSynced(Path path, String text) throws IOException {
    try (FileChannel ch = FileChannel.open(path, WRITE, CREATE, TRUNCATE_EXISTING)) {
      ByteBuffer buf = ByteBuffer.wrap(text.getBytes(StandardCharsets.UTF_8));
      while (buf.hasRemaining()) {
        ch.write(buf);
      }
      // true: sync the metadata too, as fsync does. false is like fdatasync.
      ch.force(true);
    }
    syncDirectory(path.toAbsolutePath().getParent());
  }

  /**
   * Syncs the entries of a directory, so that a new name in it is on disk. Java
   * has no call for it, but on Linux and macOS a directory opens for reading as
   * a channel, and force calls fsync on it.
   */
  public static void syncDirectory(Path dir) throws IOException {
    try (FileChannel ch = FileChannel.open(dir, READ)) {
      ch.force(true);
    }
  }
}
