// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReadCsvTest {
    @TempDir Path dir;

    static final List<Map<String, String>> ADA_ALAN =
            List.of(Map.of("name", "Ada", "age", "36"), Map.of("name", "Alan", "age", "41"));

    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("simple", "name,age\nAda,36\nAlan,41\n", ADA_ALAN),
                Arguments.of("crlf", "name,age\r\nAda,36\r\nAlan,41\r\n", ADA_ALAN),
                Arguments.of("no-final-newline", "name,age\nAda,36\nAlan,41", ADA_ALAN),
                Arguments.of("quoted-comma", "name,born\n\"Lovelace, Ada\",1815\n",
                        List.of(Map.of("name", "Lovelace, Ada", "born", "1815"))),
                Arguments.of("escaped-quote", "id,quote\n1,\"She said \"\"hi\"\".\"\n",
                        List.of(Map.of("id", "1", "quote", "She said \"hi\"."))),
                Arguments.of("quoted-header", "\"first name\",\"last name\"\nAda,Lovelace\n",
                        List.of(Map.of("first name", "Ada", "last name", "Lovelace"))),
                Arguments.of("newline-in-field", "id,address\n1,\"12 High St\nLeeds\"\n",
                        List.of(Map.of("id", "1", "address", "12 High St\nLeeds"))),
                Arguments.of("crlf-in-field", "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n",
                        List.of(Map.of("id", "1", "address", "12 High St\nLeeds"))),
                Arguments.of("blank-line-in-field", "id,note\n1,\"one\n\ntwo\"\n",
                        List.of(Map.of("id", "1", "note", "one\n\ntwo"))),
                Arguments.of("empty-last-line", "a,b\n1,2\n\n", List.of(Map.of("a", "1", "b", "2"))),
                Arguments.of("blank-lines", "a,b\n\n1,2\n\n\n3,4\n",
                        List.of(Map.of("a", "1", "b", "2"), Map.of("a", "3", "b", "4"))),
                Arguments.of("empty-fields", "a,b,c\n,,\n", List.of(Map.of("a", "", "b", "", "c", ""))),
                Arguments.of("quoted-empty", "name\n\"\"\n", List.of(Map.of("name", ""))),
                Arguments.of("spaces", "a,b\n x , y \n", List.of(Map.of("a", " x ", "b", " y "))),
                Arguments.of("header-only", "a,b\n", List.of()),
                Arguments.of("empty-file", "", List.of()),
                Arguments.of("bom", "\uFEFFname,age\nAda,36\n", List.of(Map.of("name", "Ada", "age", "36"))),
                Arguments.of("non-ascii", "city,country\nZürich,Schweiz\n東京,日本\n",
                        List.of(Map.of("city", "Zürich", "country", "Schweiz"), Map.of("city", "東京", "country", "日本"))));
    }

    Path write(String text) throws IOException {
        return Files.writeString(dir.resolve("input.csv"), text, StandardCharsets.UTF_8);
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void reads(String name, String text, List<Map<String, String>> want) throws IOException {
        assertEquals(want, ReadCsv.read(write(text)), name);
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource
    void invalid(String name, String text, String kind) throws IOException {
        Path path = write(text);
        ReadCsv.CsvException e = assertThrows(ReadCsv.CsvException.class, () -> ReadCsv.read(path));
        assertEquals(kind, e.kind(), name);
    }

    static Stream<Arguments> invalid() {
        return Stream.of(
                Arguments.of("too-few-fields", "a,b\n1\n", "field-count"),
                Arguments.of("too-many-fields", "a,b\n1,2,3\n", "field-count"),
                Arguments.of("unclosed-quote", "a,b\n1,\"two\n", "bad-quote"),
                Arguments.of("text-after-quote", "a,b\n1,\"two\"x\n", "bad-quote"));
    }

    @Test
    void missing() {
        assertThrows(NoSuchFileException.class, () -> ReadCsv.read(dir.resolve("missing.csv")));
    }
}
