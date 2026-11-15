---
tag: VSNJTQ
kind: tool
slug: nuget-pm
name: NuGet
aliases: [NuGet, nuget, .nupkg, nupkg, nuget.org, PackageReference, packages.config, nuget.config, packages.lock.json, Directory.Packages.props, Central Package Management, dotnet package add]
area: tools-and-workflow/package-managers
level: core
summary: >-
  NuGet is the package manager of .NET. It restores the packages of a project from nuget.org or other feeds, and it picks the lowest version that each range allows.
role: [package-manager]
licence: apache-2.0
repo: https://github.com/NuGet/NuGet.Client
homepage: https://learn.microsoft.com/en-us/nuget/
history:
  year: 2010
  people: [Outercurve Foundation]
  note: The Outercurve Foundation first made NuGet under the name NuPack, and it came out in 2010.
see_also: [package-manager-concept, dependency-resolution, version-constraints, lock-files, transitive-dependencies, dotnet, msbuild, csharp]
sources:
  - url: https://learn.microsoft.com/en-us/nuget/what-is-nuget
    title: Microsoft Learn, An introduction to NuGet
    locator: the introduction; The flow of packages; Manage dependencies
  - url: https://github.com/NuGet/NuGet.Client/blob/dev/LICENSE.txt
    title: NuGet.Client LICENSE.txt
    locator: the whole file
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-package-add
    title: Microsoft Learn, dotnet package add
    locator: Name; Description; Implicit restore
  - url: https://learn.microsoft.com/en-us/nuget/reference/nuget-config-file
    title: Microsoft Learn, nuget.config reference
    locator: config section; packageSources
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/managing-the-global-packages-and-cache-folders
    title: Microsoft Learn, How to manage the global packages, cache, and temp folders
    locator: the table of folders
  - url: https://learn.microsoft.com/en-us/nuget/hosting-packages/local-feeds
    title: Microsoft Learn, Local feeds
    locator: the introduction
  - url: https://learn.microsoft.com/en-us/nuget/concepts/dependency-resolution
    title: Microsoft Learn, How NuGet resolves package dependencies
    locator: Dependency resolution rules
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-package-list
    title: Microsoft Learn, dotnet package list
    locator: Synopsis; Description
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/package-restore
    title: Microsoft Learn, Restore packages with NuGet Package Restore
    locator: the introduction
  - url: https://learn.microsoft.com/en-us/nuget/concepts/package-versioning
    title: Microsoft Learn, Package versioning
    locator: Version basics; Version ranges
  - url: https://learn.microsoft.com/en-us/nuget/create-packages/creating-a-package-dotnet-cli
    title: Microsoft Learn, Create a NuGet package using the dotnet CLI
    locator: Set properties
  - url: https://learn.microsoft.com/en-us/nuget/nuget-org/publish-a-package
    title: Microsoft Learn, Publish NuGet packages
    locator: Use the dotnet CLI
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/package-references-in-project-files
    title: Microsoft Learn, PackageReference in project files
    locator: Floating Versions; Locking dependencies
  - url: https://learn.microsoft.com/en-us/nuget/consume-packages/central-package-management
    title: Microsoft Learn, Central Package Management (CPM)
    locator: Enabling Central Package Management; Transitive Pinning
  - url: https://en.wikipedia.org/wiki/NuGet
    title: Wikipedia, NuGet
    locator: the introduction; the infobox
---

## What it is

NuGet is the package manager of .NET. Microsoft Learn says that "a NuGet package is a single ZIP file with the .nupkg extension". The file holds compiled code (DLLs), other files for that code, and a manifest with the version number and other data about the package.[^1] NuGet has tools for the people who make packages, for the servers that host them, and for the projects that use them.[^1]

The public host is nuget.org. Microsoft Learn says that NuGet keeps "the central repository of over 100,000 unique packages at nuget.org".[^1] A team can also host packages in the cloud, on a private network, or in a folder on one computer.[^1]

The NuGet client is in the .NET SDK, so the `dotnet` command adds, restores, makes and publishes packages. The page [[dotnet]] is about the `dotnet` command, and [[msbuild]] is about the build engine that reads the project files. This page is about NuGet as a package manager. The general ideas are on other pages: [[package-manager-concept]], [[dependency-resolution]], [[version-constraints]], [[lock-files]] and [[transitive-dependencies]].

NuGet is free software under the Apache License, Version 2.0.[^2]

## Install and first use

NuGet comes with the .NET SDK. `dotnet package add` adds a `<PackageReference>` element to a project file, or it changes the version of an element that is there. Then it runs `dotnet restore`.[^3] The .NET 10 SDK added this "noun first" form. With the .NET 9 SDK or earlier, the command is `dotnet add package`.[^3]

