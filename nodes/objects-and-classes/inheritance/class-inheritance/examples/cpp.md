```cpp run id=diamond
#include <iostream>

struct Device { int id = 0; Device() { std::cout << "Device\n"; } };
struct Reader : virtual Device { Reader() { std::cout << "Reader\n"; } };
struct Writer : virtual Device { Writer() { std::cout << "Writer\n"; } };
struct Disk : Reader, Writer { Disk() { std::cout << "Disk\n"; } }; // one shared Device

struct A { int id = 0; };
struct B : A {};
struct C : A {};
struct D : B, C {}; // two copies of A

int main()
{
    Disk d;
    d.id = 7; // one Device subobject, so the name is not ambiguous
    std::cout << d.Reader::id << ' ' << d.Writer::id << '\n';

    D x;
    x.B::id = 1;
    x.C::id = 2;
    std::cout << x.B::id << ' ' << x.C::id << '\n';
}
```

```output for=diamond
Device
Reader
Writer
Disk
7 7
1 2
```

```cpp fails expect=compile id=ambiguous
struct A { int id = 0; };
struct B : A {};
struct C : A {};
struct D : B, C {};

int main()
{
    D x;
    x.id = 3; // which A? B's or C's
}
```

- Verdict: native. C++ has multiple inheritance, with `public`, `protected` and `private` bases. Public inheritance makes the derived class a subtype of the base, while private inheritance reuses the code of the base without the subtype relation. Constructors run from the base to the derived class, and destructors in the reverse order.
- In a diamond, each ordinary base is a separate subobject, so `D` has two `A` parts, and the failing block cannot name `x.id` because it is ambiguous. A `virtual` base is shared: `Disk` has one `Device`, which the most derived class constructs once, before `Reader` and `Writer`.
