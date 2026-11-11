---
tag: TRTN8M
kind: concept
slug: type-erasure
name: Type erasure
aliases: [erasure, reified generics, reification]
area: types/erasure-and-reification
level: core
summary: >-
  Type erasure removes type information when a program is compiled. Reification keeps it for the run time. Java and TypeScript erase type arguments, and C# and Go keep them.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1998
  people: [Gilad Bracha, Martin Odersky, David Stoutamire, Philip Wadler]
  note: GJ added generic types to Java and translated them by erasure, so that no information about type parameters was kept at run time.
see_also: [parametric-polymorphism, subtyping, static-and-dynamic-typing, interfaces, higher-order-functions, type-conversion]
same_as:
  wikidata: Q7860901
  wikipedia: Type_erasure
sources:
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.6
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.6 Type Erasure
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.7
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.7 Reifiable Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.8
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.8 Raw Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.12.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.12.2 Variables of Reference Type (heap pollution)
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-4.html#jvms-4.7.9
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 4.7.9 The Signature Attribute
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.12.4.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.12.4.5 Create Frame, Synchronize, Transfer Control (the example of a bridge method)
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-4.html#jvms-4.6
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 4.6 Methods (the flag ACC_BRIDGE)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.8.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.4.8.3 Requirements in Overriding and Hiding (methods with the same name must have different erasures)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.10.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.10.1 Array Creation Expressions
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.8.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 15.8.2 Class Literals
  - url: https://docs.oracle.com/javase/tutorial/java/generics/restrictions.html
    title: The Java Tutorials, Restrictions on Generics
    locator: Cannot Instantiate Generic Types with Primitive Types, Cannot Create Instances of Type Parameters, Cannot Use Casts or instanceof with Parameterized Types, Cannot Overload a Method Where the Formal Parameter Types of Each Overload Erase to the Same Raw Type
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/generics-in-the-run-time
    title: C# programming guide, Generics in the runtime
    locator: the introduction, value types as type arguments, reference types as type arguments
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/reflection-and-attributes/generics-and-reflection
    title: C# programming guide, Generics and reflection
    locator: the introduction (the runtime has generic type information at run time)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/constraints-on-type-parameters
    title: C# programming guide, Constraints on type parameters
    locator: the table of constraints (where T new())
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/default#default-operator
    title: C# language reference, default value expressions
    locator: default operator
  - paper: doi:10.1145/378795.378797
    title: Andrew Kennedy and Don Syme, Design and implementation of generics for the .NET Common Language Runtime, PLDI 2001
    locator: the abstract (exact run-time types, just-in-time type specialization, code sharing based on representation)
  - url: https://doc.rust-lang.org/book/ch10-01-syntax.html#performance-of-code-using-generics
    title: The Rust Programming Language, 10.1 Generic Data Types
    locator: Performance of Code Using Generics
  - url: https://doc.rust-lang.org/reference/types/trait-object.html#r-type.trait-object.unsized
    title: The Rust Reference, Trait objects
    locator: r-type.trait-object.unsized (a pointer to a trait object holds a pointer to the value and a pointer to a vtable)
  - url: https://doc.rust-lang.org/book/ch18-02-trait-objects.html#performing-dynamic-dispatch
    title: The Rust Programming Language, 18.2 Using Trait Objects to Abstract over Shared Behavior
    locator: Performing Dynamic Dispatch
  - url: https://timsong-cpp.github.io/cppwp/n4950/temp.spec.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[temp.spec.general], paragraphs 1 and 2"
  - url: https://timsong-cpp.github.io/cppwp/n4950/func.wrap.func
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[func.wrap.func], paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/any.class
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[any.class], paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/any.nonmembers
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[any.nonmembers], the Throws clause of any_cast"
  - url: https://github.com/golang/proposal/blob/master/design/generics-implementation-dictionaries-go1.18.md
    title: Go 1.18 Implementation of Generics via Dictionaries and Gcshape Stenciling
    locator: the overview, Gcshapes and Dictionary Format
  - url: https://www.typescriptlang.org/docs/handbook/2/basic-types.html#erased-types
    title: TypeScript Handbook, The Basics
    locator: Erased Types
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#type-assertions
    title: TypeScript Handbook, Everyday Types
    locator: Type Assertions
  - url: https://docs.python.org/3.14/library/stdtypes.html#types-genericalias
    title: The Python Standard Library, Built-in Types, Python 3.14
    locator: Generic Alias Type (isinstance, the note that the runtime does not enforce annotations, erasure during object creation) and __args__
  - url: https://docs.python.org/3.14/library/typing.html
    title: The Python Standard Library, typing, Python 3.14
    locator: the note at the top of the page
  - url: https://kotlinlang.org/docs/generics.html#type-erasure
    title: Kotlin documentation, Generics, in, out, where
    locator: Type erasure and Generics type checks and casts (reified type parameters of inline functions)
  - paper: doi:10.1145/286936.286957
    title: Gilad Bracha, Martin Odersky, David Stoutamire and Philip Wadler, Making the future safe for the past, Adding genericity to the Java programming language, OOPSLA 1998
    locator: the abstract (the translation erases type parameters, maps type variables to their bounds and inserts casts)
  - url: https://homepages.inf.ed.ac.uk/wadler/gj/
    title: Philip Wadler, GJ, A Generic Java Language Extension
    locator: the timeline at the top of the page and the paragraph Efficient translation
  - url: https://jcp.org/en/jsr/detail?id=14
    title: "JSR 14: Add Generic Types To The Java Programming Language"
    locator: the description and the Final Release date (30 Sep 2004)
  - url: https://go.dev/doc/devel/release
    title: Go release history
    locator: go1.18 (released 2022-03-15)
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_calling_generic_methods?view=powershell-7.5
    title: PowerShell documentation, about_Calling_Generic_Methods
    locator: Short description and Long description