The script below makes a small feed in a folder. It packs two versions of a library, `Example.Greet` 1.0.0 and 1.1.0, and a second library, `Example.Polite` 1.0.0, which needs `Example.Greet` 1.0.0 or higher. Then it adds `Example.Polite` to a program and runs the program. The file `nuget.config` clears the other package sources, and `NUGET_PACKAGES` points to a temporary folder, so nothing comes from nuget.org.[^4][^5][^6]

The script is shown as text and does not run when this site is built, because a run takes longer than the time limit of a run block. The output after it is from one run on the build machine of this site, with .NET SDK 10.0.400. The `grep` and `sed` commands remove the temporary path and some lines of build progress.

```text
# Pack two versions of a library and a second library that depends on it
# into a folder feed, then add the second library to a program.
# The global packages folder is a temporary one, and nuget.config clears
# the other sources, so nothing comes from nuget.org.
d=$(mktemp -d)
cd "$d"
export NUGET_PACKAGES="$d/packages" DOTNET_NOLOGO=1
dotnet --version
dotnet nuget --version
mkdir feed Greet Polite App
cat > nuget.config <<'XML'
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <packageSources>
    <clear />
    <add key="local" value="feed" />
  </packageSources>
</configuration>
XML
cat > Greet/Greet.csproj <<'XML'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <PackageId>Example.Greet</PackageId>
    <Authors>Example</Authors>
    <Description>Says hello.</Description>
  </PropertyGroup>
</Project>
XML
cat > Greet/Greeter.cs <<'CS'
namespace Example.Greet;

public static class Greeter
{
    public static string Hello(string name) => $"Hello, {name}!";
}
CS
cat > Polite/Polite.csproj <<'XML'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <PackageId>Example.Polite</PackageId>
    <Version>1.0.0</Version>
    <Authors>Example</Authors>
    <Description>Says hello politely.</Description>
  </PropertyGroup>
  <ItemGroup>
    <PackageReference Include="Example.Greet" Version="1.0.0" />
  </ItemGroup>
</Project>
XML
cat > Polite/Polite.cs <<'CS'
namespace Example.Polite;

public static class Polite
{
    public static string Hello(string name) => Example.Greet.Greeter.Hello(name) + " Welcome.";
}
CS
cat > App/App.csproj <<'XML'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
  </PropertyGroup>
</Project>
XML
echo 'System.Console.WriteLine(Example.Polite.Polite.Hello("NuGet"));' > App/Program.cs
for v in 1.0.0 1.1.0; do
  dotnet pack Greet -c Release -o feed -p:Version=$v 2>&1 | grep -o "Successfully created package.*" | sed "s|$d/||"
done
dotnet pack Polite -c Release -o feed 2>&1 | grep -o "Successfully created package.*" | sed "s|$d/||"
echo "--- dotnet package add"
dotnet package add Example.Polite --version 1.0.0 --project App/App.csproj 2>&1 | grep -E "^info : PackageReference" | sed "s|$d/||"
echo "--- App/App.csproj"
cat App/App.csproj
echo "--- dotnet package list"
dotnet package list --include-transitive --project App/App.csproj | sed -e "s|$d/||" -e 's/ *$//' -e '/^$/d'
echo "--- dotnet run"
dotnet run --project App
echo "--- the global packages folder"
ls packages packages/example.greet
cd / && rm -rf "${d:?}"
```

The output of that run:

```text
10.0.400
7.9.0-rc.38015+14fbf8d5271c98133561eb55185fdb05b286f578.14fbf8d5271c98133561eb55185fdb05b286f578
Successfully created package 'feed/Example.Greet.1.0.0.nupkg'.
Successfully created package 'feed/Example.Greet.1.1.0.nupkg'.
Successfully created package 'feed/Example.Polite.1.0.0.nupkg'.
--- dotnet package add
info : PackageReference for package 'Example.Polite' version '1.0.0' added to file 'App/App.csproj'.
--- App/App.csproj
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
  </PropertyGroup>
  <ItemGroup>
    <PackageReference Include="Example.Polite" Version="1.0.0" />
  </ItemGroup>
</Project>
--- dotnet package list
  Determining projects to restore...
  All projects are up-to-date for restore.
Project 'App' has the following package references
   [net10.0]:
   Top-level Package      Requested   Resolved
   > Example.Polite       1.0.0       1.0.0
   Transitive Package      Resolved
   > Example.Greet         1.0.0
--- dotnet run
Hello, NuGet! Welcome.
--- the global packages folder
packages:
example.greet
example.polite

packages/example.greet:
1.0.0
```

