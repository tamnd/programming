// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.NotDirectoryException;
import java.nio.file.Path;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ListEntriesTest {
    @TempDir Path dir;

    static final List<String> TREE =
            List.of("docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md");

    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("flat", List.of("b.txt", "a.txt", "c.txt"), false, List.of("a.txt", "b.txt", "c.txt")),
                Arguments.of("nested", TREE, true, TREE),
                Arguments.of("nested-shallow", TREE, false, List.of("docs/", "readme.md")),
                Arguments.of("empty", List.of(), true, List.of()),
                Arguments.of("empty-subdir", List.of("empty/"), true, List.of("empty/")),
                Arguments.of("hidden", List.of(".env", ".git/", ".git/config", "main.go"), true,
                        List.of(".env", ".git/", ".git/config", "main.go")),
                Arguments.of("case-order", List.of("a.txt", "B.txt", "c.txt"), false, List.of("B.txt", "a.txt", "c.txt")),
                Arguments.of("dot-before-slash", List.of("a/", "a/x", "a.txt", "a-b"), true,
                        List.of("a-b", "a.txt", "a/", "a/x")),
                Arguments.of("symlink-to-dir", List.of("real/", "real/f", "link -> real"), true,
                        List.of("link", "real/", "real/f")),
                Arguments.of("symlink-to-file", List.of("f", "g -> f"), true, List.of("f", "g")),
                Arguments.of("broken-symlink", List.of("dangling -> nowhere"), true, List.of("dangling")),
                Arguments.of("symlink-loop", List.of("d/", "d/up -> .."), true, List.of("d/", "d/up")),
                Arguments.of("spaces", List.of("my file.txt", "two  spaces/", "two  spaces/x"), true,
                        List.of("my file.txt", "two  spaces/", "two  spaces/x")),
                Arguments.of("non-ascii", List.of("été.txt", "zebra.txt", "apple.txt"), false,
                        List.of("apple.txt", "zebra.txt", "été.txt")));
    }

    void makeTree(List<String> tree) throws IOException {
        for (String entry : tree) {
            int arrow = entry.indexOf(" -> ");
            if (arrow >= 0) {
                Files.createSymbolicLink(dir.resolve(entry.substring(0, arrow)), Path.of(entry.substring(arrow + 4)));
            } else if (entry.endsWith("/")) {
                Files.createDirectory(dir.resolve(entry));
            } else {
                Files.createFile(dir.resolve(entry));
            }
        }
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void lists(String name, List<String> tree, boolean recursive, List<String> want) throws IOException {
        makeTree(tree);
        assertEquals(want, ListEntries.list(dir, recursive), name);
    }

    @Test
    void missing() {
        assertThrows(NoSuchFileException.class, () -> ListEntries.list(dir.resolve("missing"), true));
    }

    @Test
    void notADirectory() throws IOException {
        makeTree(List.of("notes.txt"));
        assertThrows(NotDirectoryException.class, () -> ListEntries.list(dir.resolve("notes.txt"), true));
    }
}
