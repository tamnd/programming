---
tag: Y57YQF
kind: task
slug: publish-a-package
name: Publish a package
aliases: [publish a library, release a package, upload a package, go publish, cargo publish, npm publish, twine upload, dotnet nuget push, mvn deploy]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  Publish a small library to the registry of each language, with its metadata and a version. The scripts do every step up to the upload, or upload to a local directory.
see_also: [package-registries, package-a-program-for-release, add-a-dependency, version-constraints, dependency-updates, use-a-third-party-package]
sources:
  - url: https://go.dev/doc/modules/publishing
    title: Go, Publishing a module
    locator: Publishing steps
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/publishing.html
    title: The Cargo Book, Publishing on crates.io
    locator: Before publishing a new crate; Packaging a crate; cargo yank
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/commands/npm-publish
    title: npm Docs (v11), npm publish
    locator: Description; Files included in package; Configuration (dry-run)
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/tutorials/packaging-projects/
    title: Python Packaging User Guide, Packaging Python Projects
    locator: Configuring metadata; Generating distribution archives; Uploading the distribution archives
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-pack
    title: Microsoft Learn, dotnet pack
    locator: Description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/nuget-org/publish-a-package
    title: Microsoft Learn, Publish NuGet packages
    locator: Upload to the nuget.org web portal; Create an API key; Use the dotnet CLI
    accessed: 2026-11-16
  - url: https://maven.apache.org/plugins/maven-deploy-plugin/
    title: Apache Maven Deploy Plugin, Introduction
    locator: the introduction; Goals Overview
    accessed: 2026-11-16
  - url: https://central.sonatype.org/publish/requirements/
    title: Sonatype Central, Requirements
    locator: Supply Javadoc and Sources; Provide File Checksums; Sign Files with GPG/PGP; Correct Coordinates
    accessed: 2026-11-16
  - url: https://docs.pypi.org/trusted-publishers/
    title: PyPI Docs, Publishing to PyPI with a Trusted Publisher
    locator: Quick background
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/generating-provenance-statements
    title: npm Docs, Generating provenance statements
    locator: About npm provenance; Provenance limitations
    accessed: 2026-11-16
---

## Task

Publish a small library, so that other projects can add it as a dependency. The library is `greet`, with one function `hello(name)` that returns `Hello, <name>!`. Each solution must:

- Give the package a name, a version, a description and a license, in the manifest of the language.
- Run the checks of the tool before the upload: the tests, the build, and the list of the files that go into the package.
- Make the package file, or the tag, that the registry gets.
- Show the command that uploads to the public registry.

An upload to a public registry needs an account and a secret, and a published version is permanent. So the scripts on this site stop before the upload, or they upload to a local directory. See [[package-registries]] for the registries, and [[add-a-dependency]] for the other side.

## Solutions

| Language | Registry | Metadata | Check before the upload | Upload |
| --- | --- | --- | --- | --- |
| Go | the Go module proxy | `go.mod` and the tag | `go mod tidy`, `go test ./...` | `git push origin v0.1.0` |
| Rust | crates.io | `Cargo.toml` | `cargo package`, or `cargo publish --dry-run` | `cargo publish` |
| JavaScript and TypeScript | the npm registry | `package.json` | `npm publish --dry-run` | `npm publish` |
| Python | PyPI | `pyproject.toml` | `python3 -m build` | `python3 -m twine upload dist/*` |
| C# | nuget.org | the `.csproj` file | `dotnet pack` | `dotnet nuget push` |
| Java and Kotlin | Maven Central | `pom.xml` | `mvn verify` | `mvn deploy` |

**Go.** Go has no upload. A version of a module is a tag in its version control repository. Before the tag, run `go mod tidy`, which "removes any dependencies the module might have accumulated that are no longer necessary", and `go test ./...`.[^1] Then tag the commit, push the tag, and ask the proxy for the version with `GOPROXY=proxy.golang.org go list -m example.com/mymodule@v0.1.0`, so that the proxy adds the module to its index.[^1] The script stops before the push.

