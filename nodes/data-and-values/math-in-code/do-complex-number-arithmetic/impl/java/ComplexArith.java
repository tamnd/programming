// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

/** Complex number arithmetic. Java has no complex type, so this file defines one. */
public final class ComplexArith {
    private ComplexArith() {}

    /** A complex number with a real part and an imaginary part. */
    public record Complex(double re, double im) {
        public Complex plus(Complex w) {
            return new Complex(re + w.re, im + w.im);
        }

        public Complex times(Complex w) {
            return new Complex(re * w.re - im * w.im, re * w.im + im * w.re);
        }

        /** Divides by the larger part of w first, so the formula cannot overflow. */
        public Complex div(Complex w) {
            if (Math.abs(w.re) >= Math.abs(w.im)) {
                double r = w.im / w.re;
                double t = w.re + w.im * r;
                return new Complex((re + im * r) / t, (im - re * r) / t);
            }
            double r = w.re / w.im;
            double t = w.re * r + w.im;
            return new Complex((re * r + im) / t, (im * r - re) / t);
        }

        public double abs() {
            return Math.hypot(re, im);
        }

        public double phase() {
            return Math.atan2(im, re);
        }
    }

    /** Returns {sum, product, quotient, abs(a), phase(a)} as 8 numbers. a and b are {re, im}. */
    public static double[] complexArithmetic(double[] a, double[] b) {
        Complex z = new Complex(a[0], a[1]);
        Complex w = new Complex(b[0], b[1]);
        Complex s = z.plus(w);
        Complex p = z.times(w);
        Complex q = z.div(w);
        return new double[] {s.re(), s.im(), p.re(), p.im(), q.re(), q.im(), z.abs(), z.phase()};
    }
}
