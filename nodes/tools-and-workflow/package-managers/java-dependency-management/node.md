---
tag: ZE0RJ6
kind: practice
slug: java-dependency-management
name: Java dependency management
aliases: [Java dependencies, Maven dependencies, Gradle dependencies, dependencyManagement, bill of materials, BOM, nearest definition, dependency mediation, Maven Central, version catalog, gradle.lockfile]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Name each Java dependency by its coordinates, let Maven or Gradle get the transitive ones, set the versions of shared libraries in one place, because the two tools pick versions by different rules.
see_also: [dependency-resolution, transitive-dependencies, version-constraints, lock-files, maven, gradle, java, package-registries]
sources:
  - url: https://maven.apache.org/pom.html
    title: Apache Maven, POM Reference
    locator: Maven Coordinates; Dependency Version Requirement Specification
  - url: https://docs.gradle.org/current/userguide/dependency_management_basics.html
    title: Gradle User Manual, Dependency Management Basics
    locator: Declaring Your Dependencies; Viewing Project Dependencies
  - url: https://maven.apache.org/guides/introduction/introduction-to-repositories.html
    title: Apache Maven, Introduction to Repositories
    locator: Artifact Repositories; Downloading from a Remote Repository
  - url: https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html
    title: Apache Maven, Introduction to the Dependency Mechanism
    locator: Transitive Dependencies; Dependency Scope; Dependency Management; Importing Dependencies; Bill of Materials (BOM) POMs
  - url: https://docs.gradle.org/current/userguide/graph_resolution.html
    title: Gradle User Manual, Graph Resolution
    locator: Version conflicts
  - url: https://docs.gradle.org/current/userguide/version_catalogs.html
    title: Gradle User Manual, Version Catalogs
    locator: the introduction
  - url: https://maven.apache.org/plugins/maven-dependency-plugin/tree-mojo.html
    title: Apache Maven Dependency Plugin, dependency:tree
    locator: the description; the parameter verbose
  - url: https://maven.apache.org/enforcer/enforcer-rules/dependencyConvergence.html
    title: Apache Maven Enforcer, Dependency Convergence
    locator: the introduction
  - url: https://docs.gradle.org/current/userguide/dependency_locking.html
    title: Gradle User Manual, Locking Versions
    locator: the introduction; Generating and updating dependency locks
  - url: https://central.sonatype.org/publish/requirements/
    title: Sonatype, Maven Central Requirements
    locator: Supply Javadoc and Sources; Provide File Checksums
---

## Statement

Name each dependency of a Java project by its coordinates and an exact version, in the build file. Let the build tool get the dependencies of those dependencies from a repository. When two parts of the graph need different versions of one library, do not trust the default rule of the tool: set the version in one place, such as `dependencyManagement` in Maven or a version catalog in Gradle, and look at the resolved tree.

## Meaning

**Coordinates.** Maven names each artifact by three values: `groupId`, `artifactId` and `version`. The POM reference says that they "act much like an address and timestamp in one".[^1] Gradle uses the same coordinates, in the form `group:name:version`, for example `implementation("com.google.guava:guava:32.1.2-jre")`.[^2] The pages [[maven]] and [[gradle]] describe the two tools.

**Repositories.** Maven has two types of repositories. The local repository is a directory on the computer that caches the downloads, and a remote repository is any other repository, for example a server of a company or the central repository.[^3] By default, Maven downloads from the central repository.[^3] The page [[package-registries]] describes registries in general.

**Transitive dependencies.** Maven reads the project files of the dependencies and adds their dependencies automatically. The guide says: "There is no limit to the number of levels that dependencies can be gathered from."[^4] The graph can become large, so Maven has more controls:[^4]

- Dependency scope: a dependency is only in the classpaths of some stages of the build. There are six scopes. `compile` is the default, and `test` is only for the compilation and the run of the tests.[^4]
- Excluded dependencies: if X depends on Y and Y depends on Z, the owner of X can exclude Z.[^4]
- Optional dependencies: the owner of Y can mark Z as optional. Then X depends only on Y, not on Z.[^4]

**Two rules for one conflict.** Maven and Gradle pick a version by different rules.

- Maven uses dependency mediation. It picks the "nearest definition", the version that is closest to the project in the tree. If two versions are at the same depth, the first declaration wins.[^4]
- Gradle "considers all requested versions across the dependency graph and, by default, selects the highest version".[^5]

The Maven guide gives this tree: A depends on B, B on C, and C on D 2.0. A also depends on E, and E on D 1.0. Maven uses D 1.0, because the path through E is shorter.[^4] So in Maven, a project can get a lower version than a library in its tree needs. The page [[dependency-resolution]] compares more rules.

