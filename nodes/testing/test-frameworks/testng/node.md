---
tag: K8C3TK
kind: tool
slug: testng
name: TestNG
aliases: []
area: testing/test-frameworks
level: advanced
summary: >-
  TestNG is a Java test framework with data providers, groups, test dependencies and suites in XML. This page shows its test model and how it differs from JUnit.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/testng-team/testng
homepage: https://testng.org
see_also: [junit-5, junit, test-framework, unit-testing, java]
same_as:
  wikidata: Q4385601
  wikipedia: TestNG
sources:
  - url: https://testng.org/
    title: TestNG documentation, version 7.9.0
    locator: 1. Introduction; 1.4. Requirements; 1.5. License; 2. Download; 3.1. Introduction; 3.2. Annotations; 3.3. testng.xml; 3.4. Running TestNG; 3.6.2. Data Providers; 3.7.1. Dependencies with annotations; 3.11.2. Parallel tests, classes and methods; 9.2. Asserts; 9.3. Running JUnit Tests; 13. The TestNG TextBook; 15. GitHub Repository
    accessed: 2026-11-15
  - url: https://github.com/junit-team/testng-engine
    title: junit-team/testng-engine, TestNG Engine for the JUnit Platform, README
    locator: the introduction and Usage
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/writing-tests/test-instance-lifecycle.html
    title: JUnit User Guide 6.1.3, Test Instance Lifecycle
    locator: introduction
    accessed: 2026-11-15
  - url: https://repo1.maven.org/maven2/org/testng/testng/
    title: Maven Central, the directory org/testng/testng
    locator: the dates of the version directories
    accessed: 2026-11-15
---

## What it is

TestNG is a [[test-framework]] for [[java]]. Its documentation describes it so: "TestNG is a testing framework inspired from JUnit and NUnit but introducing some new functionalities that make it more powerful and easier to use".[^1] It also says that "TestNG is designed to cover all categories of tests: unit, functional, end-to-end, integration, etc".[^1] Cédric Beust is "the creator and main contributor to the TestNG project".[^1] The introduction gives the reason for it: "I started TestNG out of frustration for some JUnit deficiencies".[^1]

The features that make TestNG different from [[junit]] 4 are:

- data providers, which give the rows of a parameterized test;
- groups, which put a tag on test methods so that a run can select them;
- dependencies between test methods, so that a test runs only after other tests pass;
- suites in an XML file, `testng.xml`, which select the classes, groups and methods, and set the parallel mode.[^1]

TestNG is open source under the Apache License 2.0.[^1]

## Install and first use

TestNG is the jar `org.testng:testng`. The documentation gives this Maven dependency:[^1]

```text
<dependency>
  <groupId>org.testng</groupId>
  <artifactId>testng</artifactId>
  <version>7.9.0</version>
  <scope>test</scope>
</dependency>
```

The requirements are "TestNG Upto v7.5: JDK 8." and "TestNG v7.6.0 and above: JDK 11 or higher."[^1] A test class does not extend a base class. The documentation says: "No need to extend a class or implement an interface."[^1] The annotations are in `org.testng.annotations`, and the assertions are in `org.testng.Assert`.

The examples on this page run TestNG 7.12.0 on the JUnit Platform, through the TestNG engine. That engine "Allows executing TestNG tests on the JUnit Platform".[^2] The report is the tree of the JUnit console launcher, and the TestNG engine is one node of it.