---

## Idea

A compiler knows the type of each expression. The question is how much of that knowledge the running program keeps. Type erasure removes it. The compiler checks the types, and then it emits code that has no record of some of them. Reification keeps it. A reified type is one that the running program can see, test and pass around.

Most languages keep some types and erase others. A Java object knows its class, but not the type arguments of its class. The JLS calls the types that keep all their information at run time reifiable types.[^2] A C# object knows its class and its type arguments.[^12][^13] A TypeScript program keeps no static types at all, because the compiler removes every type annotation.[^25]

The Java program below asks two questions. The first asks for the declared type of the field `names`. The second asks the object in the field for its type parameter.

```java run id=idea
import java.util.ArrayList;
import java.util.List;

public class Main {
    static List<String> names = new ArrayList<>();

    public static void main(String[] args) throws Exception {
        System.out.println(Main.class.getDeclaredField("names").getGenericType());
        System.out.println(names.getClass().getTypeParameters()[0]);
    }
}
```

```output for=idea
java.util.List<java.lang.String>
E
```

The declaration keeps its full type, because the compiler writes it into the Signature attribute of the class file. The virtual machine does not use this attribute to check anything, but reflection can read it.[^5] The object knows only its class `ArrayList` and the name `E` of its type parameter. It has no record of `String`. [[parametric-polymorphism]] describes the ways to compile generic code. This page is about what each way leaves for the run time.

## How it works

**Erasure in Java.** The JLS defines type erasure as a mapping from types to types that are never parameterized types or type variables. The erasure of `List<String>` is `List`. The erasure of a type variable is the erasure of its leftmost bound, so `T` becomes `Object` and `T extends Comparable<T>` becomes `Comparable`.[^1] The compiler checks the program with the full types. Then it compiles each generic class once, with the erased types. This follows GJ, the design that Java generics came from. The GJ translation erases all type parameters, maps type variables to their bounds, and inserts casts where they are needed.[^30]

A reifiable type keeps all its information at run time. The reifiable types are primitive types, types that are not generic, and raw types. Parameterized types whose type arguments are all unbounded wildcards, such as `List<?>`, are reifiable too. So are arrays of these types.[^2] Some operations need the type at run time, so Java rejects them for a type that is not reifiable:

- An array creation, such as `new T[n]` or `new List<String>[n]`, is a compile-time error.[^9]
- A class literal, such as `T.class`, is a compile-time error.[^10]
- `new T()` does not compile, and `instanceof ArrayList<Integer>` does not compile, because the run time does not know the type argument.[^11]
- Two methods of one class with the same name must have different erasures. So `total(List<Integer>)` and `total(List<String>)` cannot both exist.[^8]

A raw type, such as `List` with no type arguments, exists only for compatibility with code that was written before generics.[^3] Through a raw type or an unchecked cast, a variable of a parameterized type can refer to an object that is not of that type. The JLS calls this heap pollution. The run time cannot detect it, because type variables are not reified.[^4]

**Bridge methods.** Erasure can change the signature of a method that implements another method. A class that implements `Comparator<String>` declares `compare(String, String)`. After erasure, the method of the interface is `compare(Object, Object)`. So the compiler adds a bridge method with the erased signature. The bridge method casts its arguments and calls the real method.[^6] The class file marks it with the flag `ACC_BRIDGE`.[^7]