**Soft and hard requirements.** In Maven, a version such as `1.0` is a soft requirement: "Use 1.0 if no other version appears earlier in the dependency tree."[^1] A range such as `[1.0,2.0)` or an exact version `[1.0]` is a hard requirement. If no version satisfies all the hard requirements, the build fails.[^1]

**One place for the versions.** The `dependencyManagement` section of a POM sets the versions of artifacts "when they are encountered in transitive dependencies or in dependencies where no version has been specified".[^4] A project can inherit it from a parent POM. A project has only one parent, so it can also import the `dependencyManagement` of another POM with the scope `import`.[^4] A bill of materials (BOM) is a POM that sets the versions of all the artifacts of a library, so that other projects import it and use one set of versions.[^4] In Gradle, a version catalog, the file `gradle/libs.versions.toml`, lists the dependencies and their versions for all the projects of a build.[^6]

## Examples

**The two rules on the tree of the guide.** The model below is not Maven and not Gradle. It applies the nearest definition rule and the highest version rule to the tree of the Maven guide. It ran with Python 3.12.

```python run id=java-mediation
# A model of two rules for version conflicts, on the tree of the Maven guide:
# A -> B -> C -> D 2.0 and A -> E -> D 1.0. It is not Maven and not Gradle.
from collections import deque

deps = {
    "A": [("B", "1.0"), ("E", "1.0")],
    "B": [("C", "1.0")],
    "C": [("D", "2.0")],
    "E": [("D", "1.0")],
}

def requests(root):
    """Yield (name, version, depth, path) in breadth-first order."""
    queue = deque([(root, 0, root)])
    while queue:
        node, depth, path = queue.popleft()
        for name, version in deps.get(node, []):
            yield name, version, depth + 1, f"{path} -> {name} {version}"
            queue.append((name, depth + 1, f"{path} -> {name}"))

def version_key(v):
    return tuple(int(x) for x in v.split("."))

def nearest(root, managed=None):
    """Maven: the closest declaration wins; dependencyManagement overrides it."""
    chosen = {}
    for name, version, depth, path in requests(root):
        chosen.setdefault(name, (version, path))
    if managed:
        for name, version in managed.items():
            chosen[name] = (version, "dependencyManagement in A")
    return chosen

def highest(root):
    """Gradle: the highest of all the requested versions wins."""
    chosen = {}
    for name, version, depth, path in requests(root):
        if name not in chosen or version_key(version) > version_key(chosen[name][0]):
            chosen[name] = (version, path)
    return chosen

for _, _, _, path in requests("A"):
    print(path)
for title, result in [
    ("nearest definition", nearest("A")),
    ("highest version", highest("A")),
    ("nearest definition, D 2.0 managed", nearest("A", {"D": "2.0"})),
]:
    version, why = result["D"]
    print(f"{title:34} D {version}  ({why})")
```

```output for=java-mediation
A -> B 1.0
A -> E 1.0
A -> B -> C 1.0
A -> E -> D 1.0
A -> B -> C -> D 2.0
nearest definition                 D 1.0  (A -> E -> D 1.0)
highest version                    D 2.0  (A -> B -> C -> D 2.0)
nearest definition, D 2.0 managed  D 2.0  (dependencyManagement in A)
```

**The same tree with Maven.** The script below makes the artifacts of that tree, installs them into a temporary local repository, and runs `dependency:tree` on A with the parameter `verbose`, which shows the omitted nodes.[^7] Then it adds D 2.0 to the `dependencyManagement` of A and runs the goal again.

The script is shown as text and does not run when this site is built, because Maven must first download its plugins into the empty local repository, and that takes longer than the time limit of a run. The output after it is from one run on the build machine of this site, with Maven 3.8.7 and OpenJDK 25.

