// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

import java.io.IOException;
import java.io.UncheckedIOException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.NotDirectoryException;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

/** Lists the entries of a directory, or of a whole tree. */
public final class ListEntries {
    private ListEntries() {}

    /**
     * Returns the relative paths of the entries under root, sorted by code point. A directory has
     * a "/" at the end. A symbolic link is listed, but it is never followed.
     *
     * @throws java.nio.file.NoSuchFileException if root does not exist
     * @throws NotDirectoryException if root is not a directory
     */
    public static List<String> list(Path root, boolean recursive) throws IOException {
        // Files.walk on a file gives the file itself, so check the type first.
        // readAttributes throws NoSuchFileException for a missing path.
        if (!Files.readAttributes(root, BasicFileAttributes.class).isDirectory()) {
            throw new NotDirectoryException(root.toString());
        }
        List<String> out = new ArrayList<>();
        // Files.walk does not follow symbolic links unless it gets FOLLOW_LINKS.
        try (Stream<Path> paths = Files.walk(root, recursive ? Integer.MAX_VALUE : 1)) {
            paths.skip(1).forEach(p -> {
                String rel = root.relativize(p).toString().replace(p.getFileSystem().getSeparator(), "/");
                out.add(Files.isDirectory(p, LinkOption.NOFOLLOW_LINKS) ? rel + "/" : rel);
            });
        } catch (UncheckedIOException e) {
            throw e.getCause();
        }
        // String.compareTo compares UTF-16 code units. That is code point order
        // for names with no characters outside the Basic Multilingual Plane.
        out.sort(null);
        return out;
    }
}
