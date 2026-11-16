// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

/** Turns a distance and an angle in degrees into the coordinates x and y. */
public final class Cartesian {
    private Cartesian() {}

    /** Returns {x, y} at the distance r and the angle in degrees. */
    public static double[] toCartesian(double r, double degrees) {
        double t = Math.toRadians(degrees);
        return new double[] {r * Math.cos(t), r * Math.sin(t)};
    }
}
