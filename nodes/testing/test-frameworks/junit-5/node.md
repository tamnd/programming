---
tag: KJPDRQ
kind: tool
slug: junit-5
name: JUnit 5
aliases: [JUnit Jupiter, JUnit Platform]
area: testing/test-frameworks
level: core
summary: >-
  JUnit 5 is the JUnit generation for Java that splits the tool into the JUnit Platform, which launches tests, the Jupiter engine for new tests and the Vintage engine for JUnit 3 and 4 tests.
role: [test-framework]
licence: epl-2.0
repo: https://github.com/junit-team/junit-framework
homepage: https://junit.org
first_release: 2017-09-10
history:
  year: 2017
  note: JUnit 5.0.0 was released in September 2017. It replaced the runner and rule model of JUnit 4 with a launcher platform, test engines and one extension API. JUnit 6.0.0, in September 2025, kept this structure and gave all parts one version number.
see_also: [junit, testng, test-framework, unit-testing, test-double, property-based-tests, java]
sources:
  - url: https://docs.junit.org/5.14.4/overview.html
    title: JUnit 5 User Guide 5.14.4, Overview
    locator: What is JUnit 5?; Supported Java Versions
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/overview.html
    title: JUnit User Guide 6.1.3, Overview
    locator: What is JUnit?; Supported Java Versions
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/release-notes.html
    title: JUnit User Guide 6.1.3, Release Notes
    locator: 6.1.0; 6.0.0
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/running-tests/build-support.html
    title: JUnit User Guide 6.1.3, Build Support
    locator: Gradle; Maven
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/annotations.html
    title: JUnit User Guide 6.1.3, Annotations
    locator: the table of annotations
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/assertions.html
    title: JUnit User Guide 6.1.3, Assertions
    locator: introduction; the example of grouped assertions
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/display-names.html
    title: JUnit User Guide 6.1.3, Display Names
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/test-classes-and-methods.html
    title: JUnit User Guide 6.1.3, Test Classes and Methods
    locator: Class and method visibility
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/advanced-topics/engines.html
    title: JUnit User Guide 6.1.3, Test Engines
    locator: JUnit Test Engines; Custom Test Engines
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/definitions.html
    title: JUnit User Guide 6.1.3, Definitions
    locator: Platform Concepts; Jupiter Concepts
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/migrating-from-junit4.html
    title: JUnit User Guide 6.1.3, Migrating from JUnit 4
    locator: introduction; Running JUnit 4 Tests on the JUnit Platform; Migration Tips
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/test-instance-lifecycle.html
    title: JUnit User Guide 6.1.3, Test Instance Lifecycle
    locator: introduction; Changing the Default Test Instance Lifecycle
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/test-execution-order.html
    title: JUnit User Guide 6.1.3, Test Execution Order
    locator: introduction; Method Order
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/nested-tests.html
    title: JUnit User Guide 6.1.3, Nested Tests
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/extensions/overview.html
    title: JUnit User Guide 6.1.3, Extension Model, Overview
    locator: the whole page
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/extensions/registering-extensions.html
    title: JUnit User Guide 6.1.3, Registering Extensions
    locator: introduction; Declarative Extension Registration
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/tagging-and-filtering.html
    title: JUnit User Guide 6.1.3, Tagging and Filtering
    locator: the whole page
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/running-tests/console-launcher.html
    title: JUnit User Guide 6.1.3, Console Launcher
    locator: Exit Code; Options
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/parameterized-classes-and-tests.html
    title: JUnit User Guide 6.1.3, Parameterized Classes and Tests
    locator: introduction; Sources of Arguments
    accessed: 2026-11-15
  - url: https://docs.junit.org/5.14.4/release-notes.html
    title: JUnit 5 User Guide 5.14.4, Release Notes
    locator: 5.14.0; 5.13.0
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/dynamic-tests.html
    title: JUnit User Guide 6.1.3, Dynamic Tests
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/running-tests/configuration-parameters.html
    title: JUnit User Guide 6.1.3, Configuration Parameters
    locator: introduction
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/parallel-execution.html
    title: JUnit User Guide 6.1.3, Parallel Execution
    locator: introduction
    accessed: 2026-11-15
  - url: https://junit.org/junit4/
    title: JUnit 4, About
    locator: the whole page
    accessed: 2026-11-15
  - url: https://github.com/junit-team/testng-engine
    title: junit-team/testng-engine, TestNG Engine for the JUnit Platform, README
    locator: Usage
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit-framework/releases
    title: junit-team/junit-framework, Releases on GitHub
    locator: the releases r5.0.0 to r6.1.3 and their dates
    accessed: 2026-11-15
