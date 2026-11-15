---
tag: Q4XEJ4
kind: tool
slug: maven
name: Maven
aliases: [Apache Maven, mvn, mvnw, pom.xml, POM, Maven Central, Maven Wrapper]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Apache Maven builds Java projects from a project object model in pom.xml. A fixed lifecycle of phases runs plugin goals, and Maven gets the dependencies from repositories such as Maven Central.
role: [build, package-manager]
licence: apache-2.0
repo: https://github.com/apache/maven
homepage: https://maven.apache.org/
history:
  year: 2001
  people: [Jason van Zyl, Bob McWhirter]
  note: The first prototype sources were imported in August 2001 in the Jakarta Alexandria project, and the Jakarta Turbine project was its first test bed. Maven 1.0 came out on 13 July 2004, 2.0 on 20 October 2005 and 3.0 on 8 October 2010.
see_also: [gradle, build-system-concept, build-a-project, bazel, make, build-graph, incremental-builds, reproducible-builds]
sources:
  - url: https://maven.apache.org/what-is-maven.html
    title: Apache Maven, Introduction (What is Maven?)
    locator: Introduction; Maven's Objectives; Feature summary
  - url: https://maven.apache.org/guides/introduction/introduction-to-the-pom.html
    title: Apache Maven, Introduction to the POM
    locator: What is a POM?; Super POM; Minimal POM
  - url: https://maven.apache.org/background/history-of-maven.html
    title: Apache Maven, History of Maven, by Jason van Zyl
    locator: the whole page
  - url: https://maven.apache.org/guides/introduction/introduction-to-the-lifecycle.html
    title: Apache Maven, Introduction to the Build Lifecycle
    locator: Build Lifecycle Basics; Usual Command Line Calls; A Build Phase is Made Up of Plugin Goals; Lifecycle Reference; Built-in Lifecycle Bindings
  - url: https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html
    title: Apache Maven, Introduction to the Dependency Mechanism
    locator: Transitive Dependencies; Dependency Scope
  - url: https://maven.apache.org/guides/introduction/introduction-to-repositories.html
    title: Apache Maven, Introduction to Repositories
    locator: Artifact Repositories; Downloading from a Remote Repository
  - url: https://github.com/apache/maven/blob/maven-3.9.11/LICENSE
    title: Apache Maven licence, tag maven-3.9.11
    locator: the whole file (Apache License 2.0)
  - url: https://maven.apache.org/guides/getting-started/maven-in-five-minutes.html
    title: Apache Maven, Maven in 5 Minutes
    locator: Creating a Project; The POM; Build the Project; Java 9 or later; Maven Phases
  - url: https://maven.apache.org/guides/introduction/introduction-to-archetypes.html
    title: Apache Maven, Introduction to Archetypes
    locator: What is Archetype?
  - url: https://maven.apache.org/plugins/index.html
    title: Apache Maven, Available Plugins
    locator: the introduction
  - url: https://maven.apache.org/tools/wrapper/
    title: Apache Maven, Maven Wrapper
    locator: the introduction
  - url: https://maven.apache.org/guides/introduction/introduction-to-profiles.html
    title: Apache Maven, Introduction to Build Profiles
    locator: Explicit profile activation; How can I tell which profiles are in effect during a build?
  - url: https://maven.apache.org/settings.html
    title: Apache Maven, Settings Reference
    locator: Introduction (Quick Overview)
  - url: https://maven.apache.org/docs/history.html
    title: Apache Maven, Maven Releases History
    locator: the introduction; Maven 4.0+; the release tables for 1.0, 2.0, 3.0, 3.8.7 and 3.9.0
---

## What it is

Apache Maven is a build tool for Java projects. The Maven site says that the name is "a Yiddish word meaning accumulator of knowledge", and that Maven began as an attempt to make the builds of the Jakarta Turbine project simpler.[^1] Turbine had several projects, each with its own Ant build file, and the JAR files of the libraries were in CVS. The team wanted one standard way to build the projects, to describe them, and to share the JAR files between them.[^1]

A project describes itself in one XML file, `pom.xml`, the project object model (POM). Maven builds a project from its POM and a set of plugins.[^1] The POM gives the name and the version of the project, its dependencies and its plugins. Maven uses defaults for the rest: the sources are in `src/main/java`, the tests are in `src/test/java`, and the build output goes to `target`.[^2] Jason van Zyl, who wrote the first version, calls this approach "convention over configuration".[^3]

Maven does three jobs that other tools divide:

- It runs the build steps in a fixed order, the build lifecycle.[^4]
- It downloads the libraries that a project depends on, and the libraries that these depend on, from repositories.[^5][^6]
- It installs and deploys the result as an artifact that other projects can use.[^4]