```bash run timeout=30 id=publish-go
d=$(mktemp -d)
cd "$d"
git init --quiet greet
cd greet
go mod init example.com/greet 2>/dev/null
cat >greet.go <<'END'
// Package greet greets people by name.
package greet

// Hello returns a greeting for name.
func Hello(name string) string {
	return "Hello, " + name + "!"
}
END
cat >greet_test.go <<'END'
package greet

import "testing"

func TestHello(t *testing.T) {
	if got := Hello("Go"); got != "Hello, Go!" {
		t.Errorf("Hello(%q) = %q", "Go", got)
	}
}
END

# The steps before the publish: tidy, test, commit, and tag the version.
go mod tidy
go test -count=1 ./... | sed 's/\t[0-9.]*s$//'
git add .
git -c user.name=ci -c user.email=ci commit --quiet -m "greet: changes for v0.1.0"
git tag v0.1.0
git tag --list
# The publish is git push origin v0.1.0, then a request to the proxy:
# GOPROXY=proxy.golang.org go list -m example.com/greet@v0.1.0
cd /
rm -rf "${d:?}"
```

```output for=publish-go
ok  	example.com/greet
v0.1.0
```

**Rust.** Before you publish, fill in the fields `license` or `license-file`, `description`, `homepage`, `repository` and `readme` of `Cargo.toml`.[^2] `cargo publish` checks the package, compresses the source into a `.crate` file, extracts that file into a temporary directory and builds it, and then uploads it.[^2] The documentation recommends that "you first run cargo publish --dry-run (or cargo package which is equivalent)", which does the first three steps; `cargo package --list` shows the files.[^2] Cargo ignores the files that the version control system ignores.[^2] The script commits the files first, and it uses `cargo package --offline`, because `cargo publish --dry-run` needs a network connection on the build machine of this site.

```bash run timeout=30 id=publish-rust
d=$(mktemp -d)
cd "$d"
cargo new --quiet --lib greet
cd greet
cat >src/lib.rs <<'END'
//! A small library that greets people.

/// Returns a greeting for `name`.
pub fn hello(name: &str) -> String {
    format!("Hello, {name}!")
}
END
# The manifest with the metadata that crates.io asks for.
cat >Cargo.toml <<'END'
[package]
name = "greet"
version = "0.1.0"
edition = "2024"
description = "Greets people by name"
license = "MIT OR Apache-2.0"
repository = "https://example.com/greet"
END
git add . >/dev/null
git -c user.name=ci -c user.email=ci commit --quiet -m "greet 0.1.0"

# List the files of the package. Then make the file greet-0.1.0.crate, unpack it and build it, as cargo publish does before the upload.
cargo package --list --offline
cargo package --offline 2>&1 | grep -E '^ +(Packaging|Verifying|Compiling)' | sed "s|$d|DIR|"
ls target/package/*.crate
cd /
rm -rf "${d:?}"
```

```output for=publish-rust
.cargo_vcs_info.json
.gitignore
Cargo.lock
Cargo.toml
Cargo.toml.orig
src/lib.rs
   Packaging greet v0.1.0 (DIR/greet)
   Verifying greet v0.1.0 (DIR/greet)
   Compiling greet v0.1.0 (DIR/greet/target/package/greet-0.1.0)
target/package/greet-0.1.0.crate
```

**JavaScript and TypeScript.** `npm publish` "Publishes a package to the registry so that it can be installed by name."[^3] With a list `files` in `package.json`, only those files go into the package, together with files such as `package.json`, `README.md` and `LICENSE`, which npm always includes.[^3] With `--dry-run`, npm "should only report what it would have done".[^3] In the output, the file `notes.txt` is not in the package, and npm warns that a real publish needs a login. A TypeScript package publishes the compiled JavaScript and the `.d.ts` files in the same way.

```bash run timeout=30 id=publish-javascript
d=$(mktemp -d)
cd "$d"
export npm_config_cache="$d/npm-cache"
mkdir greet
cd greet
cat >package.json <<'END'
{
  "name": "greet",
  "version": "1.0.0",
  "description": "Greets people by name",
  "license": "MIT",
  "type": "module",
  "exports": "./index.js",
  "files": ["index.js"]
}
END
cat >index.js <<'END'
export function hello(name) {
  return `Hello, ${name}!`;
}
END
echo 'Notes that must not go into the package.' >notes.txt

# Show what npm publish would upload, without an upload.
npm publish --dry-run 2>&1 | grep -E 'notice ([0-9]|name|version|filename|total files)|dry-run'
cd /
rm -rf "${d:?}"
```