---

## What it is

JUnit 5 is the fifth generation of [[junit]], the main [[test-framework]] of [[java]]. It is not one library. It has three sub-projects: "JUnit 5 = JUnit Platform + JUnit Jupiter + JUnit Vintage".[^1]

- The JUnit Platform "serves as a foundation for launching testing frameworks on the JVM".[^2] It defines the `TestEngine` API. IDEs, Maven, Gradle and the Console Launcher talk to the platform, not to one framework.[^2]
- "JUnit Jupiter is the combination of the programming model and extension model for writing JUnit tests and extensions."[^2] Its engine runs the tests that this page shows.
- JUnit Vintage is an engine that runs JUnit 3 and JUnit 4 tests on the platform. The guide says that "the JUnit Vintage engine is deprecated and should only be used temporarily while migrating tests".[^2]

Since JUnit 6.0.0 the project uses a "Single version number for Platform, Jupiter, and Vintage".[^3] The model of this page is the same in JUnit 6. The name JUnit 5 stays in use for this model, and the examples here run with the JUnit Console Launcher 1.13.4, which contains Jupiter 5.13.4. JUnit 5 needs Java 8 or later at runtime, and JUnit 6 needs Java 17 or later.[^1][^2]

## Install and first use

A build tool gets JUnit from Maven Central. In Gradle, add the Jupiter dependency and the launcher, and call `useJUnitPlatform()` in the test task.[^4]

```text
// build.gradle.kts
dependencies {
    testImplementation("org.junit.jupiter:junit-jupiter:6.1.3")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

tasks.test {
    useJUnitPlatform()
}
```

Maven Surefire and Failsafe run the platform when an engine is on the test class path. As of JUnit 6.0, they must be at version 3.0.0 or later.[^4] The guide recommends the bill of materials `org.junit:junit-bom` to keep the versions of the JUnit artifacts the same.[^4]

A test class is a class with test methods. `@Test` marks a test method, and `@BeforeEach` marks a method that runs before each test.[^5] The static methods of `org.junit.jupiter.api.Assertions` do the checks.[^6] `@DisplayName` gives a test a name for the reports, and the name can contain spaces.[^7]

```java test id=first-use file=InventoryTest.java
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.HashMap;
import java.util.Map;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

final class Inventory {
    private final Map<String, Integer> stock = new HashMap<>();

    void add(String item, int count) {
        stock.merge(item, count, Integer::sum);
    }

    void remove(String item, int count) {
        int have = count(item);
        if (count > have) {
            throw new IllegalStateException(item + ": have " + have + ", asked " + count);
        }
        stock.put(item, have - count);
    }

    int count(String item) {
        return stock.getOrDefault(item, 0);
    }
}

class InventoryTest {
    private Inventory inventory;

    @BeforeEach
    void fillShelf() {
        inventory = new Inventory();
        inventory.add("pen", 10);
    }

    @Test
    void addIncreasesCount() {
        inventory.add("pen", 5);
        assertEquals(15, inventory.count("pen"));
    }

    @Test
    @DisplayName("removing more than the stock fails")
    void removeTooMany() {
        IllegalStateException e =
                assertThrows(IllegalStateException.class, () -> inventory.remove("pen", 11));
        assertEquals("pen: have 10, asked 11", e.getMessage());
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ InventoryTest ✔
│     ├─ addIncreasesCount() ✔
│     └─ removing more than the stock fails ✔
└─ JUnit Vintage ✔

Test run finished after $TIME
[         4 containers found      ]
[         0 containers skipped    ]
[         4 containers started    ]
[         0 containers aborted    ]
[         4 containers successful ]
[         0 containers failed     ]
[         2 tests found           ]
[         0 tests skipped         ]
[         2 tests started         ]
[         0 tests aborted         ]
[         2 tests successful      ]
[         0 tests failed          ]
```

The report is a tree. The Jupiter engine found the class and its two tests. The JUnit Platform Suite engine and the Vintage engine also ran, and they found no tests. Test classes and test methods do not have to be `public`, but they cannot be `private`.[^8]

## Concepts of the tool