```java run id=bridge
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Comparator;

public class Main {
    static class ByLength implements Comparator<String> {
        public int compare(String a, String b) {
            return a.length() - b.length();
        }
    }

    public static void main(String[] args) {
        Method[] methods = ByLength.class.getDeclaredMethods();
        Arrays.sort(methods, Comparator.comparing(Method::toString));
        for (Method m : methods) {
            System.out.println(Arrays.toString(m.getParameterTypes()) + " bridge=" + m.isBridge());
        }
    }
}
```

```output for=bridge
[class java.lang.Object, class java.lang.Object] bridge=true
[class java.lang.String, class java.lang.String] bridge=false
```

**Reification in C#.** The compiled code of a C# generic type holds metadata for its type parameters. At run time, .NET makes one specialized type for each value type that is used as a type argument. All reference types share one specialized type, because all references have the same size.[^12] Each closed type, such as `List<int>` or `List<string>`, is a separate type at run time, and reflection can read its type arguments.[^13] So the code of a generic method can use `typeof(T)` and `default(T)`.[^15] With the constraint `where T : new()`, it can also call `new T()`.[^14] Kennedy and Syme designed this support for the .NET Common Language Runtime. Their design gives exact run-time types, specializes code in the just-in-time compiler, and shares code between instantiations that have the same representation.[^16]

**Monomorphization in C++ and Rust.** A C++ template is a pattern for code. The standard calls the step that makes code from it template instantiation. A function that the compiler makes from a function template is an instantiated function.[^20] Rust compiles generic code in the same way. The compiler finds each concrete type that generic code is called with, and it makes code for that type. The Rust Book calls this monomorphization.[^17] Each copy knows its types when it is compiled, so the run time needs no record of them. In the Rust example, `type_name::<T>()` gives a different name in each copy.

**Dictionaries in Go.** Go compiles generic code with GC shape stenciling and dictionaries. Two types have the same GC shape when they have the same underlying type, or when both are pointer types. The compiler makes one copy of a generic function for each GC shape. Each call passes a dictionary as a hidden first argument. The dictionary holds the run-time type descriptor of each type argument. The code uses these descriptors for conversions to interfaces, for type assertions and for type switches.[^24] So a Go type argument is reified. In the Go example, `float64` and a named type `Celsius` share one copy of the code, and reflection still tells them apart.

**Erasure of all types.** The TypeScript compiler checks the types and then removes them, so the annotations never change what the program does.[^25] A type assertion, such as `value as number`, is removed too. It has no check at run time, so a wrong assertion causes no exception and no `null`.[^26] Python keeps annotations as data, but the interpreter does not enforce them.[^28] A generic alias such as `list[int]` keeps its type argument in `__args__`. An object that is made from the alias is a plain `list`, so the type argument is erased when the object is created. `isinstance` does not accept a parameterized generic.[^27]

| Language | What the run time has | Can code read `T` at run time? |
|---|---|---|
| Java, Kotlin | one copy of the code with erased types[^1][^29] | no; pass a `Class<T>` token |
| TypeScript | no types at all[^25] | no; pass a function that checks a value |
| Python | annotations as data, not checked[^27] | the alias keeps `T`; the object does not |
| C# | type arguments in each closed type[^13] | yes, with `typeof(T)` |
| Go | one copy for each GC shape, and a dictionary[^24] | yes, through the dictionary |
| C++, Rust | one copy for each set of type arguments[^20][^17] | each copy knows `T` when it is compiled |

## In languages

The example files show what each language keeps at run time. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | annotations kept as data and not checked; `__args__` on a generic alias[^27] |
| JavaScript | n/a | no static types; each value carries its type at run time |
| TypeScript | native | all types erased; `as` has no check at run time[^26] |
| Java | native | type arguments erased; bridge methods; `Class<T>` tokens |
| C | absent | no type parameters; no type information at run time |
| C++ | native | each template instantiation is separate code; `std::function` and `std::any` hide a type |
| C# | native | type arguments reified; `typeof(T)`, `default(T)`, `new T()` |
| Go | native | GC shape stenciling with dictionaries; reflection sees `T` |
| Rust | native | monomorphization; `dyn Trait` hides the concrete type |
| Kotlin | native | erased on the JVM; `reified` type parameters in inline functions[^29] |
| PowerShell | stdlib | the generic types of .NET, which keep their type arguments[^34][^12] |
| Bash | n/a | no static types |
| SQL | n/a | no generic types |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup and style rules have no types |