The program asks only for `Example.Polite`. NuGet found `Example.Greet` as a transitive package. The feed has `Example.Greet` 1.0.0 and 1.1.0, and NuGet picked 1.0.0, because `Example.Polite` asks for 1.0.0 or higher and NuGet uses the lowest applicable version.[^7] The global packages folder has no copy of 1.1.0. `dotnet package list --include-transitive` shows the two kinds of reference.[^8]

## Concepts of the tool

**Two formats.** A project lists its packages in one of two formats.[^1]

- PackageReference (NuGet 4.0 and later) keeps the list of the top-level dependencies in the project file. NuGet writes the full graph, with the transitive dependencies, into `obj/project.assets.json`. .NET Core projects always use PackageReference.[^1]
- `packages.config` (NuGet 1.0 and later) is an XML file with a flat list of all the dependencies, also the dependencies of other packages. It keeps the packages in a `packages` folder.[^1]

**Restore.** The packages are not in source control. Package restore gets all the dependencies that the project file or `packages.config` lists.[^9] The commands `nuget restore`, `dotnet restore` and `msbuild -t:restore` do a restore, and `dotnet build` and `dotnet run` do a restore automatically.[^9]

**Versions.** A version has the form `Major.Minor.Patch[-Suffix]`. A suffix such as `-beta` or `-rc` marks a pre-release version.[^10] A dependency gives a version range in interval notation:[^10]

| Notation | Rule |
|---|---|
| `1.0` | x ≥ 1.0 |
| `[1.0,)` | x ≥ 1.0 |
| `(1.0,)` | x > 1.0 |
| `[1.0]` | x = 1.0 |
| `(,1.0]` | x ≤ 1.0 |
| `[1.0,2.0)` | 1.0 ≤ x < 2.0 |

A plain version such as `1.0` is a minimum, not an exact version.[^10] This is different from Cargo, where `1.0` means a compatible version (see [[cargo]]). The page [[version-constraints]] compares the rules of more tools.

**Resolution.** Microsoft Learn gives four rules: "lowest applicable version, floating versions, direct-dependency-wins, and cousin dependencies".[^7]

- Lowest applicable version: NuGet restores the lowest version that the range allows. If the range is `>= 2.1` and the feed has no 2.1, NuGet takes the next version, for example 2.2. If an exact version is not on the feed, the restore fails.[^7]
- Floating versions: a version with `*`, such as `6.0.*`, gets the highest version that matches. Only a project can use a floating version.[^7]
- Direct dependency wins: if the project depends directly on a package, that version is used, and the versions that other packages ask for are ignored. This lets an application override a version in the graph. It can also cause a downgrade.[^7]
- Cousin dependencies: if two parts of the graph ask for different versions, NuGet uses the lowest version that satisfies all the requirements.[^7]

The model below is not NuGet. It applies the notation and the first two rules to a feed with versions that are made up. It ran with Python 3.12.

```python run id=nuget-range
# A model of how NuGet reads a version in a PackageReference and picks a
# version from a feed. It is not NuGet, and it knows only stable versions.

def parse(v):
    parts = [int(x) for x in v.split(".")]
    return tuple(parts + [0] * (3 - len(parts)))

def show(v):
    return ".".join(map(str, v))

def allowed(spec):
    """Return a test for a version and the rule that picks one version."""
    if "*" in spec:  # a floating version such as 6.* or 6.1.*
        prefix = spec.rstrip("*").rstrip(".")
        fixed = parse(prefix)[: len(prefix.split("."))] if prefix else ()
        return (lambda v: v[: len(fixed)] == fixed), max
    if spec[0] not in "[(":  # a plain version means "this version or higher"
        spec = "[" + spec + ",)"
    low_in, high_in = spec[0] == "[", spec[-1] == "]"
    inner = spec[1:-1]
    if "," not in inner:  # [1.2] is an exact version
        return (lambda v: v == parse(inner)), min
    lo, hi = (x.strip() for x in inner.split(","))
    def test(v):
        if lo and (v < parse(lo) if low_in else v <= parse(lo)):
            return False
        if hi and (v > parse(hi) if high_in else v >= parse(hi)):
            return False
        return True
    return test, min

feed = [parse(v) for v in ["1.0.0", "1.2.0", "2.2.0", "2.3.1", "3.0.0", "6.0.4", "6.1.0", "6.1.7"]]
print("feed:", " ".join(show(v) for v in feed))
for spec in ["2.1", "[2.1]", "[1.0,3.0)", "(,2.3.1]", "6.*", "6.1.*", "*"]:
    test, pick = allowed(spec)
    ok = [v for v in feed if test(v)]
    print(f"{spec:>10} -> {show(pick(ok)) if ok else 'error: no version on the feed'}")
```

