// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

import static java.nio.file.StandardOpenOption.READ;
import static java.nio.file.StandardOpenOption.WRITE;

import java.io.IOException;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.OverlappingFileLockException;
import java.nio.file.Path;

/**
 * Takes advisory locks on files with FileChannel.tryLock. A Java lock belongs to the whole
 * virtual machine: two locks of one JVM on one file do not wait for each other, and the second
 * tryLock throws OverlappingFileLockException. So the locks keep other programs out, not other
 * threads of this program.
 */
public final class LockFile {
  private LockFile() {}

  /**
   * Asks for a shared or an exclusive lock on the whole file, without a wait. It returns null
   * when another program, or this JVM, has a lock that conflicts. Close the lock to release it
   * and its channel. The file must exist.
   */
  public static FileLock tryLockFile(Path path, boolean shared) throws IOException {
    // A shared lock needs a channel open for reading, and an exclusive lock needs one open for
    // writing, so the channel is open for both. Without CREATE, a missing file is an error.
    FileChannel ch = FileChannel.open(path, READ, WRITE);
    try {
      FileLock lock = ch.tryLock(0, Long.MAX_VALUE, shared);
      if (lock == null) {
        ch.close();
      }
      return lock;
    } catch (OverlappingFileLockException e) {
      ch.close();
      return null;
    } catch (IOException | RuntimeException e) {
      ch.close();
      throw e;
    }
  }

  /** Releases a lock from tryLockFile and closes its channel. */
  public static void release(FileLock lock) throws IOException {
    FileChannel ch = lock.channel();
    try {
      lock.release();
    } finally {
      ch.close();
    }
  }

  /**
   * The holder of the tests: "java LockFile hold PATH MODE" takes the lock, prints "locked",
   * and keeps the lock until its standard input ends.
   */
  public static void main(String[] args) throws IOException {
    if (args.length != 3 || !args[0].equals("hold")) {
      System.err.println("usage: LockFile hold PATH shared|exclusive");
      System.exit(2);
    }
    FileLock lock = tryLockFile(Path.of(args[1]), args[2].equals("shared"));
    System.out.println(lock == null ? "busy" : "locked");
    System.out.flush();
    while (System.in.read() >= 0) {
      // Wait for the end of the input.
    }
    if (lock != null) {
      release(lock);
    }
  }
}
