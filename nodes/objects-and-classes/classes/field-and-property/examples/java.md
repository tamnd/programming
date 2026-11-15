```java run id=getters-and-setters
public class Main {
  static final class Rectangle {
    private int width; // fields are private
    private int height;

    Rectangle(int width, int height) {
      setWidth(width);
      this.height = height;
    }

    int getWidth() { // a getter
      return width;
    }

    void setWidth(int width) { // a setter with a check
      if (width < 0) {
        throw new IllegalArgumentException("negative width");
      }
      this.width = width;
    }

    int getArea() { // a computed value, with no field
      return width * height;
    }
  }

  record Size(int width, int height) {} // a record: final fields and accessors

  public static void main(String[] args) {
    Rectangle r = new Rectangle(3, 4);
    System.out.println(r.getWidth() + " " + r.getArea());
    r.setWidth(5);
    System.out.println(r.getArea());
    try {
      r.setWidth(-1);
    } catch (IllegalArgumentException e) {
      System.out.println(e.getMessage());
    }
    Size s = new Size(3, 4);
    System.out.println(s.width() + " " + s);
  }
}
```

```output for=getters-and-setters
3 12
20
negative width
3 Size[width=3, height=4]
```

- Verdict: idiom. Java has fields and no property syntax. The convention from JavaBeans is a private field with public methods `getWidth` and `setWidth`, and tools such as serializers find properties by these names.
- A computed property is a method with no field behind it, such as `getArea`.
- A record declares its fields in the header and gets one accessor method for each, named like the field: `s.width()`, not `s.getWidth()`. The fields are final, so a record has no setters.