**Platform, engine and launcher.** A `TestEngine` finds and runs the tests of one programming model. JUnit gives three engines: `junit-jupiter-engine`, `junit-vintage-engine` and `junit-platform-suite-engine`.[^9] Other projects can write engines of their own.[^9] The launcher of the platform runs all the engines on the class path, so one run can mix the tests of more than one framework.

**Containers and tests.** The platform sees a tree. A container is "a node in the test tree that contains other containers or tests as its children".[^10] A test class is a container, and a `@Test` method is a test. A `@ParameterizedTest` method is a container too, and each invocation is a test under it.[^10]

**Lifecycle methods.** `@BeforeEach` and `@AfterEach` run around each test method. `@BeforeAll` and `@AfterAll` run once for a class, and they must be static in the default lifecycle.[^5] These names replace `@Before`, `@After`, `@BeforeClass` and `@AfterClass` of JUnit 4.[^11]

**The test instance.** By default, "JUnit creates a new instance of each test class before executing each test method".[^12] Fields are new in each test, so one test cannot change the data of the next one. With `@TestInstance(Lifecycle.PER_CLASS)`, "a new test instance will be created once per test class".[^12] In the next example, the outer class uses the default. The `@Nested` inner class uses one instance, so its list keeps the entries of the earlier test. `@TestMethodOrder` with `MethodOrderer.OrderAnnotation` runs the nested tests in the order of their `@Order` values.[^13]

```java test id=lifecycle file=LifecycleTest.java
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import org.junit.jupiter.api.MethodOrderer;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestMethodOrder;

class LifecycleTest {
    // A new LifecycleTest object for each test: the list is empty at the start of each test.
    private final List<String> log = new ArrayList<>();

    @Test
    void first() {
        log.add("first");
        assertEquals(List.of("first"), log);
    }

    @Test
    void second() {
        log.add("second");
        assertEquals(List.of("second"), log);
    }

    @Nested
    @TestInstance(TestInstance.Lifecycle.PER_CLASS)
    @TestMethodOrder(MethodOrderer.OrderAnnotation.class)
    class SharedInstance {
        // One SharedInstance object for all its tests: the list keeps its entries.
        private final List<String> shared = new ArrayList<>();

        @Test
        @Order(1)
        void stepOne() {
            shared.add("one");
            assertEquals(List.of("one"), shared);
        }

        @Test
        @Order(2)
        void stepTwo() {
            shared.add("two");
            assertEquals(List.of("one", "two"), shared);
        }
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ LifecycleTest ✔
│     ├─ second() ✔
│     ├─ first() ✔
│     └─ SharedInstance ✔
│        ├─ stepOne() ✔
│        └─ stepTwo() ✔
└─ JUnit Vintage ✔

Test run finished after $TIME
[         5 containers found      ]
[         0 containers skipped    ]
[         5 containers started    ]
[         0 containers aborted    ]
[         5 containers successful ]
[         0 containers failed     ]
[         4 tests found           ]
[         0 tests skipped         ]
[         4 tests started         ]
[         0 tests aborted         ]
[         4 tests successful      ]
[         0 tests failed          ]
```

**Order.** Without `@TestMethodOrder`, the order of the methods is "deterministic but intentionally nonobvious".[^13] It is the same in each run, but it is not the order of the source file. Do not write tests that need another test to run first. The outer tests above pass in any order.

**Nested classes.** A `@Nested` class is an inner class with tests of its own. Nested classes give a tree of groups, for example one group for each state of the object under test.[^14]

**Extensions.** JUnit 4 had three ways to add behaviour: runners, `TestRule` and `MethodRule`. In Jupiter, "the JUnit Jupiter extension model consists of a single, coherent concept: the Extension API."[^15] An extension implements one or more callback interfaces, such as `BeforeEachCallback` or `ParameterResolver`. You register it with `@ExtendWith` on a class, a method or a field, with `@RegisterExtension` on a field, or through the Java `ServiceLoader`.[^16]

**Tags.** `@Tag("slow")` puts a label on a class or a method. A build tool or the Console Launcher can then include or exclude tests by tag.[^17][^18]

## Common tasks

**Run one test with many inputs.** "Parameterized tests make it possible to run a test method multiple times with different arguments."[^19] A source annotation gives the arguments: `@ValueSource` for one value per run, `@CsvSource` for rows, and `@MethodSource` for a static method that returns a stream.[^19] `assertAll` runs all the checks that it gets and reports all the failures together, so one failed check does not hide the others.[^6]