```java test id=first-use file=TheaterTest.java deps=deps/java/pom.xml
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.assertThrows;

import org.testng.annotations.BeforeMethod;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

public class TheaterTest {
    static class Theater {
        private int free;

        Theater(int seats) {
            free = seats;
        }

        void book(int people) {
            if (people > free) {
                throw new IllegalStateException("only " + free + " seats left");
            }
            free -= people;
        }

        int free() {
            return free;
        }
    }

    private Theater theater;

    @BeforeMethod
    public void openTheater() {
        theater = new Theater(2);
    }

    @Test(groups = "fast")
    public void startsWithAllSeatsFree() {
        assertEquals(theater.free(), 2);
    }

    @DataProvider(name = "parties")
    public Object[][] parties() {
        return new Object[][] {{1, 1}, {2, 0}};
    }

    @Test(dataProvider = "parties")
    public void bookingTakesSeats(int people, int left) {
        theater.book(people);
        assertEquals(theater.free(), left);
    }

    @Test
    public void overbookingFails() {
        assertThrows(IllegalStateException.class, () -> theater.book(3));
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
├─ JUnit Vintage ✔
└─ TestNG ✔
   └─ TheaterTest ✔
      ├─ bookingTakesSeats(int, int) ✔
      │  ├─ [0] 1, 1 ✔
      │  └─ [1] 2, 0 ✔
      ├─ overbookingFails ✔
      └─ startsWithAllSeatsFree ✔

Test run finished after $TIME
[         6 containers found      ]
[         0 containers skipped    ]
[         6 containers started    ]
[         0 containers aborted    ]
[         6 containers successful ]
[         0 containers failed     ]
[         4 tests found           ]
[         0 tests skipped         ]
[         4 tests started         ]
[         0 tests aborted         ]
[         4 tests successful      ]
[         0 tests failed          ]
```

The method with `@BeforeMethod` runs before each test method, so each test starts with two free seats. The data provider gives two rows, so `bookingTakesSeats` runs two times, and the report shows one node for each row.

## Concepts of the tool

### Suites, tests, classes and methods

TestNG has four levels. "A suite is represented by one XML file. It can contain one or more tests and is defined by the <suite> tag."[^1] "A test is represented by <test> and can contain one or more TestNG classes."[^1] A TestNG class "is a Java class that contains at least one TestNG annotation".[^1] "A test method is a Java method annotated by @Test in your source."[^1]

In TestNG, the word test has two meanings: a `<test>` element of the XML file is a group of classes, and a test method is one test. JUnit has no level that is the same as a `<test>` element.

### Configuration methods

Each level has a pair of configuration annotations:[^1]

| Annotation | When the method runs |
|---|---|
| `@BeforeSuite`, `@AfterSuite` | before and after all tests in the suite |
| `@BeforeTest`, `@AfterTest` | before and after the test methods of the classes in a `<test>` element |
| `@BeforeGroups`, `@AfterGroups` | before the first and after the last test method of the given groups |
| `@BeforeClass`, `@AfterClass` | before the first and after the last test method of the class |
| `@BeforeMethod`, `@AfterMethod` | before and after each test method |

These annotations "will also be honored (inherited) when placed on a superclass of a TestNG class".[^1] Unlike `@BeforeAll` in JUnit Jupiter, `@BeforeClass` in TestNG is put on an instance method, as in the examples of the documentation.[^1]

### One instance for the methods of a class

JUnit "creates a new instance of each test class before executing each test method".[^3] TestNG does not do this. In the next example, the two test methods use the same list, so the second test sees the value that the first test added:

```java test id=shared-instance file=SharedStateTest.java deps=deps/java/pom.xml
import static org.testng.Assert.assertEquals;

import java.util.ArrayList;
import java.util.List;
import org.testng.annotations.Test;

public class SharedStateTest {
    private final List<String> calls = new ArrayList<>();

    @Test
    public void first() {
        calls.add("first");
        assertEquals(calls, List.of("first"));
    }

    @Test(dependsOnMethods = "first")
    public void second() {
        calls.add("second");
        assertEquals(calls, List.of("first", "second"));
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
├─ JUnit Vintage ✔
└─ TestNG ✔
   └─ SharedStateTest ✔
      ├─ first ✔
      └─ second ✔

Test run finished after $TIME
[         5 containers found      ]
[         0 containers skipped    ]
[         5 containers started    ]
[         0 containers aborted    ]
[         5 containers successful ]
[         0 containers failed     ]
[         2 tests found           ]
[         0 tests skipped         ]
[         2 tests started         ]
[         0 tests aborted         ]
[         2 tests successful      ]
[         0 tests failed          ]
```

The field is shared, so the result of a test can depend on the tests that ran before it. Reset the state in a `@BeforeMethod` method when each test must start clean, as `TheaterTest` does. This model also has a risk for parallel runs: two test methods of one instance that run in different threads use the same fields.

### Data providers

"A Data Provider is a method on your class that returns an array of array of objects."[^1] The size of the first dimension is the number of times that the test method runs, and each inner array holds the arguments of one call.[^1] A data provider can also return an `Iterator<Object[]>`, which "lets you create your test data lazily".[^1] The test method gives the name of the provider in `@Test(dataProvider = "...")`. With `dataProviderClass`, the provider can be in a different class.[^1]

