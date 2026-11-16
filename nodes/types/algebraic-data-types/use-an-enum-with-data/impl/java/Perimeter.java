// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
import java.util.List;
import java.util.Optional;
import java.util.OptionalInt;

final class Perimeter {
  private Perimeter() {}

  /** A sealed interface lists all of its variants; each record carries its own data. */
  sealed interface Shape permits Square, Rectangle, Triangle {}

  record Square(int side) implements Shape {}

  record Rectangle(int width, int height) implements Shape {}

  record Triangle(int a, int b, int c) implements Shape {}

  /** Builds a shape, or returns an empty Optional when the data is not a valid shape. */
  static Optional<Shape> makeShape(String kind, List<Integer> sides) {
    if (sides.stream().anyMatch(s -> s <= 0)) {
      return Optional.empty();
    }
    int n = sides.size();
    if (kind.equals("square") && n == 1) {
      return Optional.of(new Square(sides.get(0)));
    }
    if (kind.equals("rectangle") && n == 2) {
      return Optional.of(new Rectangle(sides.get(0), sides.get(1)));
    }
    if (kind.equals("triangle") && n == 3) {
      int a = sides.get(0);
      int b = sides.get(1);
      int c = sides.get(2);
      if (a < b + c && b < a + c && c < a + b) {
        return Optional.of(new Triangle(a, b, c));
      }
    }
    return Optional.empty();
  }

  /** The switch needs no default: the compiler knows all variants of Shape. */
  static int shapePerimeter(Shape shape) {
    return switch (shape) {
      case Square(int side) -> 4 * side;
      case Rectangle(int width, int height) -> 2 * (width + height);
      case Triangle(int a, int b, int c) -> a + b + c;
    };
  }

  static OptionalInt perimeter(String kind, List<Integer> sides) {
    return makeShape(kind, sides)
        .map(s -> OptionalInt.of(shapePerimeter(s)))
        .orElse(OptionalInt.empty());
  }
}