```java test id=parameterized file=ShippingTest.java
import static org.junit.jupiter.api.Assertions.assertAll;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.CsvSource;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

final class Shipping {
    // The price in cents for a parcel of the given weight in grams.
    static int price(int grams) {
        if (grams <= 0) {
            throw new IllegalArgumentException("weight must be positive");
        }
        if (grams <= 500) {
            return 400;
        }
        if (grams <= 2000) {
            return 750;
        }
        return 750 + (grams - 2000 + 999) / 1000 * 200;
    }
}

class ShippingTest {
    @ParameterizedTest
    @CsvSource({"1, 400", "500, 400", "501, 750", "2000, 750", "2001, 950"})
    void priceAtTheEdges(int grams, int cents) {
        assertEquals(cents, Shipping.price(grams));
    }

    @ParameterizedTest
    @ValueSource(ints = {1, 1000, 5000})
    void priceIsPositive(int grams) {
        assertTrue(Shipping.price(grams) > 0);
    }

    static Stream<Arguments> heavyParcels() {
        return Stream.of(Arguments.of(3000, 950), Arguments.of(4500, 1350));
    }

    @ParameterizedTest
    @MethodSource("heavyParcels")
    void heavyParcels(int grams, int cents) {
        assertEquals(cents, Shipping.price(grams));
    }

    @Test
    void zeroAndNegativeAreRejected() {
        assertAll(
                () -> assertThrows(IllegalArgumentException.class, () -> Shipping.price(0)),
                () -> assertThrows(IllegalArgumentException.class, () -> Shipping.price(-5)));
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ ShippingTest ✔
│     ├─ priceIsPositive(int) ✔
│     │  ├─ [1] 1 ✔
│     │  ├─ [2] 1000 ✔
│     │  └─ [3] 5000 ✔
│     ├─ zeroAndNegativeAreRejected() ✔
│     ├─ priceAtTheEdges(int, int) ✔
│     │  ├─ [1] 1, 400 ✔
│     │  ├─ [2] 500, 400 ✔
│     │  ├─ [3] 501, 750 ✔
│     │  ├─ [4] 2000, 750 ✔
│     │  └─ [5] 2001, 950 ✔
│     └─ heavyParcels(int, int) ✔
│        ├─ [1] 3000, 950 ✔
│        └─ [2] 4500, 1350 ✔
└─ JUnit Vintage ✔

Test run finished after $TIME
[         7 containers found      ]
[         0 containers skipped    ]
[         7 containers started    ]
[         0 containers aborted    ]
[         7 containers successful ]
[         0 containers failed     ]
[        11 tests found           ]
[         0 tests skipped         ]
[        11 tests started         ]
[         0 tests aborted         ]
[        11 tests successful      ]
[         0 tests failed          ]
```

The rows of `@CsvSource` are at the edges of each price band, as boundary value analysis suggests. JUnit 5.13 added `@ParameterizedClass`, which runs all the tests of a class once for each set of arguments.[^20] The guide says that "Parameterized classes are currently an experimental feature."[^19]

**Make tests at run time.** A `@TestFactory` method returns a stream of `DynamicTest` objects. The factory is not a test: "a dynamic test is the product of a factory."[^21] Use it when the cases come from data, such as the rows of a file.

```java test id=dynamic file=RomanTest.java
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.DynamicTest.dynamicTest;

import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Stream;
import org.junit.jupiter.api.DynamicTest;
import org.junit.jupiter.api.TestFactory;

class RomanTest {
    private static final int[] VALUES = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    private static final String[] SYMBOLS = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };

    static String roman(int n) {
        StringBuilder out = new StringBuilder();
        for (int i = 0; i < VALUES.length; i++) {
            while (n >= VALUES[i]) {
                out.append(SYMBOLS[i]);
                n -= VALUES[i];
            }
        }
        return out.toString();
    }

    @TestFactory
    Stream<DynamicTest> knownNumbers() {
        Map<Integer, String> cases = new TreeMap<>(Map.of(4, "IV", 9, "IX", 14, "XIV", 1994, "MCMXCIV"));
        return cases.entrySet().stream()
                .map(c -> dynamicTest(c.getKey() + " is " + c.getValue(),
                        () -> assertEquals(c.getValue(), roman(c.getKey()))));
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ RomanTest ✔
│     └─ knownNumbers() ✔
│        ├─ 4 is IV ✔
│        ├─ 9 is IX ✔
│        ├─ 14 is XIV ✔
│        └─ 1994 is MCMXCIV ✔
└─ JUnit Vintage ✔

Test run finished after $TIME
[         5 containers found      ]
[         0 containers skipped    ]
[         5 containers started    ]
[         0 containers aborted    ]
[         5 containers successful ]
[         0 containers failed     ]
[         4 tests found           ]
[         0 tests skipped         ]
[         4 tests started         ]
[         0 tests aborted         ]
[         4 tests successful      ]
[         0 tests failed          ]
```