### Groups

A test method "can belong to one or several groups".[^1] A suite file can include and exclude groups, so a run can select the fast tests, or leave out the broken tests. A group name can have a prefix, such as `windows.checkintest`, and the suite file can select groups with a regular expression.[^1]

### Dependencies between tests

A test method can name other methods in `dependsOnMethods`, or groups in `dependsOnGroups`. The documentation gives two kinds of dependencies:[^1]

- Hard dependencies: "All the methods you depend on must have run and succeeded for you to run. If at least one failure occurred in your dependencies, you will not be invoked and marked as a SKIP in the report."
- Soft dependencies: "You will always be run after the methods you depend on, even if some of them have failed." You get a soft dependency with `alwaysRun=true` in the `@Test` annotation.

The documentation adds that the methods that depend on a failed method "are not marked as FAIL but as SKIP".[^1] In this example, the first test fails. The second test has a hard dependency on it, so TestNG skips it. The third test has a soft dependency, so it runs:

```java test fails expect=test-failure id=dependencies file=ServerTest.java deps=deps/java/pom.xml
import static org.testng.Assert.assertEquals;

import java.util.ArrayList;
import java.util.List;
import org.testng.annotations.Test;

public class ServerTest {
    private final List<String> steps = new ArrayList<>();

    private String status() {
        return "down";
    }

    @Test
    public void serverStarts() {
        steps.add("start");
        assertEquals(status(), "up");
    }

    @Test(dependsOnMethods = "serverStarts")
    public void serverAnswers() {
        steps.add("answer");
    }

    @Test(dependsOnMethods = "serverStarts", alwaysRun = true)
    public void startWasTried() {
        assertEquals(steps, List.of("start"));
    }

    // A fixed instance name keeps the output the same on each run.
    @Override
    public String toString() {
        return "ServerTest";
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
├─ JUnit Vintage ✔
└─ TestNG ✔
   └─ ServerTest ✔
      ├─ serverStarts on ServerTest ✘ expected [up] but found [down]
      ├─ serverAnswers on ServerTest ■ Method ServerTest.serverAnswers() on instance ServerTest depends on not successfully finished methods
      │        [ServerTest.serverStarts() on instance ServerTest]
      └─ startWasTried on ServerTest ✔

Failures (1):
  TestNG:ServerTest:serverStarts on ServerTest
    MethodSource [className = 'ServerTest', methodName = 'serverStarts', methodParameterTypes = '']
    => java.lang.AssertionError: expected [up] but found [down]
       org.testng.Assert.fail(Assert.java:111)
       org.testng.Assert.failNotEquals(Assert.java:1590)
       org.testng.Assert.assertEqualsImpl(Assert.java:150)
       org.testng.Assert.assertEquals(Assert.java:132)
       org.testng.Assert.assertEquals(Assert.java:656)
       org.testng.Assert.assertEquals(Assert.java:666)
       ServerTest.serverStarts(ServerTest.java:17)
       org.testng.internal.invokers.MethodInvocationHelper.invokeMethod(MethodInvocationHelper.java:141)
       org.testng.internal.invokers.TestInvoker.invokeMethod(TestInvoker.java:697)
       [...]

Test run finished after $TIME
[         5 containers found      ]
[         0 containers skipped    ]
[         5 containers started    ]
[         0 containers aborted    ]
[         5 containers successful ]
[         0 containers failed     ]
[         3 tests found           ]
[         0 tests skipped         ]
[         3 tests started         ]
[         1 tests aborted         ]
[         1 tests successful      ]
[         1 tests failed          ]
```

The report shows one failure and one skipped test. In the summary of the JUnit Platform, the skipped test counts as aborted, and the tree marks it with `■`. The skip message names the test instance. The class overrides `toString` so that the name is the same on each run, and the tree then also shows the name after each method. The message of the failure gives the expected value first, although the call gives the actual value first. In `org.testng.Assert`, the actual value is the first argument and the expected value is the second. The documentation says: "Note that the class org.testng.Assert uses a different argument ordering than the ones used by JUnit."[^1]

