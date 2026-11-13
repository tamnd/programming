// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class WriteCsvTest {
    @TempDir Path dir;

    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("simple", List.of(List.of("name", "age"), List.of("Ada", "36"), List.of("Alan", "41")), "name,age\r\nAda,36\r\nAlan,41\r\n"),
                Arguments.of("comma", List.of(List.of("name", "born"), List.of("Lovelace, Ada", "1815")), "name,born\r\n\"Lovelace, Ada\",1815\r\n"),
                Arguments.of("double-quote", List.of(List.of("id", "quote"), List.of("1", "She said \"hi\".")), "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n"),
                Arguments.of("only-a-quote", List.of(List.of("q"), List.of("\"")), "q\r\n\"\"\"\"\r\n"),
                Arguments.of("header-only", List.of(List.of("name", "age")), "name,age\r\n"),
                Arguments.of("newline-in-field", List.of(List.of("id", "address"), List.of("1", "12 High St\nLeeds")), "id,address\r\n1,\"12 High St\nLeeds\"\r\n"),
                Arguments.of("crlf-in-field", List.of(List.of("id", "address"), List.of("1", "12 High St\r\nLeeds")), "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n"),
                Arguments.of("cr-in-field", List.of(List.of("id", "text"), List.of("1", "a\rb")), "id,text\r\n1,\"a\rb\"\r\n"),
                Arguments.of("empty-fields", List.of(List.of("a", "b", "c"), List.of("", "", "")), "a,b,c\r\n,,\r\n"),
                Arguments.of("one-empty-field", List.of(List.of("note"), List.of(""), List.of("x")), "note\r\n\"\"\r\nx\r\n"),
                Arguments.of("spaces", List.of(List.of("a", "b"), List.of(" x ", " y")), "a,b\r\n x , y\r\n"),
                Arguments.of("no-records", List.of(), ""),
                Arguments.of("non-ascii", List.of(List.of("city"), List.of("Zürich"), List.of("東京")), "city\r\nZürich\r\n東京\r\n"));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void formatAndWrite(String name, List<List<String>> records, String want) throws IOException {
        assertEquals(want, WriteCsv.formatCsv(records));
        Path path = dir.resolve(name + ".csv");
        WriteCsv.writeCsv(path, records);
        assertArrayEquals(want.getBytes(StandardCharsets.UTF_8), Files.readAllBytes(path));
    }

    static Stream<Arguments> errors() {
        return Stream.of(
                Arguments.of("too-many-fields", List.of(List.of("a", "b"), List.of("1", "2", "3"))),
                Arguments.of("too-few-fields", List.of(List.of("a", "b"), List.of("1"))));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("errors")
    void fieldCount(String name, List<List<String>> records) {
        var e = assertThrows(WriteCsv.CsvException.class, () -> WriteCsv.formatCsv(records));
        assertEquals("field-count", e.kind());
        Path path = dir.resolve(name + ".csv");
        assertThrows(WriteCsv.CsvException.class, () -> WriteCsv.writeCsv(path, records));
        assertFalse(Files.exists(path));
    }
}
