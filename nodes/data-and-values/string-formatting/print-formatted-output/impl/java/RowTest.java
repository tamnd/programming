// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RowTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("apple", "apple", 3, 1.5, "apple     |    3|    1.50"),
                Arguments.of("widget", "widget", 12, 19.99, "widget    |   12|   19.99"),
                Arguments.of("laptop", "laptop", 1, 1234.5, "laptop    |    1| 1234.50"),
                Arguments.of("empty-name", "", 0, 0.0, "          |    0|    0.00"),
                Arguments.of("exact-widths", "abcdefghij", 99999, 99999.99, "abcdefghij|99999|99999.99"),
                Arguments.of("long-name", "watermelonade", 5, 2.0, "watermelonade|    5|    2.00"),
                Arguments.of("wide-quantity", "pen", 123456, 0.5, "pen       |123456|    0.50"),
                Arguments.of("wide-price", "car", 1, 123456.75, "car       |    1|123456.75"),
                Arguments.of("negative-values", "refund", -2, -3.5, "refund    |   -2|   -3.50"),
                Arguments.of("rounds-up", "nail", 1000, 0.006, "nail      | 1000|    0.01"),
                Arguments.of("rounds-down", "bolt", 7, 2.344, "bolt      |    7|    2.34"),
                Arguments.of("name-with-spaces", "blue pen", 4, 0.99, "blue pen  |    4|    0.99"),
                Arguments.of("name-with-percent", "100% {x}", 1, 1.0, "100% {x}  |    1|    1.00")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void formatRow(String name, String item, long qty, double price, String want) {
        assertEquals(want, Row.formatRow(item, qty, price));
    }
}