**Give a test a resource with an extension.** A `ParameterResolver` supplies the arguments of test methods. This extension gives each parameter of type `Clock` a clock that stands still, so the test does not depend on the date of the run.

```java test id=extension file=InvoiceTest.java
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.time.Clock;
import java.time.Instant;
import java.time.LocalDate;
import java.time.ZoneOffset;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.junit.jupiter.api.extension.ExtensionContext;
import org.junit.jupiter.api.extension.ParameterContext;
import org.junit.jupiter.api.extension.ParameterResolver;

class FixedClockExtension implements ParameterResolver {
    @Override
    public boolean supportsParameter(ParameterContext parameter, ExtensionContext context) {
        return parameter.getParameter().getType() == Clock.class;
    }

    @Override
    public Object resolveParameter(ParameterContext parameter, ExtensionContext context) {
        return Clock.fixed(Instant.parse("2026-03-01T09:00:00Z"), ZoneOffset.UTC);
    }
}

final class Invoice {
    private final Clock clock;

    Invoice(Clock clock) {
        this.clock = clock;
    }

    LocalDate dueDate() {
        return LocalDate.now(clock).plusDays(30);
    }
}

@ExtendWith(FixedClockExtension.class)
class InvoiceTest {
    @Test
    void dueInThirtyDays(Clock clock) {
        assertEquals(LocalDate.of(2026, 3, 31), new Invoice(clock).dueDate());
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ InvoiceTest ✔
│     └─ dueInThirtyDays(Clock) ✔
└─ JUnit Vintage ✔

Test run finished after $TIME
[         4 containers found      ]
[         0 containers skipped    ]
[         4 containers started    ]
[         0 containers aborted    ]
[         4 containers successful ]
[         0 containers failed     ]
[         1 tests found           ]
[         0 tests skipped         ]
[         1 tests started         ]
[         0 tests aborted         ]
[         1 tests successful      ]
[         0 tests failed          ]
```

A fixed clock is a [[test-double]] of the system clock. Without the extension, JUnit has no resolver for `Clock`, and the test fails before it starts.

**Run the tests from the command line.** The Console Launcher runs the platform without a build tool. The example runs on this page use a command like this one:

```text
java -cp classes:junit-platform-console-standalone.jar \
  org.junit.platform.console.ConsoleLauncher execute \
  --scan-class-path --details=tree --include-tag=fast
```

`--select-class` and `--select-method` choose single tests, and `--include-tag` and `--exclude-tag` filter by tag.[^18] `--fail-fast` stops after the first failed test.[^18] The exit code is 0 when all tests pass, and "status code 1 is returned if any containers or tests failed."[^18]

**Keep old JUnit 4 tests while you migrate.** Jupiter uses the package `org.junit.jupiter`, so "having both JUnit 4 and JUnit Jupiter in the classpath does not lead to any conflicts."[^11] With the Vintage engine and the JUnit 4 jar on the class path, the platform runs both kinds of tests. The trap is that the annotations of the two versions have the same names in different packages. This class takes `@Test` from JUnit 4 and `@BeforeEach` from Jupiter. Vintage runs the test, but it does not know `@BeforeEach`, so the field is still `null`:

