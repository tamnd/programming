// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ComplexArithTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("textbook", new double[]{3.0, 4.0}, new double[]{1.0, -2.0}, new double[]{4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122}),
                Arguments.of("rosetta", new double[]{1.0, 1.0}, new double[]{3.14159, 1.25}, new double[]{4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483}),
                Arguments.of("real-only", new double[]{6.0, 0.0}, new double[]{3.0, 0.0}, new double[]{9.0, 0.0, 18.0, 0.0, 2.0, 0.0, 6.0, 0.0}),
                Arguments.of("imaginary-unit", new double[]{0.0, 1.0}, new double[]{0.0, 1.0}, new double[]{0.0, 2.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.5707963267948966}),
                Arguments.of("negative-real", new double[]{-2.0, 0.0}, new double[]{1.0, 0.0}, new double[]{-1.0, 0.0, -2.0, 0.0, -2.0, 0.0, 2.0, 3.141592653589793}),
                Arguments.of("zero", new double[]{0.0, 0.0}, new double[]{2.0, -3.0}, new double[]{2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}),
                Arguments.of("negative-phase", new double[]{1.0, -1.0}, new double[]{-1.0, -1.0}, new double[]{0.0, -2.0, -2.0, 0.0, 0.0, 1.0, 1.4142135623730951, -0.7853981633974483}),
                Arguments.of("imaginary-divisor", new double[]{2.0, 4.0}, new double[]{0.0, 2.0}, new double[]{2.0, 6.0, -8.0, 4.0, 2.0, -1.0, 4.47213595499958, 1.1071487177940904}),
                Arguments.of("large-divisor", new double[]{1e+150, 1e+150}, new double[]{1e+155, 1e+155}, new double[]{1.00001e+155, 1.00001e+155, 0.0, 2e+305, 9.999999999999999e-06, 0.0, 1.414213562373095e+150, 0.7853981633974483})
        );
    }

    static boolean close(double got, double want) {
        return Math.abs(got - want) <= Math.max(1e-9, 1e-12 * Math.abs(want));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void complexArithmetic(String name, double[] a, double[] b, double[] want) {
        double[] got = ComplexArith.complexArithmetic(a, b);
        assertEquals(8, got.length);
        for (int i = 0; i < 8; i++) {
            assertTrue(close(got[i], want[i]), "item " + i + " = " + got[i]);
        }
    }

    @Test
    void recordEquality() {
        assertEquals(new ComplexArith.Complex(-1, 0), new ComplexArith.Complex(0, 1).times(new ComplexArith.Complex(0, 1)));
    }
}