Use a dependency when a test has no meaning without the result of a different test, such as a test of a server that did not start. A dependency also makes an order of the tests, and it makes the result of one test depend on a different test. Most unit tests are better with no dependencies.

## Common tasks

**Run a suite from the command line.** "Assuming that you have TestNG in your class path, the simplest way to invoke TestNG is as follows":[^1]

```text
java org.testng.TestNG testng1.xml [testng2.xml testng3.xml ...]
```

**Select classes, groups and methods.** A suite file lists the classes or the packages, and can include and exclude groups and methods. This example is from the documentation:[^1]

```text
<test name="Regression1">
  <groups>
    <run>
      <exclude name="brokenTests"/>
      <include name="checkinTests"/>
    </run>
  </groups>
  <classes>
    <class name="test.IndividualMethodsTest">
      <methods>
        <include name="testMethod"/>
      </methods>
    </class>
  </classes>
</test>
```

**Run tests in parallel.** The `parallel` attribute of `<suite>` sets the mode, and `thread-count` sets the number of threads:[^1]

```text
<suite name="My suite" parallel="methods" thread-count="5">
```

The modes are `methods`, `tests`, `classes` and `instances`.[^1] For example, with `parallel="classes"`, "TestNG will run all the methods in the same class in the same thread, but each class will be run in a separate thread."[^1] With `parallel="methods"`, "Dependent methods will also run in separate threads but they will respect the order that you specified."[^1] A data provider can also give its rows to parallel threads with `@DataProvider(parallel = true)`.[^1]

**Run the TestNG tests with the JUnit Platform.** Add the TestNG engine, `org.junit.support:testng-engine`, to the test class path. Then the JUnit console launcher, Gradle and Maven Surefire run the TestNG tests with the JUnit tests in one report.[^2] The examples on this page run in this way.

**Run old JUnit tests with TestNG.** "TestNG can automatically recognize and run JUnit tests".[^1] Put the JUnit jar on the TestNG class path, and run TestNG in its mixed mode.[^1]

## Configuration

A suite file starts with the document type of TestNG and a `<suite>` element. A suite has one or more `<test>` elements. By default, "TestNG will run your tests in the order they are found in the XML file".[^1] The attribute `preserve-order="false"` gives an unpredictable order.[^1]

The `@Test` annotation has attributes for each test method. The documentation lists, among others:[^1]

- `groups`, the groups of the method;
- `dataProvider` and `dataProviderClass`, the source of the rows;
- `dependsOnMethods` and `dependsOnGroups`, the dependencies;
- `alwaysRun`, which makes a dependency soft;
- `enabled`, which turns a method off;
- `expectedExceptions`, the exceptions that make the test pass;
- `invocationCount`, `threadPoolSize` and `timeOut`, which run a method many times, in many threads, with a time limit.

## Alternatives

- [[junit-5]]: JUnit Jupiter has parameterized tests with many sources, tags, nested classes, and one extension API. It makes a new instance of the test class for each test method by default.[^3] It has no dependencies between test methods.
- [[junit]] 4: the older JUnit model with runners and rules. TestNG started as an answer to the limits of JUnit.[^1]
- With the TestNG engine, a project can keep its TestNG tests and write new tests in JUnit Jupiter, and run both in one build.[^2]

## History and versions

The documentation of version 7.9.0 is signed "TestNG 7.9.0 by Cédric Beust".[^1] The source code is in the GitHub organization `testng-team`.[^1] Cédric Beust and Hani Suleiman wrote a book about TestNG, Next Generation Java Testing.[^1]

The directory of TestNG in Maven Central has these dates. The oldest directory is 4.4.7, with the date 2006-01-23. Some old versions have a later date because they were uploaded later.[^4]

| Version | Date of the directory in Maven Central | Note |
|---|---|---|
| 5.0 | 2006-07-27 | |
| 6.0 | 2011-03-15 | |
| 7.0.0 | 2019-08-17 | |
| 7.5 | 2022-01-06 | the last version for JDK 8[^1] |
| 7.6.0 | 2022-05-18 | needs JDK 11 or higher[^1] |
| 7.9.0 | 2023-12-26 | the version of the documentation[^1] |
| 7.10.0 | 2024-04-07 | |
| 7.11.0 | 2025-02-13 | |
| 7.12.0 | 2026-01-22 | the version in the examples on this page |

## References