Maven is free software under the Apache License 2.0.[^7]

## Install and first use

Maven is a Java program, so Java must be installed first. The command `mvn --version` shows the version of Maven and of Java.[^8] The command below makes a project from a template, an archetype:[^8][^9]

```text
mvn archetype:generate -DgroupId=com.mycompany.app -DartifactId=my-app -DarchetypeArtifactId=maven-archetype-quickstart -DarchetypeVersion=1.5 -DinteractiveMode=false
```

The script below makes a smaller project by hand: one class, one JUnit test and a `pom.xml`. The POM pins the versions of the compiler, Surefire and JAR plugins. The Maven documentation advises this, so that the build does not use the defaults of the Maven version that runs it.[^8] The property `maven.compiler.release` gives the Java release to compile for.[^8]

The script is shown as text and does not run when this site is built, because a Maven build on the build machine can take longer than the time limit of a run. The output after it is from one run on the build machine of this site, with Maven 3.8.7 and OpenJDK 25.0.5. The `grep` keeps only the goals, the test results and the result of the build. The variable `M2_REPO` names the local repository; without it, the run uses the default, `~/.m2/repository`.

```text
# A Maven project with one class, one test and a pom.xml.
# M2_REPO selects the local repository; the default is ~/.m2/repository.
d=$(mktemp -d)
cd "$d"
mkdir -p src/main/java/com/example src/test/java/com/example
cat > pom.xml <<'EOF'
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example</groupId>
  <artifactId>calc</artifactId>
  <version>1.0</version>

  <properties>
    <maven.compiler.release>21</maven.compiler.release>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>

  <dependencies>
    <dependency>
      <groupId>org.junit.jupiter</groupId>
      <artifactId>junit-jupiter-api</artifactId>
      <version>5.13.4</version>
      <scope>test</scope>
    </dependency>
  </dependencies>

  <build>
    <plugins>
      <plugin>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.14.0</version>
      </plugin>
      <plugin>
        <artifactId>maven-surefire-plugin</artifactId>
        <version>3.5.4</version>
      </plugin>
      <plugin>
        <artifactId>maven-jar-plugin</artifactId>
        <version>3.4.2</version>
      </plugin>
    </plugins>
  </build>
</project>
EOF
cat > src/main/java/com/example/Calc.java <<'EOF'
package com.example;

public class Calc {
    public static int add(int a, int b) {
        return a + b;
    }

    public static void main(String[] args) {
        System.out.println("2 + 3 = " + add(2, 3));
    }
}
EOF
cat > src/test/java/com/example/CalcTest.java <<'EOF'
package com.example;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

class CalcTest {
    @Test
    void addsTwoNumbers() {
        assertEquals(5, Calc.add(2, 3));
    }
}
EOF
mvn -B -Dmaven.repo.local="${M2_REPO:-$HOME/.m2/repository}" package 2>&1 |
  grep -E '^\[INFO\] (--- |Tests run:|BUILD)' |
  sed -E 's/ @ calc ---/ ---/; s/, Time elapsed: [0-9.]+ s//'
echo "--- the jar"
ls target/*.jar
java -cp target/calc-1.0.jar com.example.Calc
cd / && rm -rf "${d:?}"
```

The output of that run:

```text
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) ---
[INFO] --- maven-compiler-plugin:3.14.0:compile (default-compile) ---
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) ---
[INFO] --- maven-compiler-plugin:3.14.0:testCompile (default-testCompile) ---
[INFO] --- maven-surefire-plugin:3.5.4:test (default-test) ---
[INFO] Tests run: 1, Failures: 0, Errors: 0, Skipped: 0 -- in com.example.CalcTest
[INFO] Tests run: 1, Failures: 0, Errors: 0, Skipped: 0
[INFO] --- maven-jar-plugin:3.4.2:jar (default-jar) ---
[INFO] BUILD SUCCESS
--- the jar
target/calc-1.0.jar
2 + 3 = 5
```

