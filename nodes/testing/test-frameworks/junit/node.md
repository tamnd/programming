---
tag: B6HKSD
kind: tool
slug: junit
name: JUnit
aliases: [JUnit 4, JUnit 3]
area: testing/test-frameworks
level: core
summary: >-
  JUnit is the main test framework of Java and the first widely known xUnit framework. This page follows it from the TestCase classes of JUnit 3 to the runners and rules of JUnit 4.
role: [test-framework]
licence: epl-2.0
repo: https://github.com/junit-team/junit-framework
homepage: https://junit.org
history:
  year: 1997
  people: [Kent Beck, Erich Gamma]
  note: Kent Beck and Erich Gamma wrote the first version of JUnit in 1997, on a flight to the OOPSLA conference. JUnit 4 moved from a base class to annotations, and JUnit 5 replaced runners and rules with a launcher platform and one extension API.
see_also: [junit-5, testng, test-framework, unit-testing, test-driven-development, java]
same_as:
  wikidata: Q847675
  wikipedia: JUnit
sources:
  - url: https://junit.org/junit4/
    title: JUnit 4, About
    locator: the whole page
    accessed: 2026-11-15
  - url: https://junit.org/junit4/faq.html
    title: JUnit FAQ
    locator: Overview, What is JUnit?; Writing Tests
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/blob/r4.13.2/src/main/java/junit/framework/TestCase.java
    title: JUnit 4.13.2 source, junit.framework.TestCase
    locator: the class comment
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/blob/r4.13.2/src/main/java/junit/framework/TestSuite.java
    title: JUnit 4.13.2 source, junit.framework.TestSuite
    locator: the comment of the constructor TestSuite(Class)
    accessed: 2026-11-15
  - url: https://junit.org/junit4/cookbook.html
    title: JUnit Cookbook
    locator: Simple Test Case; Fixture; Running Tests; Expected Exceptions
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/wiki/Test-runners
    title: JUnit 4 wiki, Test runners
    locator: "@RunWith annotation; Specialized Runners"
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/wiki/Rules
    title: JUnit 4 wiki, Rules
    locator: introduction; ExternalResource Rules
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/migrating-from-junit4.html
    title: JUnit User Guide 6.1.3, Migrating from JUnit 4
    locator: introduction; Running JUnit 4 Tests on the JUnit Platform; Migration Tips
    accessed: 2026-11-15
  - url: https://repo1.maven.org/maven2/junit/junit/
    title: Maven Central, the directory junit/junit
    locator: the dates of the version directories
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/running-tests/build-support.html
    title: JUnit User Guide 6.1.3, Build Support
    locator: Gradle, Configuring Test Engines
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/blob/r4.13.2/doc/ReleaseNotes4.13.md
    title: JUnit 4.13 release notes
    locator: Add assertThrows; Deprecate Assert#assertThat; Deprecate ExpectedException.none()
    accessed: 2026-11-15
  - url: https://martinfowler.com/bliki/Xunit.html
    title: Martin Fowler, Xunit, bliki, 17 January 2006
    locator: the whole page
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/wiki/Parameterized-tests
    title: JUnit 4 wiki, Parameterized tests
    locator: Identify Individual test cases
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/blob/r4.13.2/doc/ReleaseNotes4.7.md
    title: JUnit 4.7 release notes
    locator: Rules
    accessed: 2026-11-15
  - url: https://junit.org/junit4/javadoc/4.13.2/org/junit/rules/TestRule.html
    title: JUnit 4.13.2 API, Interface TestRule
    locator: the class description
    accessed: 2026-11-15
  - url: https://junit.org/junit4/javadoc/4.13.2/org/junit/Rule.html
    title: JUnit 4.13.2 API, Annotation Type Rule
    locator: the description
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit4/blob/r4.13.2/doc/ReleaseNotes4.4.md
    title: JUnit 4.4 release notes
    locator: assertThat; Assumptions; Theories
    accessed: 2026-11-15
  - url: https://testng.org/
    title: TestNG Documentation
    locator: 1. Welcome to TestNG
    accessed: 2026-11-15
  - url: https://github.com/junit-team/junit-framework/releases
    title: junit-team/junit-framework, Releases on GitHub
    locator: the releases r5.0.0, r6.0.0 and r6.1.3
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/release-notes.html
    title: JUnit User Guide 6.1.3, Release Notes
    locator: 6.0.0
    accessed: 2026-11-15
  - url: https://docs.junit.org/6.1.3/overview.html
    title: JUnit User Guide 6.1.3, Overview
    locator: What is JUnit?
    accessed: 2026-11-15
