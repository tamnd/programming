```powershell run id=hierarchy
class Shape {
    [double] Area() { return 0 }
    [string] Describe() { return '{0} {1}' -f $this.GetType().Name, $this.Area() }
}

class Square : Shape {
    [double] $Side
    Square([double] $side) { $this.Side = $side }
    [double] Area() { return $this.Side * $this.Side }
}

class Tile : Square {
    [string] $Colour
    Tile([double] $side, [string] $colour) : base($side) { $this.Colour = $colour }
}

$shapes = [Shape[]] @([Square]::new(3), [Tile]::new(2, 'red'))
foreach ($shape in $shapes) { $shape.Describe() }
$shapes[1] -is [Square]
$shapes[1] -is [Shape]
[Shape] $view = [Square]::new(5)
$view.GetType().Name
$view.Area()
```

```output for=hierarchy
Square 9
Tile 4
True
True
Square
25
```

- Verdict: native. A class can inherit from one base class with `class Square : Shape`. The derived class can override a method of the base class.
- `-is` tests the dynamic type against the class and its base classes.
- A variable typed `[Shape]` holds a `Square`, and a call of `Area` uses the method of `Square`.
- A constructor calls the constructor of its base class with `: base(...)`.