```output for=publish-javascript
npm notice 60B index.js
npm notice 178B package.json
npm notice name: greet
npm notice version: 1.0.0
npm notice filename: greet-1.0.0.tgz
npm notice total files: 2
npm warn This command requires you to be logged in to https://registry.npmjs.org/ (dry-run)
```

**Python.** The metadata goes in the table `[project]` of `pyproject.toml`.[^4] `python3 -m build` makes two files in `dist`: "The tar.gz file is a source distribution whereas the .whl file is a built distribution."[^4] Twine uploads them; `--repository testpypi` uploads to TestPyPI, "a separate instance of the package index intended for testing and experimentation".[^4] The tutorial uses an API token, and Twine asks for it.[^4] The script is shown as text, because the build machine of this site has no `build` module and no network.

```text
mkdir -p greet/src/greet
cd greet
cat >pyproject.toml <<'END'
[build-system]
requires = ["hatchling >= 1.26"]
build-backend = "hatchling.build"

[project]
name = "greet"
version = "1.0.0"
description = "Greets people by name"
license = "MIT"
requires-python = ">=3.9"
END
cat >src/greet/__init__.py <<'END'
def hello(name):
    return f"Hello, {name}!"
END
python3 -m pip install --upgrade build twine
python3 -m build
ls dist
# greet-1.0.0-py3-none-any.whl
# greet-1.0.0.tar.gz
python3 -m twine upload --repository testpypi dist/*
```

**C#.** `dotnet pack` "builds the project and creates NuGet packages".[^5] The project file holds the metadata of the package; the script sets `PackageId`, `Version`, `Authors`, `Description` and `PackageLicenseExpression`. To push to nuget.org, you need an API key from your nuget.org account, and the command is `dotnet nuget push <package-file> --api-key <API-key> --source https://api.nuget.org/v3/index.json`.[^6] The script pushes to a local folder instead.

The C# script is shown as text and does not run when this site is built, because the .NET commands in it can take longer than the time limit of a run. The output is from one run on the build machine of this site.

```text
d=$(mktemp -d)
cd "$d"
export NUGET_PACKAGES="$d/packages" DOTNET_NOLOGO=1
mkdir greet
cat >greet/Greet.csproj <<'END'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <PackageId>Greet</PackageId>
    <Version>1.0.0</Version>
    <Authors>Example</Authors>
    <Description>Greets people by name</Description>
    <PackageLicenseExpression>MIT</PackageLicenseExpression>
  </PropertyGroup>
</Project>
END
cat >greet/Greeter.cs <<'END'
namespace Greet;

/// <summary>Greets people by name.</summary>
public static class Greeter
{
    /// <summary>Returns a greeting for <paramref name="name"/>.</summary>
    public static string Hello(string name) => $"Hello, {name}!";
}
END

# Make the package, then push it to a local folder feed. For nuget.org, the source is https://api.nuget.org/v3/index.json and an API key is needed.
dotnet pack greet --configuration Release --output out -v quiet >/dev/null
ls out
mkdir feed
dotnet nuget push out/Greet.1.0.0.nupkg --source "$d/feed" 2>&1 | sed "s|$d|DIR|"
find feed -type f | sort
cd /
rm -rf "${d:?}"
```

```text
Greet.1.0.0.nupkg
Pushing Greet.1.0.0.nupkg to 'DIR/feed'...
Your package was pushed.
feed/Greet.1.0.0.nupkg
```

**Java and Kotlin.** The deploy plugin is used "to add your artifact(s) to a remote repository for sharing with other developers and projects".[^7] It uploads the JAR, the POM, the metadata of the repository, and the MD5 and SHA1 hash files.[^7] The element `distributionManagement` of `pom.xml` gives the repository; here it is a local directory. Maven Central asks for more: a `-sources.jar` and a `-javadoc.jar` for each JAR, a GPG signature in an `.asc` file for each file, and the elements `name`, `description`, `url`, `licenses`, `developers` and `scm` in the POM.[^8] A version that ends in `-SNAPSHOT` cannot go to Maven Central.[^8]

The Java script is shown as text and does not run when this site is built, because Maven can take longer than the time limit of a run. The output is from one run on the build machine of this site.