---

## What it is

JUnit is the main [[test-framework]] of [[java]]. The JUnit 4 site describes it so: "JUnit is a simple framework to write repeatable tests. It is an instance of the xUnit architecture for unit testing frameworks."[^1] The FAQ lists its parts: "Assertions for testing expected results", "Test fixtures for sharing common test data" and "Test runners for running tests".[^2] It also says: "JUnit was originally written by Erich Gamma and Kent Beck."[^2]

The name covers several generations with different models:

- JUnit 3 and earlier: a test class extends `junit.framework.TestCase`, and each method whose name starts with `test` is a test.[^3][^4]
- JUnit 4: annotations such as `@Test` and `@Before` mark the tests, `@RunWith` picks a runner, and `@Rule` wraps tests in extra behaviour.[^5][^6][^7]
- JUnit 5 and JUnit 6: a launcher platform with test engines, the Jupiter programming model and one extension API. See [[junit-5]].

This page covers the product, the JUnit 3 and JUnit 4 models, and the versions. The examples run JUnit 4.13.2 on the JUnit Platform, through the Vintage engine.[^8]

## Install and first use

JUnit 4 is the jar `junit:junit`. The last version is 4.13.2.[^9] In a Maven `pom.xml`:

```text
<dependency>
  <groupId>junit</groupId>
  <artifactId>junit</artifactId>
  <version>4.13.2</version>
  <scope>test</scope>
</dependency>
```

A Gradle build that runs JUnit 4 tests on the JUnit Platform adds `junit:junit:4.13.2` and the engine `org.junit.vintage:junit-vintage-engine`.[^10] JUnit 4 also has its own console runner: `java org.junit.runner.JUnitCore` followed by the names of the test classes.[^5]

The cookbook gives the steps for a JUnit 4 test: "Annotate a method with @org.junit.Test", and for a fixture, "Annotate a method with @org.junit.Before and initialize the variables in that method".[^5]

```java test id=first-use file=PlaylistTest.java deps=deps/java/pom.xml
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertThrows;

import java.util.ArrayDeque;
import java.util.Deque;
import org.junit.Before;
import org.junit.Test;

public class PlaylistTest {
    static final class Playlist {
        private final Deque<String> songs = new ArrayDeque<>();

        void add(String song) {
            songs.addLast(song);
        }

        String next() {
            String song = songs.pollFirst();
            if (song == null) {
                throw new IllegalStateException("the playlist is empty");
            }
            return song;
        }
    }

    private Playlist playlist;

    @Before
    public void setUp() {
        playlist = new Playlist();
        playlist.add("Intro");
    }

    @Test
    public void nextReturnsSongsInOrder() {
        playlist.add("Outro");
        assertEquals("Intro", playlist.next());
        assertEquals("Outro", playlist.next());
    }

    @Test
    public void nextOnEmptyPlaylistFails() {
        playlist.next();
        assertThrows(IllegalStateException.class, () -> playlist.next());
    }

    @Test(expected = IllegalStateException.class)
    public void theOlderWayToExpectAnException() {
        playlist.next();
        playlist.next();
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
└─ JUnit Vintage ✔
   └─ PlaylistTest ✔
      ├─ theOlderWayToExpectAnException ✔
      ├─ nextOnEmptyPlaylistFails ✔
      └─ nextReturnsSongsInOrder ✔

Test run finished after $TIME
[         4 containers found      ]
[         0 containers skipped    ]
[         4 containers started    ]
[         0 containers aborted    ]
[         4 containers successful ]
[         0 containers failed     ]
[         3 tests found           ]
[         0 tests skipped         ]
[         3 tests started         ]
[         0 tests aborted         ]
[         3 tests successful      ]
[         0 tests failed          ]
```

