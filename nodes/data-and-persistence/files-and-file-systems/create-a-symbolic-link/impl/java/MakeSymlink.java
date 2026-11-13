// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public final class MakeSymlink {
  private MakeSymlink() {}

  /**
   * Makes a symbolic link at link that holds target, and returns the target that the link holds,
   * read back with readSymbolicLink. target is stored as it is: it need not exist, and a relative
   * target is read from the directory of link. Files.createSymbolicLink takes the new link first
   * and the target second, the other order from ln -s. It throws FileAlreadyExistsException when
   * link exists, also as a dangling link, and NoSuchFileException when the directory of link does
   * not exist.
   */
  public static String makeSymlink(String target, Path link) throws IOException {
    Files.createSymbolicLink(link, Path.of(target));
    return Files.readSymbolicLink(link).toString();
  }
}