```java test fails expect=test-failure id=mixed file=MixedTest.java deps=deps/java/pom.xml
import static org.junit.Assert.assertEquals;

import java.util.ArrayList;
import java.util.List;
import org.junit.Test;
import org.junit.jupiter.api.BeforeEach;

public class MixedTest {
    private List<String> names;

    @BeforeEach
    void setUp() {
        names = new ArrayList<>();
    }

    @Test
    public void addOneName() {
        names.add("Ada");
        assertEquals(1, names.size());
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
└─ JUnit Vintage ✔
   └─ MixedTest ✔
      └─ addOneName ✘ Cannot invoke "java.util.List.add(Object)" because "this.names" is null

Failures (1):
  JUnit Vintage:MixedTest:addOneName
    MethodSource [className = 'MixedTest', methodName = 'addOneName', methodParameterTypes = '']
    => java.lang.NullPointerException: Cannot invoke "java.util.List.add(Object)" because "this.names" is null
       MixedTest.addOneName(MixedTest.java:18)

Test run finished after $TIME
[         4 containers found      ]
[         0 containers skipped    ]
[         4 containers started    ]
[         0 containers aborted    ]
[         4 containers successful ]
[         0 containers failed     ]
[         1 tests found           ]
[         0 tests skipped         ]
[         1 tests started         ]
[         0 tests aborted         ]
[         0 tests successful      ]
[         1 tests failed          ]
```

The test fails with a `NullPointerException`, and the report puts it under the Vintage engine, not under Jupiter. To fix the class, take all the annotations from `org.junit.jupiter.api`. The migration guide also lists the JUnit 4 features with no direct copy: `@RunWith` gives way to `@ExtendWith`, `@Rule` gives way to `@ExtendWith` and `@RegisterExtension`, and `@Test(expected = ...)` gives way to `assertThrows`.[^11]

## Configuration

Configuration parameters are key and value pairs for the engines. The platform reads them from the options of the launcher or of the build tool, from JVM system properties, and from "a file named junit-platform.properties in the root of the class path".[^22]

```text
# src/test/resources/junit-platform.properties
junit.jupiter.testinstance.lifecycle.default = per_class
junit.jupiter.execution.parallel.enabled = true
junit.jupiter.execution.parallel.mode.default = concurrent
```

- `junit.jupiter.testinstance.lifecycle.default` changes the default instance lifecycle for all test classes.[^12]
- "By default, JUnit Jupiter tests are run sequentially in a single thread".[^23] `junit.jupiter.execution.parallel.enabled = true` turns on parallel runs. Even then, tests still run in the same thread until the mode is `concurrent`, either for all tests with `junit.jupiter.execution.parallel.mode.default` or for one class with `@Execution`.[^23]
- In Gradle, `systemProperty` sets a configuration parameter for the test task. In Maven Surefire, the property `configurationParameters` does the same.[^22]

## Alternatives

**JUnit 4** is the earlier generation, with runners and rules. Its site says: "JUnit 4 is in maintenance mode."[^24] See [[junit]] for its model and the history of all versions.

**TestNG** is a Java test framework with groups, dependencies between test methods and data providers. The JUnit team keeps a TestNG engine for the JUnit Platform, so TestNG tests can run in the same launcher.[^25] See [[testng]].

**Engines on the platform.** Other test libraries, for example libraries for [[property-based-tests]], can plug into the same launcher as an engine or as a Jupiter extension.[^9][^15]

## History and versions

The guide of JUnit 5 said: "JUnit 5 requires Java 8 (or higher) at runtime."[^1] The 5.x line ran from 5.0.0 in September 2017 to 5.14 in 2025.[^26] JUnit 6.0.0 brought a "Java 17 and Kotlin 2.1 baseline" and one version number for all three parts.[^3] 5.14.0 came out on the same day, with "Deprecations along with new APIs to ease migration to JUnit 6".[^20]

| Version | Date | Notes |
|---|---|---|
| 5.0.0 | 2017-09-10 | The first general release of the Platform, Jupiter and Vintage[^26] |
| 5.13.0 | 2025-05-30 | `@ParameterizedClass` and `@ClassTemplate`; options `--redirect-stdout` and `--redirect-stderr` for the Console Launcher[^20][^26] |
| 5.14.0 | 2025-09-30 | Deprecations and new APIs to help the move to JUnit 6[^20][^26] |
| 6.0.0 | 2025-09-30 | Java 17 baseline; one version number for Platform, Jupiter and Vintage; `--fail-fast` for the Console Launcher[^3][^26] |
| 6.1.0 | 2026-05-19 | `@DefaultLocale` and `@DefaultTimeZone`; a new executor for parallel tests[^3][^26] |
| 6.1.3 | 2026-08-07 | The current release at the date of this page[^3][^26] |

## References