The report shows the class under the JUnit Vintage engine. `assertThrows` came in JUnit 4.13.[^11] Before it, a test gave the class of the exception in `@Test(expected = ...)`, as the last test does.[^5] That form passes when any line of the method throws the exception, so it can hide an exception from the wrong line.

## Concepts of the tool

### JUnit 3: a base class and a naming rule

In JUnit 3, the comment of `TestCase` gives the steps: "implement a subclass of TestCase", "define instance variables that store the state of the fixture", "initialize the fixture state by overriding setUp()" and "clean-up after a test by overriding tearDown()."[^3] The comment also says: "Each test runs in its own fixture so there can be no side effects among test runs."[^3] A `TestSuite` made from a class "Adds all the methods starting with "test" as test cases to the suite."[^4] So the name of a method decides if it is a test.

JUnit 4.13.2 still contains `junit.framework`, so a JUnit 3 test still runs:

```java test id=junit3 file=TallyTest.java deps=deps/java/pom.xml
import junit.framework.TestCase;

public class TallyTest extends TestCase {
    private StringBuilder tally;

    @Override
    protected void setUp() {
        tally = new StringBuilder();
    }

    public void testStartsEmpty() {
        assertEquals("", tally.toString());
    }

    public void testMarksAddUp() {
        tally.append("||");
        tally.append("|");
        assertEquals(3, tally.length());
    }

    // The name does not start with "test", so JUnit 3 does not run this method.
    public void checkNothing() {
        fail("this method is not a test");
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
└─ JUnit Vintage ✔
   └─ TallyTest ✔
      ├─ testStartsEmpty ✔
      └─ testMarksAddUp ✔

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

The method `checkNothing` would fail, but it is not in the report, because it is not a test. A typo in the prefix, such as `tset`, has the same effect: the test does not run, and nothing tells you.

In JUnit 3 and JUnit 4, the optional message of an assertion is the first argument, as in `assertEquals("wrong total", 3, total)`. Martin Fowler remembers this choice of Beck and Gamma as a break with the Java convention of optional arguments at the end.[^12] Jupiter moved the message to the last argument.[^8]

### JUnit 4: annotations, runners and rules

JUnit 4 marks tests with annotations, so a test class does not extend a base class, and a test method can have any name. `@Before` and `@After` run around each test, and `@BeforeClass` and `@AfterClass` run once for the class.[^2][^5]

**Runners.** A runner is the class that runs the tests of one class. "When a class is annotated with @RunWith or extends a class annotated with @RunWith , JUnit will invoke the class it references to run the tests in that class instead of the runner built into JUnit."[^6] The default runner is `BlockJUnit4ClassRunner`.[^6] JUnit has runners for suites, for categories and for parameterized tests.[^6] A class has one `@RunWith` annotation, so it can use only one runner.

"Parameterized is a standard runner that implements parameterized tests."[^6] The class gives a static method with `@Parameters` that returns the rows, and the constructor takes one row. The `name` of `@Parameters` can use `{index}` and `{0}`, `{1}` for the values.[^13]

```java test id=parameterized file=GradeTest.java deps=deps/java/pom.xml
import static org.junit.Assert.assertEquals;

import java.util.Arrays;
import java.util.Collection;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

@RunWith(Parameterized.class)
public class GradeTest {
    static String grade(int score) {
        if (score >= 90) {
            return "A";
        }
        if (score >= 75) {
            return "B";
        }
        if (score >= 50) {
            return "C";
        }
        return "F";
    }

    @Parameters(name = "{0} gives {1}")
    public static Collection<Object[]> rows() {
        return Arrays.asList(new Object[][] {{95, "A"}, {75, "B"}, {74, "C"}, {10, "F"}});
    }

