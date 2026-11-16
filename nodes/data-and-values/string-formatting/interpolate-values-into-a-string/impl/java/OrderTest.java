// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class OrderTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("three-items", "Ada", 3, 1.5, "Order {Ada}: 3 items, total 4.50"),
                Arguments.of("one-item", "Grace", 1, 19.99, "Order {Grace}: 1 item, total 19.99"),
                Arguments.of("many-items", "Linus", 12, 0.25, "Order {Linus}: 12 items, total 3.00"),
                Arguments.of("zero-items", "Ada", 0, 9.99, "Order {Ada}: 0 items, total 0.00"),
                Arguments.of("one-item-plural", "Bob", 1, 0.5, "Order {Bob}: 1 item, total 0.50"),
                Arguments.of("empty-name", "", 2, 1.0, "Order {}: 2 items, total 2.00"),
                Arguments.of("braces-in-name", "{x}", 2, 1.25, "Order {{x}}: 2 items, total 2.50"),
                Arguments.of("percent-in-name", "50% off", 4, 2.5, "Order {50% off}: 4 items, total 10.00"),
                Arguments.of("dollar-in-name", "${HOME}", 1, 3.0, "Order {${HOME}}: 1 item, total 3.00"),
                Arguments.of("product-rounds", "Cy", 3, 19.99, "Order {Cy}: 3 items, total 59.97"),
                Arguments.of("large-total", "Dee", 1000, 1234.56, "Order {Dee}: 1000 items, total 1234560.00"),
                Arguments.of("negative-price", "Refund", 2, -4.5, "Order {Refund}: 2 items, total -9.00")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void describeOrder(String name, String who, long count, double price, String want) {
        assertEquals(want, Order.describeOrder(who, count, price));
    }
}