Kotlin erases type arguments, as Java does. At run time, an instance of `Foo<Bar>` is only a `Foo<*>`, and the compiler rejects a check such as `ints is List<Int>`. An inline function can declare a reified type parameter. The compiler then inlines the actual type argument at each call site, so `value is T` works inside that function.[^29]

## Variations and related ideas

- **Type tokens.** A Java method that needs its type at run time can take a `Class<T>` argument. The Java Tutorial uses this to create an object of a type parameter with reflection.[^11] A super type token is an anonymous subclass of a generic class, such as `new TypeRef<Map<String, Integer>>() {}`. The class file records the type argument of the superclass in its Signature attribute, and `getGenericSuperclass` reads it back.[^5] The Java example shows both.
- **Type erasure as a C++ technique.** In C++ the term has a second meaning. `std::function` stores, copies and calls any callable object with a given call signature.[^21] `std::any` holds one value of any type.[^22] The concrete type is hidden behind one interface, but the value still knows its type. `any_cast` with the wrong type throws `std::bad_any_cast`.[^23] The C++ example shows both. [[higher-order-functions]] uses `std::function` to pass functions as values.
- **Trait objects.** A Rust `&dyn Display` hides its concrete type in the same way. It is a pointer to the value and a pointer to a vtable, which holds the methods for the concrete type. A call through it is a virtual call.[^18] [[interfaces]] compares this with the interfaces of other languages.
- **Run-time types in dynamic languages.** In JavaScript and Python, each value carries its type at run time. An operation checks the type when it runs. [[static-and-dynamic-typing]] describes this.
- **Casts.** A cast to an erased type checks only the erased part, or nothing at all in TypeScript. [[type-conversion]] covers the kinds of casts and what each one checks.

## Pitfalls

### Two overloads clash after erasure

Two methods with the same name must have different erasures.[^8] Both methods below have the erasure `total(List)`.

```java fails expect=compile id=name-clash
import java.util.List;

public class Main {
    static int total(List<Integer> numbers) {
        int sum = 0;
        for (int n : numbers) {
            sum += n;
        }
        return sum;
    }

    static int total(List<String> words) {
        int sum = 0;
        for (String w : words) {
            sum += w.length();
        }
        return sum;
    }

    public static void main(String[] args) {
        System.out.println(total(List.of(1, 2, 3)));
    }
}
```

`javac` reports `name clash: total(List<String>) and total(List<Integer>) have the same erasure`. The fix is two names, such as `sumOf` and `totalLength`.[^11]

### An unchecked cast fails far from its cause

A cast to a type with a type argument checks only the erased type. Here `read` casts an `Object` to `List<T>`, and the run time checks only that the object is a `List`.

```java fails expect=exception:ClassCastException id=unchecked
import java.util.List;
import java.util.Map;

public class Main {
    @SuppressWarnings("unchecked")
    static <T> List<T> read(Map<String, Object> config, String key) {
        return (List<T>) config.get(key);
    }

    static int longest(List<String> names) {
        int max = 0;
        for (String name : names) {
            max = Math.max(max, name.length());
        }
        return max;
    }

    public static void main(String[] args) {
        Map<String, Object> config = Map.of("names", List.of(1, 2, 3));
        List<String> names = read(config, "names");
        System.out.println("read " + names.size() + " names");
        System.out.println(longest(names));
    }
}
```

The program prints `read 3 names`. The `ClassCastException` comes later, from the loop in `longest`, where the compiler inserted a cast to `String`. The list of numbers is heap pollution: a `List<String>` variable refers to an object that is not a `List<String>`.[^4] The annotation `@SuppressWarnings("unchecked")` hid the only warning. The fix is a `Class<T>` token, so that `read` checks each element when it reads the data.

```java run id=checked
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Main {
    static <T> List<T> read(Map<String, Object> config, String key, Class<T> type) {
        List<T> result = new ArrayList<>();
        for (Object item : (List<?>) config.get(key)) {
            if (!type.isInstance(item)) {
                throw new IllegalArgumentException(key + " holds a " + item.getClass().getSimpleName());
            }
            result.add(type.cast(item));
        }
        return result;
    }

    public static void main(String[] args) {
        Map<String, Object> config = Map.of("names", List.of(1, 2, 3));
        try {
            List<String> names = read(config, "names", String.class);
            System.out.println(names);
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }
}
```

```output for=checked
names holds a Integer
```

### A generic array cannot be created

An array knows its element type at run time, but a type variable is not reifiable. So array creation with the element type `T` is a compile-time error.[^9]

