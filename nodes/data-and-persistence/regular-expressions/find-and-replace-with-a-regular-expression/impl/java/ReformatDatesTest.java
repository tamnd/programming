// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReformatDatesTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-date", "Due 2024-01-02.", "-", "/", "Due 02/01/2024."),
                Arguments.of("two-dates", "From 2024-01-02 to 2024-12-31", "-", "/", "From 02/01/2024 to 31/12/2024"),
                Arguments.of("no-date", "no dates here", "-", "/", "no dates here"),
                Arguments.of("empty-text", "", "-", "/", ""),
                Arguments.of("empty-to-sep", "2024-01-02", "-", "", "02012024"),
                Arguments.of("whole-text", "2024-01-02", "-", "/", "02/01/2024"),
                Arguments.of("adjacent", "2024-01-02,2024-01-03", "-", "/", "02/01/2024,03/01/2024"),
                Arguments.of("digit-before", "12024-01-02", "-", "/", "12024-01-02"),
                Arguments.of("digit-after", "2024-01-023", "-", "/", "2024-01-023"),
                Arguments.of("date-after-rejected", "2024-01-0234-05-06", "-", "/", "2024-01-06/05/0234"),
                Arguments.of("chained", "2024-01-02-03", "-", "/", "02/01/2024-03"),
                Arguments.of("other-separator", "2024/01/02", "-", ".", "2024/01/02"),
                Arguments.of("dot-separator", "2024x01x02 2024.01.02", ".", "-", "2024x01x02 02-01-2024"),
                Arguments.of("backslash-separator", "2024\\01\\02", "\\", "-", "02-01-2024"),
                Arguments.of("plus-separator", "2024+01+02 202401102", "+", "-", "02-01-2024 202401102"),
                Arguments.of("dollar-in-replacement", "2024-01-02", "-", "$1", "02$101$12024"),
                Arguments.of("backslash-in-replacement", "2024-01-02", "-", "\\1\\", "02\\1\\01\\1\\2024"),
                Arguments.of("digit-in-replacement", "2024-01-02", "-", "0", "0200102024"),
                Arguments.of("whole-match-in-replacement", "2024-01-02", "-", "$&", "02$&01$&2024"),
                Arguments.of("unicode-digits", "٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 2024-01-02", "-", "/", "٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 02/01/2024"),
                Arguments.of("non-ascii-text", "Café ☕ 2024-01-02 🙂", "-", "/", "Café ☕ 02/01/2024 🙂"),
                Arguments.of("non-ascii-separator", "2024–01–02", "–", "·", "02·01·2024"),
                Arguments.of("multiline", "2024-01-02\n2024-01-03\n", "-", "/", "02/01/2024\n03/01/2024\n"),
                Arguments.of("not-a-calendar-date", "2024-13-45", "-", "/", "45/13/2024")
        );
    }

    static Stream<Arguments> findCases() {
        return Stream.of(
                Arguments.of("two-dates", "From 2024-01-02 to 2024-12-31", "-", List.of(List.of("2024", "01", "02"), List.of("2024", "12", "31"))),
                Arguments.of("none", "no dates here", "-", List.of()),
                Arguments.of("empty", "", "-", List.of()),
                Arguments.of("date-after-rejected", "2024-01-0234-05-06", "-", List.of(List.of("0234", "05", "06"))),
                Arguments.of("dot-separator", "2024x01x02 2024.01.02", ".", List.of(List.of("2024", "01", "02"))),
                Arguments.of("unicode-digits", "٢٠٢٤-٠١-٠٢ 2024-01-02", "-", List.of(List.of("2024", "01", "02")))
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void reformatDates(String name, String text, String fromSep, String toSep, String want) {
        assertEquals(want, ReformatDates.reformatDates(text, fromSep, toSep), name);
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("findCases")
    void findDates(String name, String text, String sep, List<List<String>> want) {
        assertEquals(want, ReformatDates.findDates(text, sep), name);
    }
}
