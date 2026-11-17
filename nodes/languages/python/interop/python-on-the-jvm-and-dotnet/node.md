---
tag: GQGZYA
kind: feature
owner: python
slug: python-on-the-jvm-and-dotnet
name: Python on the JVM and .NET
aliases: [GraalPy, Jython, IronPython, Python.NET, pythonnet, JPype, Python on the JVM, Python on .NET, Python and Java, Python and C#, clr.AddReference, java.type]
area: languages/python/interop
level: advanced
summary: >-
  GraalPy and Jython run Python on the JVM, and IronPython runs it on .NET. Python.NET and JPype connect CPython to a .NET or Java runtime in the same process. All of them call classes by name.
grammar: null
spec: {standard: null, url: "https://graalpy.org/python-developers/docs/", locator: "Interoperability; Call Java from Python; Types Mapping"}
see_also: [python/embedding-python, python/extension-modules, python/pypy-and-alternative-runtimes, runtime-reflection, global-interpreter-lock]
sources:
  - url: https://graalpy.org/python-developers/docs/
    title: GraalPy, documentation for Python developers
    locator: GraalPy Capabilities; Native Extensions Support; Interoperability; Call Java from Python; Passing Binary Data Between Java and Python; Multithreading; Types Mapping
    accessed: 2026-11-18
  - url: https://graalpy.org/jvm-developers/docs/
    title: GraalPy, documentation for JVM developers
    locator: Gradle Quick Start; Interoperability
    accessed: 2026-11-18
  - url: https://www.jython.org/
    title: Jython home page
    locator: What is Jython?
    accessed: 2026-11-18
  - url: https://ironpython.net/
    title: IronPython.net
    locator: home page
    accessed: 2026-11-18
  - url: https://github.com/IronLanguages/ironpython3
    title: IronPython 3, README
    locator: Examples; State of the Project; Differences with CPython
    accessed: 2026-11-18
  - url: https://pythonnet.github.io/
    title: Python.NET home page
    locator: introduction
    accessed: 2026-11-18
  - url: https://pythonnet.github.io/pythonnet/python.html
    title: Python.NET, Embedding .NET into Python
    locator: Loading a Runtime; Importing Modules; Using Classes; Using Generics; Overloaded and Generic Methods; Out and Ref parameters
    accessed: 2026-11-18
  - url: https://pythonnet.github.io/pythonnet/dotnet.html
    title: Python.NET, Embedding Python into .NET
    locator: introduction
    accessed: 2026-11-18
  - url: https://jpype.readthedocs.io/en/latest/userguide.html
    title: JPype User Guide
    locator: Introduction; Avoid Restarting the JVM; Java conversions; JByte, JShort, JInt, JLong; Name Mangling
    accessed: 2026-11-18
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Method.html
    title: Method (Java SE 25 and JDK 25)
    locator: invoke
    accessed: 2026-11-18
  - url: https://learn.microsoft.com/en-us/dotnet/fundamentals/reflection/overview
    title: Reflection in .NET
    locator: introduction
    accessed: 2026-11-18
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/interop/using-type-dynamic
    title: Using type dynamic, C#
    locator: introduction; Overload resolution with arguments of type dynamic; Dynamic language runtime
    accessed: 2026-11-18
  - url: https://docs.python.org/3/library/keyword.html
    title: keyword, Testing for Python keywords
    locator: keyword.iskeyword
    accessed: 2026-11-18
  - url: https://docs.python.org/3/builtins/stdtypes.html
    title: Built-in Types
    locator: Bytes Objects
    accessed: 2026-11-18
---

## Synopsis

```java run id=invoke
import java.lang.reflect.Method;

public class Main {
    public static void main(String[] args) throws Exception {
        // A bridge knows only the names that the Python code gives it.
        Class<?> type = Class.forName("java.math.BigInteger");
        Method valueOf = type.getMethod("valueOf", long.class);
        Object big = valueOf.invoke(null, 42L);

        Method shiftLeft = type.getMethod("shiftLeft", int.class);
        Method not = type.getMethod("not");
        System.out.println(shiftLeft.invoke(big, 128));
        System.out.println(not.invoke(big));
        System.out.println(big.getClass().getName());
    }
}
```

```output for=invoke
14291859410679415465461733512134264881152
-43
java.math.BigInteger
```

There are two ways to use Python with Java or .NET code. In the first way, a Python implementation runs on the virtual machine itself: GraalPy and Jython run on the Java virtual machine (JVM), and IronPython runs on .NET. In the second way, CPython runs as usual and a bridge loads the JVM or the .NET runtime into the same process: JPype is a bridge to Java, and Python.NET is a bridge to .NET. In both ways, Python code names a Java or .NET class, and the runtime finds the class, its methods and their parameter types at run time. The example shows this step in Java with reflection: it finds `BigInteger` and three of its methods by name, and then calls them.

## Semantics

**r1.** GraalPy is a Python implementation on the JVM. "GraalPy provides a Python 3.13 compliant runtime."[^1] "GraalPy can interoperate with Java and other Graal languages that are implemented on the Truffle framework."[^1] "This interoperability works in both directions. Python can call other languages, and other languages can call Python code."[^1] To use Java classes, GraalPy must run on the JVM: "The Java interoperability sections below require a JVM-based GraalPy runtime with the relevant Java classes on the classpath."[^1]

**r2.** GraalPy code gets a Java class from its name. The function `java.type` takes the full name of the class, for example `java.type("java.math.BigInteger")`.[^1] "You can import packages from the java namespace using conventional Python import syntax", for example `from java.util import ArrayList`.[^1] The module `java` also has `instanceof(obj, class)` and tests such as `is_function(obj)`.[^1] Examples: [`graalpy`](#graalpy), [`invoke`](#invoke).

**r3.** A Java method name can be a Python keyword. Python code cannot write `n.not()`, because `not` is a keyword. The function `keyword.iskeyword` tells if a name is a keyword: "Return True if s is a Python keyword."[^13] GraalPy says: "Java method names that are Python keywords must be accessed using `getattr`", for example `getattr(myBigInt, 'not')()`.[^1] JPype changes the name instead: "JPype handles naming conflicts between Java and Python by appending an underscore (_) to conflicting names."[^9] Example: [`keywords`](#keywords).

**r4.** A Java collection in GraalPy acts as the Python collection of the same kind. "For example, a Java ArrayList acts like a Python list, and a Java HashMap acts like a Python dict".[^1] "All foreign objects passed into Python have the Python type polyglot.ForeignObject or a subclass."[^1] An interop array inherits from `ForeignList` and `list`, and an interop hash inherits from `ForeignDict` and `dict`.[^1] "When a method is defined both in Python and on the foreign object, the Python’s method takes precedence."[^1] "To call the foreign method explicitly, use super(type_owning_the_python_method, foreign_object).method(*args)".[^1] Examples: [`proxy`](#proxy), [`remove`](#remove).

**r5.** A Java method in GraalPy takes only positional arguments. "An executable object can be executed as a function, but never with keyword arguments."[^1] "An instantiable object can be called just like a Python type, but never with keyword arguments."[^1] Example: [`keywordargs`](#keywordargs).

**r6.** A Java byte is signed, and a Python byte is not. The Java types `byte`, `short`, `int` and `long` "represent signed integers of varying sizes", and a byte has 8 bits.[^9] Thus a Java byte holds a value from -128 to 127. In Python, a `bytes` object holds integers "restricted such that 0 <= x < 256 (attempts to violate this restriction will trigger ValueError)".[^14] GraalPy says about a Java `byte[]`: "Only integral values that fit into a signed byte can be read from or written to such objects. Python, on the other hand, usually exposes binary data as unsigned byte values."[^1] To give the same bytes to Python without a copy, "Java byte arrays should be passed to Python using ByteBuffer.wrap(byte[])".[^1] Examples: [`signed`](#signed), [`range`](#range).

**r7.** GraalPy has a global interpreter lock (GIL). "GraalPy implements the Python global interpreter lock (GIL), which prevents any two threads from executing Python code at the same instant."[^1] "When methods in other languages are called from Python, no Python code is running while the other language executes."[^1] "To give other Python threads a chance to run at this point in time, GraalPy releases the GIL around such foreign method calls by default."[^1] The context manager `polyglot.gil_locked_during_interop` changes this for a block of code, for example to keep the GIL during short calls.[^1]

**r8.** GraalPy supports the C API of CPython, but not its binary interface. "The support extends only to the API, not the binary interface (ABI), so extensions built for CPython are not binary compatible with GraalPy."[^1] "Packages that use the native API must be built and installed with GraalPy, and the prebuilt wheels for CPython from pypi.org cannot be used."[^1] Thus an [extension module](/python/extension-modules/) must be built again for GraalPy.

**r9.** A Java program embeds GraalPy through the polyglot API. The program adds the artifacts `org.graalvm.polyglot:polyglot` and `org.graalvm.python:python-embedding`, makes a context with `GraalPyResources.createContext()`, and calls `context.eval` with the language `python` and the source code.[^2] The quick start program prints "Hello Python!".[^2]

**r10.** Jython is an older Python implementation on the JVM. "The Jython project provides implementations of Python in Java, providing to Python the benefits of running on the JVM and access to classes written in Java."[^3] "The current release (a Jython 2.7.x) only supports Python 2 (sorry)."[^3] Jython code imports Java classes as modules, for example `from java.lang import System`, and a Java program runs Python code with `PythonInterpreter.exec`.[^3]

**r11.** IronPython is a Python implementation on .NET. "IronPython is an open-source implementation of the Python programming language that is tightly integrated with .NET. IronPython can use .NET and Python libraries, and other .NET languages can use Python code just as easily."[^5] "The current target is Python 3.4, although features and behaviors from later versions may be included."[^5] The home page lists the release IronPython 3.4.2.[^4] IronPython code loads an assembly with `clr.AddReference` and then imports from its namespace. A C# program runs Python code with `IronPython.Hosting.Python.CreateEngine()`, and gets a Python function as a `dynamic` value.[^5] Example: [`dynamic`](#dynamic).

**r12.** The C# type `dynamic` moves the checks from compile time to run time. "The dynamic type is a static type, but an object of type dynamic bypasses static type checking."[^12] "Overload resolution occurs at run time instead of at compile time if one or more of the arguments in a method call have the type dynamic, or if the receiver of the method call is of type dynamic."[^12] "The dynamic language runtime (DLR) provides the infrastructure that supports the dynamic type in C#, and also the implementation of dynamic programming languages such as IronPython and IronRuby."[^12] Example: [`dynamic`](#dynamic).

**r13.** Python.NET connects CPython to a .NET runtime. "Note that this package does not implement Python as a first-class CLR language - it does not produce managed code (IL) from Python code. Rather, it is an integration of the CPython engine with a .NET runtime."[^6] Thus Python code on Python.NET can use the usual C extension modules. "Python.NET is currently compatible and tested with Python releases 3.11 - 3.15."[^6] It can load Mono, the .NET Framework or .NET Core, and "The runtime must be configured before clr is imported, otherwise the default runtime will be initialized and used."[^7] The call `pythonnet.load("coreclr")` selects .NET Core.[^7]

**r14.** In Python.NET, a .NET namespace acts as a Python package. "Python.NET allows CLR namespaces to be treated essentially as Python packages."[^7] An assembly must be loaded first: "all assemblies have to be loaded explicitly with AddReference."[^7] For example, `clr.AddReference("System.Windows.Forms")` comes before `from System.Windows.Forms import Form`.[^7]

**r15.** Python.NET chooses an overload from the arguments, and the program can choose one by its parameter types. "In many cases, Python.NET can determine the correct constructor to call automatically based on the arguments."[^7] To choose a constructor or a method, the program gives the parameter types to `__overloads__`, for example `String.__overloads__[Char, Int32]('A', 10)`.[^7] "For compatibility with IronPython, the same functionality is available with the Overloads attribute."[^7] Examples: [`reflect`](#reflect), [`remove`](#remove).

**r16.** A .NET generic type must get its type arguments before it can make an object. "A generic type must be bound to create a concrete type before it can be instantiated. Generic types support the subscript syntax to create bound types".[^7] For example, `Dictionary[String, Int32]()` makes a dictionary from strings to 32-bit integers.[^7] A generic method takes its type arguments in the same way, as in `someobject.SomeGenericMethod[UInt32](10)`.[^7] Example: [`generic`](#generic).

**r17.** In Python.NET, an `out` or `ref` parameter becomes a part of the return value. "When a managed method has out or ref parameters, the arguments appear as normal arguments in Python, but the return value of the method is modified."[^7] If the method also returns a value, "the method returns a tuple containing the return value followed by the out/ref parameter values", as in `found, new_value = dictionary.TryGetValue(key, value)`.[^7] With .NET reflection, the value of an `out` parameter comes back in the array of arguments. Example: [`reflect`](#reflect).

**r18.** A .NET program can also embed CPython through Python.NET. The program references `Python.Runtime.dll`, calls `PythonEngine.Initialize()`, and imports a module with `PyModule.Import(name)`.[^8] "When embedding Python in a managed application, one has to manage the GIL in just the same way you would when embedding Python in a C or C++ application."[^8] The code gets the GIL with `Py.GIL()` and releases it when it disposes the result.[^8]

**r19.** JPype connects CPython to the JVM. "Unlike Jython, which reimplements Python on the Java Virtual Machine (JVM), JPype bridges Python and Java at the native level using the Java Native Interface (JNI)."[^9] "With JPype, both virtual machines run in the same process, sharing the same memory space and threads."[^9] The program starts the JVM with `jpype.startJVM()`, and gives the classpath "using the classpath argument in the startJVM() function or dynamically through the addClassPath() method prior to JVM startup."[^9] "It is important to note that the JVM cannot be restarted once it has been shut down."[^9]

**r20.** JPype ranks the conversions of each argument to choose an overload. "Conversions have three different degrees. These are: exact, derived, implicit, and explicit."[^9] "An implicit conversion is one that Java would perform automatically."[^9] An explicit conversion needs a cast: "To request an explicit conversion an object must be “cast” using a cast operator @."[^9] When no overload matches, JPype raises a `TypeError` with the message "No matching overloads found", and the program can give a Java type, as in `obj.getData(JInt(1))`.[^9]

**r21.** On the Java side, a bridge calls a method with reflection. `Method.invoke` "Invokes the underlying method represented by this Method object, on the specified object with the specified parameters."[^10] "Individual parameters are automatically unwrapped to match primitive formal parameters".[^10] It throws `IllegalArgumentException` "if the number of actual and formal parameters differ" or if an argument cannot be converted to its parameter type, and `InvocationTargetException` "if the underlying method throws an exception."[^10] On .NET, "The classes in the System.Reflection namespace, together with System.Type, enable you to obtain information about loaded assemblies and the types defined within them".[^11] Examples: [`invoke`](#invoke), [`errors`](#errors), [`reflect`](#reflect).

## Examples

### GraalPy code that uses Java classes

This code runs only on GraalPy on the JVM. The sandbox of this site runs CPython, which has no module `java`. The code is from the GraalPy documentation.[^1]

```python norun reason=platform id=graalpy
import java
from java.util import ArrayList

BigInteger = java.type("java.math.BigInteger")
n = BigInteger.valueOf(42)
print(n.shiftLeft(128))
print(getattr(n, "not")())        # 'not' is a Python keyword

items = ArrayList()
items.extend([5, 6, 7])
items.remove(7)                   # list.remove: the value 7
super(list, items).remove(0)      # ArrayList.remove(int): the index 0
print(items == [6], java.instanceof(items, ArrayList))
```

### Java method names that are Python keywords

The example tests some method names of `java.math.BigInteger` with `keyword.iskeyword` (rule r3).

```python run id=keywords
import keyword

# Some method names of java.math.BigInteger.
java_names = ["not", "and", "or", "andNot", "xor", "abs"]

for name in java_names:
    if keyword.iskeyword(name):
        print(f"{name:6} keyword  GraalPy: getattr(n, {name!r})()  JPype: n.{name}_()")
    else:
        print(f"{name:6} name     n.{name}()")
```

```output for=keywords
not    keyword  GraalPy: getattr(n, 'not')()  JPype: n.not_()
and    keyword  GraalPy: getattr(n, 'and')()  JPype: n.and_()
or     keyword  GraalPy: getattr(n, 'or')()  JPype: n.or_()
andNot name     n.andNot()
xor    name     n.xor()
abs    name     n.abs()
```

### Which `remove` is called

The example is a model of a Java list in GraalPy (rule r4). The class `list` comes before the Java methods in the method resolution order, so `remove` is the Python method. The call through `super(list, items)` skips `list` and finds the Java method.

```python run id=proxy
class JavaArrayList:
    """A model of the Java methods of java.util.ArrayList."""

    def remove(self, index):
        # Java remove(int) removes the element at an index.
        return list.pop(self, index)


class ForeignList(list, JavaArrayList):
    """A model of a Java list in GraalPy: list comes first in the MRO."""


names = [cls.__name__ for cls in ForeignList.__mro__]
print(names)

items = ForeignList([5, 6, 7])
items.remove(7)                 # list.remove: the value 7
print(items)
super(list, items).remove(0)    # the Java method: the index 0
print(items)
```

```output for=proxy
['ForeignList', 'list', 'JavaArrayList', 'object']
[5, 6]
[6]
```

### Two overloads of `remove` in Java

In Java, `ArrayList<Integer>` has two methods named `remove`. The argument type chooses one of them at compile time. A bridge must make the same choice at run time from a Python `int` (rules r15 and r20).

```java run id=remove
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<>(List.of(5, 6, 7));
        list.remove(Integer.valueOf(7)); // remove(Object): the value 7
        System.out.println(list);
        list.remove(0); // remove(int): the element at index 0
        System.out.println(list);
    }
}
```

```output for=remove
[5, 6]
[6]
```

### No keyword arguments

A Java method has no Python names for its parameters (rule r5). A Python function with positional-only parameters is a model of it.

```python run id=keywordargs
def value_of(value, /):
    """A model of a Java method: its parameters have no Python names."""
    return value


print(value_of(42))
try:
    value_of(value=42)
except TypeError:
    print("TypeError: no keyword arguments")
```

```output for=keywordargs
42
TypeError: no keyword arguments
```

### Signed and unsigned bytes

The same 8 bits are 200 as a Python byte and -56 as a Java byte (rule r6).

```python run id=signed
data = bytes([200, 1, 255])
print(list(data))

# A Java byte is signed: it holds -128 to 127.
as_java = [b - 256 if b > 127 else b for b in data]
print(as_java)

# The same 8 bits, read back as an unsigned Python byte.
print(bytes(b & 0xFF for b in as_java) == data)
print(int.from_bytes(data[:1], signed=True))
```

```output for=signed
[200, 1, 255]
[-56, 1, -1]
True
-56
```

A list of Java byte values cannot become a `bytes` object, because `bytes` accepts only 0 to 255.[^14]

```python fails expect=exception:ValueError id=range
java_bytes = [-56, 1, -1]
bytes(java_bytes)
```

### Reflection errors in Java

`Method.invoke` unwraps an `Integer` for an `int` parameter. It throws `IllegalArgumentException` for a wrong number or type of arguments, and it wraps an exception of the method in `InvocationTargetException` (rule r21). A bridge turns these errors into Python exceptions.

```java run id=errors
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Main {
    public static int half(int n) {
        if (n % 2 != 0) {
            throw new ArithmeticException("odd: " + n);
        }
        return n / 2;
    }

    public static void main(String[] args) throws Exception {
        Method half = Main.class.getMethod("half", int.class);
        System.out.println(half.invoke(null, Integer.valueOf(10)));
        try {
            half.invoke(null, 1, 2);
        } catch (IllegalArgumentException e) {
            System.out.println("IllegalArgumentException");
        }
        try {
            half.invoke(null, "ten");
        } catch (IllegalArgumentException e) {
            System.out.println("IllegalArgumentException");
        }
        try {
            half.invoke(null, 7);
        } catch (InvocationTargetException e) {
            System.out.println("cause: " + e.getCause());
        }
    }
}
```

```output for=errors
5
IllegalArgumentException
IllegalArgumentException
cause: java.lang.ArithmeticException: odd: 7
```

### Reflection in .NET

The example finds one overload of `Math.Max` by its parameter types, makes a `StringBuilder` from the name of its type, and calls `TryGetValue`, whose `out` parameter comes back in the array of arguments (rules r15, r17 and r21).

```csharp run id=reflect
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

class Program
{
    static void Main()
    {
        // Find a method by its name and its parameter types.
        MethodInfo max = typeof(Math).GetMethod("Max", new[] { typeof(int), typeof(int) })!;
        Console.WriteLine(max.Invoke(null, new object[] { 3, 7 }));
        Console.WriteLine(typeof(Math).GetMethods().Count(m => m.Name == "Max") > 1);

        // Make an object from the name of its type.
        Type type = Type.GetType("System.Text.StringBuilder")!;
        object builder = Activator.CreateInstance(type)!;
        type.GetMethod("Append", new[] { typeof(string) })!.Invoke(builder, new object[] { "abc" });
        Console.WriteLine(builder);

        // An out parameter comes back in the argument array.
        var ages = new Dictionary<string, int> { ["ada"] = 36 };
        MethodInfo tryGet = ages.GetType().GetMethod("TryGetValue")!;
        object?[] arguments = { "ada", null };
        object? found = tryGet.Invoke(ages, arguments);
        Console.WriteLine($"{found} {arguments[1]}");
    }
}
```

```output for=reflect
7
True
abc
True 36
```

### `dynamic` in C#

A C# program gets IronPython functions as `dynamic` values (rule r11). The run-time binder checks each call, and it chooses an overload from the type of the value at run time (rule r12).

```csharp run id=dynamic
using System;
using Microsoft.CSharp.RuntimeBinder;

class Greeter
{
    public string Hello(string name) => "hello, " + name;
}

class Program
{
    static string Show(int value) => "int " + value;
    static string Show(string value) => "string " + value;

    static void Main()
    {
        dynamic greeter = new Greeter();
        Console.WriteLine(greeter.Hello("world"));

        // The compiler does not check this call. The run-time binder does.
        try
        {
            greeter.Goodbye();
        }
        catch (RuntimeBinderException)
        {
            Console.WriteLine("RuntimeBinderException");
        }

        // The overload is chosen at run time, from the type of the value.
        dynamic value = 1;
        Console.WriteLine(Show(value));
        value = "one";
        Console.WriteLine(Show(value));
    }
}
```

```output for=dynamic
hello, world
RuntimeBinderException
int 1
string one
```

### Generic types in Python.NET

The example is a model of the subscript syntax of Python.NET for a generic type (rule r16). The unbound type cannot make an object, and it needs the correct number of type arguments.

```python run id=generic
class GenericType:
    """A model of an unbound .NET generic type, such as Dictionary`2."""

    def __init__(self, name, arity):
        self.name, self.arity = name, arity

    def __getitem__(self, args):
        args = args if isinstance(args, tuple) else (args,)
        if len(args) != self.arity:
            raise TypeError(f"{self.name} needs {self.arity} type arguments")
        return BoundType(self.name, args)

    def __call__(self, *args):
        raise TypeError(f"bind the type arguments of {self.name} first")


class BoundType:
    def __init__(self, name, args):
        self.name, self.args = name, args

    def __call__(self):
        names = ", ".join(t.__name__ for t in self.args)
        return f"new {self.name}<{names}>"


Dictionary = GenericType("Dictionary", 2)
print(Dictionary[str, int]())
for attempt in (lambda: Dictionary(), lambda: Dictionary[str]):
    try:
        attempt()
    except TypeError as error:
        print("TypeError:", error)
```

```output for=generic
new Dictionary<str, int>
TypeError: bind the type arguments of Dictionary first
TypeError: Dictionary needs 2 type arguments
```

## Pitfalls

- **A Python version that is old.** Jython supports only Python 2, and IronPython targets Python 3.4.[^3][^5] Code that uses newer syntax or modules does not run on them.
- **CPython wheels on GraalPy.** "the prebuilt wheels for CPython from pypi.org cannot be used" with GraalPy, so packages with C code must be built for it (rule r8).[^1]
- **A method that has two meanings.** On a Java list in GraalPy, `remove(7)` is the Python method, which removes a value. The Java method `remove(int)` removes the element at an index (rule r4), as in [`proxy`](#proxy) and [`remove`](#remove).
- **Byte values out of range.** A Java byte from -128 to 127 is not a Python byte from 0 to 255 (rule r6). Use `ByteBuffer.wrap` and `memoryview` to share the bytes, or convert each value as in [`signed`](#signed).[^1]
- **A runtime that is chosen too late.** In Python.NET, "The runtime must be configured before clr is imported, otherwise the default runtime will be initialized and used."[^7]
- **An assembly that is not loaded.** Python.NET does not load an assembly from the name of a namespace; "all assemblies have to be loaded explicitly with AddReference."[^7]
- **A JVM that is shut down.** In JPype, "the JVM cannot be restarted once it has been shut down."[^9] Start it once, and keep it for the life of the program.
- **Calls without the GIL from .NET.** A .NET program that embeds Python.NET must hold the GIL with `Py.GIL()` before it uses Python objects (rule r18).[^8]

## Comparison

The bridges JPype and Python.NET have the same structure as a CPython program that [embeds](/python/embedding-python/) another runtime or that uses [extension modules](/python/extension-modules/): the two runtimes share one process, and the bridge converts the values at each call.[^9][^6] GraalPy, Jython and IronPython are instead [other Python runtimes](/python/pypy-and-alternative-runtimes/), and their objects are objects of the virtual machine.[^1][^3][^5] In C#, a program calls Python objects through `dynamic` and the DLR, while Java code calls GraalPy through the polyglot API.[^12][^2] On every platform, the bridge finds classes and methods by name with [reflection](/runtime-reflection/).[^10][^11]

## References

[^1]: [GraalPy, documentation for Python developers](https://graalpy.org/python-developers/docs/)
[^2]: [GraalPy, documentation for JVM developers](https://graalpy.org/jvm-developers/docs/)
[^3]: [Jython home page](https://www.jython.org/)
[^4]: [IronPython.net](https://ironpython.net/)
[^5]: [IronPython 3, README](https://github.com/IronLanguages/ironpython3)
[^6]: [Python.NET home page](https://pythonnet.github.io/)
[^7]: [Python.NET, Embedding .NET into Python](https://pythonnet.github.io/pythonnet/python.html)
[^8]: [Python.NET, Embedding Python into .NET](https://pythonnet.github.io/pythonnet/dotnet.html)
[^9]: [JPype User Guide](https://jpype.readthedocs.io/en/latest/userguide.html)
[^10]: [Method (Java SE 25 and JDK 25)](https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/reflect/Method.html)
[^11]: [Reflection in .NET](https://learn.microsoft.com/en-us/dotnet/fundamentals/reflection/overview)
[^12]: [Using type dynamic, C#](https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/interop/using-type-dynamic)
[^13]: [keyword, Testing for Python keywords](https://docs.python.org/3/library/keyword.html)
[^14]: [Built-in Types](https://docs.python.org/3/builtins/stdtypes.html)