    private final int score;
    private final String expected;

    public GradeTest(int score, String expected) {
        this.score = score;
        this.expected = expected;
    }

    @Test
    public void gradeMatches() {
        assertEquals(expected, grade(score));
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
└─ JUnit Vintage ✔
   └─ GradeTest ✔
      ├─ [95 gives A] ✔
      │  └─ gradeMatches[95 gives A] ✔
      ├─ [75 gives B] ✔
      │  └─ gradeMatches[75 gives B] ✔
      ├─ [74 gives C] ✔
      │  └─ gradeMatches[74 gives C] ✔
      └─ [10 gives F] ✔
         └─ gradeMatches[10 gives F] ✔

Test run finished after $TIME
[         8 containers found      ]
[         0 containers skipped    ]
[         8 containers started    ]
[         0 containers aborted    ]
[         8 containers successful ]
[         0 containers failed     ]
[         4 tests found           ]
[         0 tests skipped         ]
[         4 tests started         ]
[         0 tests aborted         ]
[         4 tests successful      ]
[         0 tests failed          ]
```

**Rules.** JUnit 4.7 added rules.[^14] "Rules allow very flexible addition or redefinition of the behavior of each test method in a test class."[^7] A `TestRule` "is an alteration in how a test method, or set of test methods, is run and reported."[^15] A field with `@Rule` "must be public, not static, and a subtype of TestRule (preferred) or MethodRule", and the rule applies to each test.[^16] A class can have more than one rule.[^16] `@ClassRule` is for static fields "that can affect the operation of a whole class".[^7] JUnit gives rules such as `TemporaryFolder`, `ErrorCollector`, `TestName`, `Timeout` and `ExternalResource`.[^7] `ExternalResource` is "a base class for Rules (like TemporaryFolder) that set up an external resource before a test".[^7] This rule sets the default locale before each test and puts the old one back after it:

```java test id=rule file=PriceFormatTest.java deps=deps/java/pom.xml
import static org.junit.Assert.assertEquals;

import java.util.Locale;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExternalResource;

public class PriceFormatTest {
    @Rule
    public final ExternalResource germanLocale = new ExternalResource() {
        private Locale saved;

        @Override
        protected void before() {
            saved = Locale.getDefault();
            Locale.setDefault(Locale.GERMANY);
        }

        @Override
        protected void after() {
            Locale.setDefault(saved);
        }
    };

    @Test
    public void usesCommaForDecimals() {
        assertEquals("1,50", String.format("%.2f", 1.5));
    }

    @Test
    public void groupsThousandsWithDots() {
        assertEquals("1.234.567", String.format("%,d", 1234567));
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
└─ JUnit Vintage ✔
   └─ PriceFormatTest ✔
      ├─ groupsThousandsWithDots ✔
      └─ usesCommaForDecimals ✔

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

**Assumptions and matchers.** JUnit 4.4 added `assertThat` with Hamcrest matchers, assumptions and theories.[^17] "Tests with failed assumptions are now marked as Ignored, rather than silently passing."[^17] JUnit 4.13 deprecated `Assert.assertThat` and recommends the `assertThat` of Hamcrest itself.[^11]

## Common tasks

**Run JUnit 3 and JUnit 4 tests on the JUnit Platform.** Put the Vintage engine and the JUnit 4 jar on the test class path. Then "JUnit 3 and JUnit 4 tests will automatically be picked up by the JUnit Platform launcher."[^8] The examples on this page run in this way. The guide also warns that the Vintage engine "is deprecated and should only be used temporarily while migrating tests".[^8]

**Migrate a JUnit 4 class to Jupiter.** The migration guide lists the changes:[^8]

| JUnit 4 | JUnit Jupiter |
|---|---|
| `org.junit.Test` | `org.junit.jupiter.api.Test` |
| `@Before`, `@After` | `@BeforeEach`, `@AfterEach` |
| `@BeforeClass`, `@AfterClass` | `@BeforeAll`, `@AfterAll` |
| `@Ignore` | `@Disabled` |
| `@Category` | `@Tag` |
| `@RunWith` | `@ExtendWith` |
| `@RunWith(Parameterized.class)` | `@ParameterizedClass` or `@ParameterizedTest` |
| `@Rule`, `@ClassRule` | `@ExtendWith`, `@RegisterExtension` |
| `@Test(expected = ...)`, `ExpectedException` | `assertThrows` |
| message as the first argument | message as the last argument |

The two versions use the same simple names in different packages. A class that takes some annotations from `org.junit` and some from `org.junit.jupiter.api` compiles, but each engine sees only its own annotations. [[junit-5]] shows such a test that fails. Change the imports of one class at a time, and run the tests after each change.

**Expect an exception.** In JUnit 4.13 or later, use `assertThrows`. It returns the exception, so the test can check the message too.[^11] The `ExpectedException` rule is older, and JUnit 4.13 deprecated `ExpectedException.none()`.[^11]

## Configuration

JUnit 4 has little configuration of its own. The build tool picks the test classes, and annotations configure each class. On the JUnit Platform, the Vintage engine reads configuration parameters. For example, `junit.vintage.execution.parallel.enabled=true`, together with `junit.vintage.execution.parallel.classes=true` or `junit.vintage.execution.parallel.methods=true`, runs JUnit 4 classes or methods in parallel.[^8]

## Alternatives

**JUnit 5 and JUnit 6** are the current generations. New tests should use Jupiter. See [[junit-5]].

**TestNG** started as a reaction to limits of JUnit. Its author writes: "I started TestNG out of frustration for some JUnit deficiencies".[^18] It has groups, dependencies between test methods and data providers. See [[testng]].

## History and versions

Martin Fowler tells the start: "JUnit was born on a flight from Zurich to the 1997 OOPSLA in Atlanta. Kent was flying with Erich Gamma".[^12] Beck had built small unit test frameworks in Smalltalk before.[^12] Fowler writes that "it's fair to give JUnit the credit for spreading the idea more."[^12] Ports to other languages followed, and the family took the name xUnit.[^12]

JUnit 4 is now old. Its site says: "JUnit 4 is in maintenance mode."[^1] It adds: "At this point, only critical bugs and security issues will be fixed."[^1] JUnit 5.0.0 came in September 2017, and JUnit 6.0.0 in September 2025, with "Single version number for Platform, Jupiter, and Vintage".[^19][^20] In 6.1.3, "JUnit 6.1.3 = JUnit Platform + JUnit Jupiter + JUnit Vintage".[^21]

The dates of JUnit 3 and JUnit 4 below are the dates of the version directories on Maven Central. The directories of 3.7, 3.8 and 3.8.1 have the same date and time, so their dates are those of an upload, not of the releases.[^9] The dates of JUnit 5 and JUnit 6 are those of the releases on GitHub.[^19]

| Version | Date | Notes |
|---|---|---|
| 3.8.1 | 2005-09-20 | The `TestCase` model: a base class, `setUp` and `tearDown`, and methods that start with `test`[^9][^3][^4] |
| 4.0 | 2006-02-18 | The first JUnit 4 release on Maven Central[^9] |
| 4.4 | 2007-08-21 | `assertThat` with Hamcrest matchers, assumptions and theories[^9][^17] |
| 4.7 | 2009-08-12 | Rules[^9][^14] |
| 4.13 | 2020-01-01 | `assertThrows`; `Assert.assertThat` and `ExpectedException.none()` deprecated[^9][^11] |
| 4.13.2 | 2021-02-13 | The last release of JUnit 4[^9] |
| 5.0.0 | 2017-09-10 | The JUnit Platform, Jupiter and Vintage; see [[junit-5]][^19] |
| 6.0.0 | 2025-09-30 | One version number for all parts; Java 17 or later[^19][^20] |
| 6.1.3 | 2026-08-07 | The current release at the date of this page[^19] |

## References
