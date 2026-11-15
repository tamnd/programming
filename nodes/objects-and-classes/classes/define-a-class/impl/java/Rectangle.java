// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

import java.util.List;

/** A rectangle with a width and a height. */
final class Rectangle {
  // Private fields: only the code of this class can read or change them.
  private int width;
  private int height;

  Rectangle(int width, int height) {
    this.width = width;
    this.height = height;
  }

  int width() {
    return width;
  }

  int height() {
    return height;
  }

  int area() {
    return width * height;
  }

  int perimeter() {
    return 2 * (width + height);
  }

  /** Multiplies both sides by factor. This changes the instance. */
  void scale(int factor) {
    width *= factor;
    height *= factor;
  }

  @Override
  public String toString() {
    return "Rectangle(" + width + ", " + height + ")";
  }

  static List<Integer> rectangleReport(int width, int height, int factor) {
    Rectangle r = new Rectangle(width, height);
    int area = r.area();
    int perimeter = r.perimeter();
    r.scale(factor);
    return List.of(area, perimeter, r.area(), r.perimeter());
  }
}
