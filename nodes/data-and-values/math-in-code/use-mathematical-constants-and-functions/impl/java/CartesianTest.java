// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CartesianTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("east", 1.0, 0.0, 1.0, 0.0),
                Arguments.of("north", 2.0, 90.0, 0.0, 2.0),
                Arguments.of("west", 2.0, 180.0, -2.0, 0.0),
                Arguments.of("diagonal", 1.0, 45.0, 0.7071067811865476, 0.7071067811865476),
                Arguments.of("thirty-degrees", 2.0, 30.0, 1.7320508075688772, 1.0),
                Arguments.of("zero-radius", 0.0, 123.0, 0.0, 0.0),
                Arguments.of("south", 1.0, 270.0, 0.0, -1.0),
                Arguments.of("negative-angle", 1.0, -90.0, 0.0, -1.0),
                Arguments.of("full-turn", 1.0, 360.0, 1.0, 0.0),
                Arguments.of("two-turns", 3.0, 720.0, 3.0, 0.0),
                Arguments.of("negative-radius", -1.0, 0.0, -1.0, 0.0),
                Arguments.of("large-radius", 1000000.0, 60.0, 500000.0, 866025.4037844386)
        );
    }

    static boolean close(double got, double want) {
        return Math.abs(got - want) <= Math.max(1e-9, 1e-12 * Math.abs(want));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void toCartesian(String name, double r, double degrees, double wantX, double wantY) {
        double[] p = Cartesian.toCartesian(r, degrees);
        assertTrue(close(p[0], wantX), "x = " + p[0]);
        assertTrue(close(p[1], wantY), "y = " + p[1]);
    }

    @Test
    void constants() {
        assertTrue(close(Math.atan2(0, -1), Math.PI));
        assertEquals(4.0, Math.sqrt(16));
        assertEquals(1024.0, Math.pow(2, 10));
        assertEquals(8, 2 ^ 10);
    }
}