```output for=nuget-range
feed: 1.0.0 1.2.0 2.2.0 2.3.1 3.0.0 6.0.4 6.1.0 6.1.7
       2.1 -> 2.2.0
     [2.1] -> error: no version on the feed
 [1.0,3.0) -> 1.0.0
  (,2.3.1] -> 1.0.0
       6.* -> 6.1.7
     6.1.* -> 6.1.7
         * -> 6.1.7
```

**The global packages folder.** NuGet installs each package that it downloads into the global packages folder, in a subfolder for the identifier and the version of the package. PackageReference projects use the packages directly from this folder.[^5] The folder is `~/.nuget/packages` on Linux and macOS, and `%userprofile%\.nuget\packages` on Windows. The variable `NUGET_PACKAGES` changes the location.[^5] With this folder, NuGet usually does not download again a package that is already on the computer.[^5] NuGet also has an HTTP cache, which is `~/.local/share/NuGet/v3-cache` on Linux and macOS.[^5]

## Common tasks

| Task | Command |
|---|---|
| Add a package to a project | `dotnet package add Newtonsoft.Json`[^3] |
| Add one version of a package | `dotnet package add Newtonsoft.Json --version 13.0.3`[^3] |
| Restore the packages | `dotnet restore`[^9] |
| List the packages with the transitive ones | `dotnet package list --include-transitive`[^8] |
| List the packages that have a higher version | `dotnet package list --outdated`[^8] |
| Make a package from a project | `dotnet pack`[^11] |
| Publish a package to nuget.org | `dotnet nuget push <package-file> --api-key <API-key> --source https://api.nuget.org/v3/index.json`[^12] |

`dotnet pack` uses properties from the project file. `PackageId` must be unique on nuget.org. If the project does not set `Version`, the version is 1.0.0.[^11] From NuGet 7.6 (.NET SDK 10.0.300), the variable `NUGET_API_KEY` can give the API key to `dotnet nuget push`, so the key is not on the command line.[^12]

**Lock files.** NuGet 4.9 and later can write a lock file. Set the MSBuild property `RestorePackagesWithLockFile` to `true`, and restore writes `packages.lock.json` at the root of the project. The file lists all the package dependencies.[^13] With `dotnet restore --locked-mode`, restore gets the exact packages in the lock file, or it fails if the dependencies of the project changed. Microsoft Learn gives CI/CD as a use for this mode.[^13] A floating version is one reason for a lock file: without it, each restore can get a different version.[^13]

**Central Package Management.** With many projects in one repository, a file `Directory.Packages.props` at the root can keep all the versions. It sets `ManagePackageVersionsCentrally` to `true` and has one `<PackageVersion>` element for each package. The `<PackageReference>` elements in the projects then have no `Version` attribute.[^14]

```xml
<Project>
  <PropertyGroup>
    <ManagePackageVersionsCentrally>true</ManagePackageVersionsCentrally>
  </PropertyGroup>
  <ItemGroup>
    <PackageVersion Include="PackageA" Version="1.0.0" />
  </ItemGroup>
</Project>
```

With `CentralPackageTransitivePinningEnabled` set to `true`, a `<PackageVersion>` also sets the version of a transitive package. Transitive pinning cannot move a package to a lower version than its dependents ask for. That gives the error NU1109.[^14]

## Configuration

**nuget.config.** The file `nuget.config` holds the settings of NuGet, for example the package sources and the location of the global packages folder.[^4] The `<packageSources>` section lists the sources. When `<clear />` is in a section, NuGet ignores the values from other configuration files for that section.[^4] The run above uses `<clear />` so that the folder feed is the only source.

**Local feeds.** A local feed is a folder structure on a network or on one computer that holds packages. NuGet can use it as a package source like any other.[^6]

**Environment variables.** `NUGET_PACKAGES` overrides the global packages folder, and `NUGET_HTTP_CACHE_PATH` overrides the HTTP cache.[^5]

## Alternatives

NuGet has more than one client. Microsoft Learn names `nuget restore` (the NuGet CLI), `dotnet restore`, `msbuild -t:restore` and Visual Studio as ways to restore packages.[^9] Inside NuGet, a project can use PackageReference or the older `packages.config`.[^1] Other language package managers have the same role as NuGet: [[cargo]] for Rust, [[go-modules]] for Go, and [[maven]] and [[gradle]] for Java.

## History and versions

The Outercurve Foundation first made NuGet under the name NuPack.[^15] Wikipedia gives 5 October 2010 as the date of the first release.[^15] Microsoft and the .NET Foundation develop it now.[^15]

The two formats show the history of NuGet: `packages.config` is from NuGet 1.0, PackageReference is from NuGet 4.0, and lock files are from NuGet 4.9.[^1][^13] The .NET 10 SDK added the form `dotnet package add`.[^3]

The output on this page is from .NET SDK 10.0.400, with the NuGet version `7.9.0-rc` that `dotnet nuget --version` printed.

## References