```text
# Build the tree of the Maven guide with real artifacts:
# A -> B -> C -> D 2.0 and A -> E -> D 1.0.
# The local repository is a temporary folder, so Maven gets its plugins
# from Maven Central and installs the example artifacts only there.
d=$(mktemp -d)
cd "$d"
repo="$d/repo"
# Maven 3.8.7 on Java 25 prints warnings about sun.misc.Unsafe; this turns them off.
export MAVEN_OPTS="--sun-misc-unsafe-memory-access=allow"
mvn -B --version | head -1
# mk ARTIFACT VERSION [DEP:VERSION]...: write a pom.xml and install it
mk() {
  a=$1 v=$2; shift 2
  mkdir -p "$a-$v"
  {
    echo '<project xmlns="http://maven.apache.org/POM/4.0.0">'
    echo '  <modelVersion>4.0.0</modelVersion>'
    echo "  <groupId>com.example</groupId><artifactId>$a</artifactId><version>$v</version>"
    echo '  <properties><maven.compiler.release>17</maven.compiler.release></properties>'
    echo '  <dependencies>'
    for dep in "$@"; do
      echo "    <dependency><groupId>com.example</groupId><artifactId>${dep%:*}</artifactId><version>${dep#*:}</version></dependency>"
    done
    echo '  </dependencies>'
    echo '</project>'
  } > "$a-$v/pom.xml"
  mvn -q -B -Dmaven.repo.local="$repo" -f "$a-$v/pom.xml" install
}
mk d 1.0
mk d 2.0
mk c 1.0 d:2.0
mk b 1.0 c:1.0
mk e 1.0 d:1.0
mkdir A
cat > A/pom.xml <<'XML'
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example</groupId><artifactId>a</artifactId><version>1.0</version>
  <dependencies>
    <dependency><groupId>com.example</groupId><artifactId>b</artifactId><version>1.0</version></dependency>
    <dependency><groupId>com.example</groupId><artifactId>e</artifactId><version>1.0</version></dependency>
  </dependencies>
</project>
XML
tree() {
  mvn -q -B -Dmaven.repo.local="$repo" -f A/pom.xml \
    org.apache.maven.plugins:maven-dependency-plugin:3.8.1:tree -Dverbose -DoutputFile="$d/tree.txt"
  cat "$d/tree.txt"
}
echo "--- mvn dependency:tree -Dverbose"
tree
echo "--- with D 2.0 in the dependencyManagement of A"
sed -i 's|  <dependencies>|  <dependencyManagement><dependencies>\n    <dependency><groupId>com.example</groupId><artifactId>d</artifactId><version>2.0</version></dependency>\n  </dependencies></dependencyManagement>\n  <dependencies>|' A/pom.xml
grep -A2 dependencyManagement A/pom.xml | head -3
tree
cd / && rm -rf "${d:?}"
```

The output of that run:

```text
Apache Maven 3.8.7
--- mvn dependency:tree -Dverbose
com.example:a:jar:1.0
+- com.example:b:jar:1.0:compile
|  \- com.example:c:jar:1.0:compile
|     \- (com.example:d:jar:2.0:compile - omitted for conflict with 1.0)
\- com.example:e:jar:1.0:compile
   \- com.example:d:jar:1.0:compile
--- with D 2.0 in the dependencyManagement of A
  <dependencyManagement><dependencies>
    <dependency><groupId>com.example</groupId><artifactId>d</artifactId><version>2.0</version></dependency>
  </dependencies></dependencyManagement>
com.example:a:jar:1.0
+- com.example:b:jar:1.0:compile
|  \- com.example:c:jar:1.0:compile
|     \- (com.example:d:jar:2.0:compile - version managed from 2.0; omitted for duplicate)
\- com.example:e:jar:1.0:compile
   \- com.example:d:jar:2.0:compile (version managed from 1.0)
```

The first tree shows the nearest definition rule: D 2.0 under C is "omitted for conflict with 1.0", and A gets D 1.0 through E. C asks for D 2.0, but the build gives it D 1.0. In the second tree, `dependencyManagement` sets D 2.0 everywhere, and the tree says "version managed from 1.0".

**Check the tree in a build.** `mvn dependency:tree` shows the tree, and Gradle shows it with the task `dependencies`, for example `./gradlew :app:dependencies`.[^7][^2] The Maven Enforcer rule `dependencyConvergence` fails the build if two dependencies of the project need different versions of one artifact.[^8] So a team sees each conflict and sets the version by hand.

**Lock the versions in Gradle.** Gradle can save the resolved versions to a lock file, so that later builds use the same versions.[^9] The command `./gradlew dependencies --write-locks` writes or updates the lock state.[^9] The page [[lock-files]] describes lock files in general.

**Publish to Maven Central.** Maven Central has requirements for the components that it accepts. For example, each JAR file must have a sources JAR and a Javadoc JAR, and all the files must have `.md5` and `.sha1` checksum files.[^10]

## Criticism and limits

The nearest definition rule depends on the shape of the tree, not on the versions. A change deep in the tree can change which version wins, and a project can get a lower version than a library needs, as in the example above.[^4] The highest version rule of Gradle avoids that downgrade, but then a library can get a higher version than the one that it asks for.[^5]

`dependencyManagement` and a BOM fix the versions, but they are one more file to keep up to date. The page [[dependency-updates]] describes how to update dependencies in small steps.

Gradle can lock the versions, and the Gradle manual names dynamic versions as one reason: a team can use dynamic versions during development and lock them for releases.[^9] In Maven, a soft requirement such as `1.0` is one version, not a range, but a version that is nearer in the tree can replace it.[^1]

## References