`mvn package` is one word, but Maven ran six goals of four plugins. The POM does not pin the resources plugin, so Maven 3.8.7 used its default version, 2.6. The [Concepts](#concepts-of-the-tool) section below explains why. The JAR file has the name `calc-1.0.jar`, from the `artifactId` and the `version` in the POM.

## Concepts of the tool

**The POM and the coordinates.** A POM must give `modelVersion`, `groupId`, `artifactId` and `version`.[^2] The last three are the coordinates of the project, in the form `groupId:artifactId:version`, such as `com.mycompany.app:my-app:1`.[^2] Each POM extends the Super POM, which is the default POM of Maven. The Super POM gives the defaults, such as the packaging type `jar` and the central repository `https://repo.maven.apache.org/maven2`.[^2]

**Lifecycles, phases and goals.** Maven has three built-in lifecycles: `default` builds and deploys the project, `clean` removes the output, and `site` makes a web site for the project.[^4] A lifecycle is a list of phases in order. When a command gives a phase, Maven runs each phase in the list up to and including that phase.[^8] The work of a phase is done by plugin goals, such as `compiler:compile`, that are bound to it. Which goals are bound depends on the packaging type of the project.[^4] A plugin is a set of goals, and the Maven site says that Maven is "a plugin execution framework; all work is done by plugins".[^10]

The model below is not Maven. It has the 23 phases of the default lifecycle and the goals that Maven binds to them for the packaging type `jar`, from the lifecycle reference.[^4] For a phase, it gives the goals that run. The goals of `mvn package` are the same as in the output of the real run above. The model ran with Python 3.12.

```python run id=maven-lifecycle
# A model of the Maven default lifecycle. It is not Maven.
# The phases and the goals bound to them for jar packaging are
# from the Maven lifecycle reference.
PHASES = [
    "validate", "initialize", "generate-sources", "process-sources",
    "generate-resources", "process-resources", "compile", "process-classes",
    "generate-test-sources", "process-test-sources",
    "generate-test-resources", "process-test-resources", "test-compile",
    "process-test-classes", "test", "prepare-package", "package",
    "pre-integration-test", "integration-test", "post-integration-test",
    "verify", "install", "deploy",
]

JAR_BINDINGS = {
    "process-resources": "resources:resources",
    "compile": "compiler:compile",
    "process-test-resources": "resources:testResources",
    "test-compile": "compiler:testCompile",
    "test": "surefire:test",
    "package": "jar:jar",
    "install": "install:install",
    "deploy": "deploy:deploy",
}


def run(phase):
    """Run each phase up to and including the given phase."""
    last = PHASES.index(phase)
    goals = []
    for p in PHASES[: last + 1]:
        if p in JAR_BINDINGS:
            goals.append(JAR_BINDINGS[p])
    return goals


print(len(PHASES), "phases in the default lifecycle")
for phase in ["compile", "package", "install"]:
    print(f"mvn {phase}:", " ".join(run(phase)))


# Dependency mediation: the nearest definition wins.
# The tree is the example of the Maven dependency guide.
TREE = {
    "A": ["B", "E"],
    "B": ["C"],
    "C": ["D 2.0"],
    "E": ["D 1.0"],
}


def resolve(root):
    chosen = {}
    queue = [(root, 0)]
    while queue:
        node, depth = queue.pop(0)
        for child in TREE.get(node, []):
            name, _, version = child.partition(" ")
            if version and name not in chosen:
                chosen[name] = (version, depth + 1)
                print(f"  {name} {version} at depth {depth + 1}: chosen")
            elif version:
                print(f"  {name} {version} at depth {depth + 1}: omitted")
            queue.append((child, depth + 1))
    return chosen


print("dependencies of A:")
resolve("A")
```

```output for=maven-lifecycle
23 phases in the default lifecycle
mvn compile: resources:resources compiler:compile
mvn package: resources:resources compiler:compile resources:testResources compiler:testCompile surefire:test jar:jar
mvn install: resources:resources compiler:compile resources:testResources compiler:testCompile surefire:test jar:jar install:install
dependencies of A:
  D 1.0 at depth 2: chosen
  D 2.0 at depth 3: omitted
```

A command can mix phases and goals, and Maven runs them in the order of the command. For example, `mvn clean dependency:copy-dependencies package` runs the `clean` lifecycle up to `clean`, then the goal `dependency:copy-dependencies`, and then the default lifecycle up to `package`.[^4][^8]

**Dependencies.** A POM lists the dependencies of the project, and Maven reads the POMs of these dependencies to find their dependencies. These are transitive dependencies, and there is no limit to the number of levels.[^5] When the graph has more than one version of a library, Maven uses the "nearest definition": the version that is closest to the project in the tree.[^5] The second part of the model above uses the example of the Maven guide: A depends on D 2.0 through B and C, and on D 1.0 through E. Maven uses D 1.0, because its path is shorter.[^5] A project can always select a version: it can declare the dependency itself, or give the version in its `dependencyManagement` section.[^5]

Each dependency has a scope. The guide gives six scopes:[^5]

| Scope | Use |
|---|---|
| `compile` (the default) | on all the classpaths of the project |
| `provided` | on the classpaths for compiling and testing; the JDK or a container gives it at run time |
| `runtime` | on the classpaths for testing and running, not for compiling |
| `test` | only for compiling and running the tests |
| `system` | for compiling and running, but Maven does not download it: it uses a JAR at a given path on the local file system |
| `import` | only in `dependencyManagement`, for a POM of type `pom`; Maven replaces it with the managed dependencies of that POM |

The scope also controls which transitive dependencies a project gets. The dependencies of a `compile` or `runtime` dependency come with it, but `provided` and `test` dependencies are not transitive.[^5]

The run above declares JUnit with the scope `test`, so the JUnit library is available to `CalcTest.java` but not to `Calc.java`.

**Repositories.** A Maven repository holds artifacts. There are exactly two types: the local repository, a directory on the computer that runs Maven, and remote repositories.[^6] The local repository is a cache of the downloads and holds the artifacts that the user has built but not released.[^6] When a project needs an artifact that is not in the local repository, Maven downloads it from a remote repository. The Super POM gives the central repository, so a project with no other repositories uses it.[^2][^6] The phase `install` copies the artifact into the local repository, and `deploy` copies it to a remote repository.[^4]

## Common tasks

| Task | Command |
|---|---|
| Compile, test and make the JAR file | `mvn package`[^4] |
| Run the unit tests | `mvn test`[^4] |
| Build and run all checks, also integration tests | `mvn verify`[^4] |
| Put the artifact in the local repository | `mvn install`[^4] |
| Remove the output, then build and deploy | `mvn clean deploy`[^4] |
| Make the project web site | `mvn site`[^8] |
| Add the Maven Wrapper to a project | `mvn wrapper:wrapper`[^11] |
| Build with the version of Maven that the project gives | `./mvnw clean install`[^11] |
| Show the profiles in effect | `mvn help:active-profiles`[^12] |

The lifecycle guide says: "If you are uncertain what you want, the preferred phase to call is" `verify`.[^4] This phase comes after `package`, so it also makes the JAR file, and it runs the checks on the results of the integration tests.[^4]

## Configuration

**The POM.** Most of the configuration is in `pom.xml`. The `<build>` element configures the build plugins, and the `<reporting>` element configures the plugins that make reports for the site.[^10] The `<pluginManagement>` section pins the versions of plugins, so that the build does not depend on the defaults of a given Maven version.[^8]

**Settings.** The file `settings.xml` holds the configuration that does not belong to one project, such as the location of the local repository, other remote repositories, and passwords.[^13] It can be in the Maven installation, at `${maven.home}/conf/settings.xml`, and in the home directory of the user, at `${user.home}/.m2/settings.xml`. If both exist, Maven merges them, and the user settings win.[^13]

**The .mvn directory.** Maven reads the options for each build from `.mvn/maven.config` and the options of the Java virtual machine from `.mvn/jvm.config`.[^8]

**Profiles.** A profile changes parts of the build for given conditions, such as a different JDK or operating system.[^12] The option `-P` with a list of profile ids turns on profiles by hand.[^12]

**The Maven Wrapper.** The scripts `mvnw` and `mvnw.cmd` run a build with the version of Maven that the file `.mvn/wrapper/maven-wrapper.properties` gives. If that version is not on the computer, the wrapper downloads it first.[^11]

## Alternatives

- [[gradle]]: a build tool for Java and other languages, with build scripts in Kotlin or Groovy.
- [[bazel]]: a build system for large repositories in many languages. Its FAQ compares it with Maven.
- [[make]]: a general build tool with no lifecycle and no dependency download.

The page [[build-a-project]] shows a build in each of several languages, and [[reproducible-builds]] explains how a build can give the same files each time.

## History and versions

Jason van Zyl wrote the history of Maven. The first prototype sources were imported in August 2001 in the Jakarta Alexandria project, and Maven then moved to the Jakarta Turbine project, which was its first test bed.[^3] He wanted one model for each project and one standard directory structure. He also wanted to stop storing copies of the JAR files in CVS, so he made Maven declare the dependencies, and he set up a repository at Ibiblio.[^3] He wrote the first version "with lots of help from Bob McWhirter".[^3]

The release history gives these dates:[^14]

| Version | Release date |
|---|---|
| 1.0 | 13 July 2004 |
| 2.0 | 20 October 2005 |
| 3.0 | 8 October 2010 |
| 3.8.7 | 24 December 2022 |
| 3.9.0 | 31 January 2023 |
| 4.0.0-rc-7 | 24 September 2026 |

When this page was written, the Maven team maintained 3.10.1 and 3.9.16, the last versions of the last two release series. Older versions have reached their end of life.[^14] Maven 4.0 was not yet generally available, and its release candidates need Java 17.[^14] The Maven 3.8.7 that made the output on this page has reached its end of life.[^14]

## References