```text
# M2_REPO selects the local repository; the default is ~/.m2/repository.
d=$(mktemp -d)
cd "$d"
mkdir -p greet/src/main/java/com/example/greet
cd greet
cat >src/main/java/com/example/greet/Greet.java <<'END'
package com.example.greet;

/** Greets people by name. */
public final class Greet {
    private Greet() {
    }

    /** Returns a greeting for the name. */
    public static String hello(String name) {
        return "Hello, " + name + "!";
    }
}
END
# The coordinates, the metadata, and the repository to deploy to. Here it is a local directory.
cat >pom.xml <<'END'
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example</groupId>
  <artifactId>greet</artifactId>
  <version>1.0.0</version>
  <name>greet</name>
  <description>Greets people by name</description>
  <licenses>
    <license>
      <name>MIT</name>
    </license>
  </licenses>
  <properties>
    <maven.compiler.release>25</maven.compiler.release>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>
  <distributionManagement>
    <repository>
      <id>local</id>
      <url>file://${project.basedir}/../repo</url>
    </repository>
  </distributionManagement>
  <build>
    <plugins>
      <plugin>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.14.0</version>
      </plugin>
      <plugin>
        <artifactId>maven-jar-plugin</artifactId>
        <version>3.4.2</version>
      </plugin>
      <plugin>
        <artifactId>maven-install-plugin</artifactId>
        <version>3.1.4</version>
      </plugin>
      <plugin>
        <artifactId>maven-deploy-plugin</artifactId>
        <version>3.1.4</version>
      </plugin>
    </plugins>
  </build>
</project>
END
m2=${M2_REPO:-$HOME/.m2/repository}
mvn -B -q -Dmaven.repo.local="$m2" deploy
cd ..
find repo -type f | sort
cd /
rm -rf "${d:?}"
```

```text
repo/com/example/greet/1.0.0/greet-1.0.0.jar
repo/com/example/greet/1.0.0/greet-1.0.0.jar.md5
repo/com/example/greet/1.0.0/greet-1.0.0.jar.sha1
repo/com/example/greet/1.0.0/greet-1.0.0.pom
repo/com/example/greet/1.0.0/greet-1.0.0.pom.md5
repo/com/example/greet/1.0.0/greet-1.0.0.pom.sha1
repo/com/example/greet/maven-metadata.xml
repo/com/example/greet/maven-metadata.xml.md5
repo/com/example/greet/maven-metadata.xml.sha1
```

## Edge cases

- **A version is permanent.** On crates.io, "The version can never be overwritten, and the code cannot be deleted."[^2] On npm, a name and version "can never be used again", even after `npm unpublish`.[^3] The Go documentation says: "Don’t change a tagged version of a module after publishing it."[^1] To fix a mistake, publish a new version.
- **A broken release.** `cargo yank` stops new lock files from using a version, but "A yank does not delete any code."[^2] npm has `npm deprecate`.[^3] See [[dependency-updates]].
- **The name is taken.** On crates.io, names are "allocated on a first-come-first-serve basis".[^2] On nuget.org, "If the package ID already exists on nuget.org, you get an error."[^6] Check the name on the registry before the first release.
- **A test registry.** PyPI has TestPyPI, and nuget.org has int.nugettest.org, where uploads "might not be preserved".[^4][^6]
- **Size.** "crates.io currently has a 10MB size limit on the .crate file."[^2] Check the list of files before the upload.

## Pitfalls

- **A leaked secret.** "Always keep your API key a secret."[^6] Do not store an API key in the repository. In CI, PyPI trusted publishing exchanges an OpenID Connect token for an API token that is "only valid for 15 minutes from time of creation".[^9] npm can publish with provenance from GitHub Actions and GitLab CI/CD; a provenance statement links the package to its source and build, but "it does not guarantee the package has no malicious code".[^10]
- **Files that should not be there.** A package can contain test data, secrets or build output by mistake. Use `npm pack --dry-run`, `cargo package --list` or a look at the `.nupkg` file before each release.[^3][^2]
- **A path or a local dependency.** A dependency on `../other` works only on your computer. Change it to a registry version before you publish.
- **No changelog and no tag.** The Cargo Book says that each version should include a changelog entry and "A git tag pointing to the published commit".[^2] See [[semantic-versioning]].

## References