```java fails expect=compile id=generic-array
import java.util.Arrays;

public class Main {
    static <T> T[] repeat(T value, int count) {
        T[] items = new T[count];
        Arrays.fill(items, value);
        return items;
    }

    public static void main(String[] args) {
        System.out.println(Arrays.toString(repeat("hi", 3)));
    }
}
```

`javac` reports `generic array creation`. The fix passes a `Class<T>` token, and `Array.newInstance` makes an array of that class. The cast to `T[]` is unchecked, but it is correct, because the array has the right class.

```java run id=array-token
import java.lang.reflect.Array;
import java.util.Arrays;

public class Main {
    static <T> T[] repeat(Class<T> type, T value, int count) {
        @SuppressWarnings("unchecked")
        T[] items = (T[]) Array.newInstance(type, count);
        Arrays.fill(items, value);
        return items;
    }

    public static void main(String[] args) {
        String[] words = repeat(String.class, "hi", 3);
        System.out.println(Arrays.toString(words));
        System.out.println(words.getClass().getSimpleName());
    }
}
```

```output for=array-token
[hi, hi, hi]
String[]
```

C# keeps type arguments at run time, so the code that Java rejects in this section works there.[^12] `new T[count]` creates an array, the two `Total` overloads have different signatures, and `is List<string>` tests the type argument.

```csharp run id=csharp-same
Console.WriteLine(string.Join(" ", Tools.Repeat("hi", 3)));
Console.WriteLine(Tools.Total(new List<int> { 1, 2, 3 }));
Console.WriteLine(Tools.Total(new List<string> { "ab", "cde" }));
object value = new List<int> { 1 };
Console.WriteLine(value is List<string>);

static class Tools
{
    public static T[] Repeat<T>(T value, int count)
    {
        var items = new T[count];
        Array.Fill(items, value);
        return items;
    }

    public static int Total(List<int> numbers) => numbers.Sum();

    public static int Total(List<string> words) => words.Sum(w => w.Length);
}
```

```output for=csharp-same
hi hi hi
6
5
False
```

### A TypeScript assertion does not check

A type assertion with `as` tells the compiler to trust the programmer. The compiler removes it, and nothing checks it at run time.[^26]

```typescript wrong id=ts-as
function readCount(raw: unknown): number {
  return raw as number;
}

const count = readCount("5");
console.log(count + 1);
```

```output for=ts-as
51
```

The type of `count` is `number`, but its value is the string `"5"`. So `+` joins two strings, and the program prints `51`. The fix is a check at run time with `typeof`, which also narrows the type for the compiler.

```typescript run id=ts-check
function readCount(raw: unknown): number {
  if (typeof raw === "number") {
    return raw;
  }
  if (typeof raw === "string" && /^\d+$/.test(raw)) {
    return Number(raw);
  }
  throw new TypeError(`not a count: ${String(raw)}`);
}

console.log(readCount("5") + 1);
console.log(readCount(5) + 1);
```

```output for=ts-check
6
6
```

## Cost

- **Erasure.** One copy of the code serves all type arguments. The price is paid in checks and in boxing. A Java type argument cannot be a primitive type, so a `List<Integer>` holds `Integer` objects.[^11] A call through a bridge method is one more call with a cast.[^6] The GJ page states that code translated by erasure is as efficient as Java code written for the same purpose.[^31]
- **Reification in .NET.** The runtime makes one copy of the code for each value type that is a type argument. All reference types share one copy.[^12]
- **Monomorphization.** Generic code runs as fast as code for the concrete types, because the compiler makes a copy for each type.[^17] The price is paid at build time and in the size of the program.
- **Trait objects.** A call through a `dyn Trait` finds its method at run time. The lookup has a cost, and the compiler cannot inline the method.[^19]
- **Dictionaries in Go.** One copy of the code serves each GC shape. Each call passes a dictionary as a hidden argument.[^24]

## History

- **1998.** Gilad Bracha, Martin Odersky, David Stoutamire and Philip Wadler described GJ, an extension of Java with generic types. GJ is translated by erasure, so no information about type parameters is kept at run time.[^30][^31]
- **1999.** In May 1999, Sun proposed to add generics to Java, based on GJ.[^31]
- **2001.** Andrew Kennedy and Don Syme described generics for the .NET Common Language Runtime, with exact run-time types.[^16]
- **2004.** JSR 14, "Add Generic Types To The Java Programming Language", reached its final release on 30 September 2004.[^32]
- **2022.** Go 1.18, released on 15 March 2022, added generics. Its implementation uses GC shape stenciling and dictionaries.[^33][^24]

## References
