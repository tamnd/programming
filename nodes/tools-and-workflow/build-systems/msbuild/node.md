---
tag: ZN3HSW
kind: tool
slug: msbuild
name: MSBuild
aliases: [Microsoft Build Engine, msbuild.exe, dotnet msbuild, .csproj, .vcxproj, Directory.Build.props]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  MSBuild is the build engine of .NET and Visual Studio. A project file in XML declares properties, items, tasks and targets, and MSBuild skips a target when its outputs are newer than its inputs.
role: [build]
licence: mit
repo: https://github.com/dotnet/msbuild
homepage: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild?view=visualstudio
history:
  year: 2003
  people: []
  note: MSBuild was first created in 2003 for .NET Framework 2.0 and Visual Studio 2005. Microsoft put its source on GitHub and gave it to the .NET Foundation on 18 March 2015.
see_also: [dotnet, make, build-system-concept, incremental-builds, build-graph, cmake, maven, build-a-project]
sources:
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild?view=visualstudio
    title: Microsoft Learn, Use the MSBuild XML schema to control builds
    locator: the introduction; Install MSBuild; Use MSBuild at a command prompt; Project file; Properties; Items; Tasks; Targets
  - url: https://en.wikipedia.org/wiki/MSBuild
    title: Wikipedia, MSBuild
    locator: the introduction; Overview; History
  - url: https://github.com/dotnet/msbuild/blob/main/LICENSE
    title: MSBuild licence
    locator: the whole file (MIT License)
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/incremental-builds?view=visualstudio
    title: Microsoft Learn, Incremental builds in MSBuild
    locator: the whole page
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-properties?view=visualstudio
    title: Microsoft Learn, Explore MSBuild name-value properties
    locator: Environment properties; Create properties during execution; Global properties
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-items?view=visualstudio
    title: Microsoft Learn, Use MSBuild items and item types
    locator: Use wildcards to specify items; Item metadata; Transform item types by using metadata
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-targets?view=visualstudio
    title: Microsoft Learn, Use MSBuild targets to create small build units
    locator: the introduction; Target build order
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-command-line-reference?view=visualstudio
    title: Microsoft Learn, MSBuild command-line reference
    locator: -target; -property; -maxCpuCount; -getProperty
  - url: https://learn.microsoft.com/en-us/dotnet/core/project-sdk/overview
    title: Microsoft Learn, .NET project SDK overview
    locator: Available SDKs; Project files; Preprocess the project file
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/customize-by-directory?view=visualstudio
    title: Microsoft Learn, Customize the build by folder
    locator: the introduction; Quick reference; Search scope
  - url: https://devblogs.microsoft.com/dotnet/msbuild-engine-is-now-open-source-on-github/
    title: Rich Lander, MSBuild Engine is now Open Source on GitHub, .NET Blog, 18 March 2015
    locator: the whole post
  - url: https://github.com/dotnet/msbuild/blob/main/README.md
    title: dotnet/msbuild README
    locator: the introduction
---

## What it is

MSBuild, the Microsoft Build Engine, is "a platform for building applications". It gives an XML schema for a project file, and the project file controls how the software is built.[^1] Visual Studio uses MSBuild to load and build projects: the project files of Visual Studio, such as `.csproj`, `.vbproj` and `.vcxproj`, contain MSBuild XML.[^1] But MSBuild does not depend on Visual Studio, so a build server can run the same build without the IDE.[^1]

On .NET, the command `dotnet build` of the [[dotnet]] CLI is "a thin wrapper over the .NET Core version of MSBuild.exe". A computer that has only the .NET SDK has MSBuild through the `dotnet` command.[^1] MSBuild also builds native C and C++ code.[^2]

Wikipedia compares MSBuild with [[make]]: the user says what goes in and what the result must be, and the tool decides what to do and in which order.[^2] The XML syntax of the project files is like the syntax of Apache Ant and NAnt.[^2] MSBuild is free software under the MIT License.[^3]

## Install and first use

On Windows, the Build Tools for Visual Studio install `MSBuild.exe` without Visual Studio. On Windows, macOS and Linux, the .NET SDK gives MSBuild through `dotnet build` and `dotnet msbuild`.[^1] The command line takes a project file and options. This command builds `MyProj.proj` with the property `Configuration` set to `Debug`:[^1]

```text
MSBuild.exe MyProj.proj -property:Configuration=Debug
```

MSBuild does not care about the extension of the project file, but the convention is an extension that ends in `proj`.[^1]

The run below uses a project file with no SDK, so that all of its parts are visible. The project copies each `.txt` file in `pages` to an `.html` file in an output directory, and it has the four parts of MSBuild: properties, items, tasks and targets. The script builds the project three times: first from nothing, then with no change, and then after a change to one page. MSBuild compares the time stamps of the files, so the script sets the time of the changed file five seconds later, to make the change visible in the same second. Last, `-getProperty` gives the value of a property for a `Release` build, without a build. The `grep` keeps only the lines about the copies and the targets. The run used the MSBuild of the .NET SDK 10.0.400 on the build machine of this site.

```bash run timeout=30 id=msbuild-project
# An MSBuild project file with no SDK: properties, items and targets.
d=$(mktemp -d)
cd "$d"
mkdir pages
echo "first page" > pages/a.txt
echo "second page" > pages/b.txt
cat > site.proj <<'EOF'
<Project DefaultTargets="Build">
  <PropertyGroup>
    <Configuration Condition="'$(Configuration)' == ''">Debug</Configuration>
    <OutDir>out/$(Configuration)/</OutDir>
  </PropertyGroup>

  <ItemGroup>
    <Page Include="pages/*.txt" />
  </ItemGroup>

  <Target Name="Prepare">
    <MakeDir Directories="$(OutDir)" />
  </Target>

  <Target Name="Build" DependsOnTargets="Prepare"
          Inputs="@(Page)" Outputs="@(Page->'$(OutDir)%(Filename).html')">
    <Message Importance="high" Text="copy %(Page.Identity) to $(OutDir)%(Page.Filename).html" />
    <Copy SourceFiles="@(Page)" DestinationFiles="@(Page->'$(OutDir)%(Filename).html')" />
  </Target>
</Project>
EOF
build() {
  dotnet msbuild site.proj -nologo -v:n "$@" 2>&1 |
    grep -E '(copy |Skipping target|partially)' | sed -E 's/^ +//'
}
echo "--- first build"
build
echo "--- second build"
build
echo "--- after a change to pages/b.txt"
echo "second page, changed" > pages/b.txt
touch -d "@$(( $(date +%s) + 5 ))" pages/b.txt
build
echo "--- the property OutDir with Configuration=Release"
dotnet msbuild site.proj -nologo -getProperty:OutDir -p:Configuration=Release
cd / && rm -rf "${d:?}"
```

```output for=msbuild-project
--- first build
copy pages/a.txt to out/Debug/a.html
copy pages/b.txt to out/Debug/b.html
--- second build
Skipping target "Build" because all output files are up-to-date with respect to the input files.
--- after a change to pages/b.txt
Building target "Build" partially, because some output files are out of date with respect to their input files.
copy pages/b.txt to out/Debug/b.html
--- the property OutDir with Configuration=Release
out/Release/
```

The second build did nothing, because each output file had a later time stamp than its input file. The third build copied only `b.txt`, the file that changed. Microsoft calls this a "partial incremental build".[^4]

## Concepts of the tool

**Properties.** A property is a name and a value. A project declares properties in a `PropertyGroup` element, and the code refers to them as `$(Name)`.[^1] A `Condition` attribute makes a declaration conditional. The project above uses the pattern of the MSBuild documentation: `Configuration` is `Debug` only if it has no value yet.[^1] Environment variables are also properties, such as `$(Path)`, but a property in the project with the same name wins.[^5] A property that is set on the command line with `-property` or `-p` is a global property, and it overrides the value in the project file.[^5] That is why `-p:Configuration=Release` changed `OutDir` in the run.

**Items.** Items are the inputs of the build, and most items are files. Each item has an item type, such as `Compile`, and the code refers to the list of items of a type as `@(Type)`.[^1] An item can use the wildcards `?`, `*` and `**`, as `pages/*.txt` does above.[^6] Each item has metadata, such as `%(Filename)`, the file name without the extension. A transform makes another list from the metadata: `@(CppFiles -> '%(Filename).obj')` turns a list of `.cpp` files into a list of `.obj` files.[^6] The project above uses the same kind of transform to name the output files.

In MSBuild, the names of elements and attributes are case-sensitive, but the names of properties, items and metadata are not.[^1]

**Tasks.** A task is a unit of executable code that does a build operation, such as compiling files or running a tool.[^1] MSBuild has common tasks, such as `Copy`, `MakeDir` and `Csc`, the C# compiler.[^1] A task is an element in a target, and its parameters are attributes.[^1] A user can write more tasks in managed code, as types that implement the interface `ITask`.[^1]

**Targets.** A target groups tasks in an order and is an entry point into the build.[^1][^7] A target can depend on other targets, and a target never runs twice in one build, even if a later target depends on it.[^7] In the project above, `Build` depends on `Prepare`, so `MakeDir` runs before `Copy`. The attribute `DefaultTargets` of the project gives the target that runs when the command names none, and `-target` runs other targets instead.[^8]

**Incremental builds.** A target can have an `Inputs` attribute and an `Outputs` attribute. MSBuild tries to find a one-to-one mapping between them, and then it compares the time stamp of each input with the time stamp of its output.[^4] An item is up to date if its output file is the same age as its input file or younger.[^4] If all the outputs are up to date, MSBuild skips the target. If only some are, it runs the target for the items that are out of date.[^4] A one-to-one mapping is possible only when `Outputs` is a transform of `Inputs`, as in the project above.[^4] MSBuild compares time stamps and not contents, so a file that is only touched counts as changed. The page [[incremental-builds]] compares this method with the methods of other build tools.

**Evaluation and execution.** MSBuild sets the properties that are outside targets in the evaluation phase of a build, and then runs the targets in the execution phase. In the execution phase, tasks and targets can make or change properties.[^5] The option `-getProperty` writes the value of a property after the evaluation, with no build.[^8]

**SDK-style projects.** A .NET project file starts with `<Project Sdk="Microsoft.NET.Sdk">`. When MSBuild evaluates such a project, it adds an import of `Sdk.props` at the top of the file and an import of `Sdk.targets` at the bottom.[^9] These imported files hold the standard build logic: `.props` files define properties, and `.targets` files define targets.[^1] So a short `.csproj` file gets all the targets of a .NET build. The command `dotnet msbuild -preprocess` shows the full project after the imports, without a build.[^9]

## Common tasks

| Task | Command |
|---|---|
| Build a .NET project or solution | `dotnet build`[^1] |
| Run MSBuild with all of its options through the .NET SDK | `dotnet msbuild`[^9] |
| Set a global property | `msbuild MyProj.proj -p:Configuration=Release`[^5] |
| Run given targets instead of the default targets | `msbuild -target:PrepareResources;Compile`[^8] |
| Build projects in parallel, with up to 3 processes | `msbuild -maxCpuCount:3`[^8] |
| Show the value of a property, with no build | `msbuild -getProperty:OutDir`[^8] |
| Show the project after all imports | `dotnet msbuild -preprocess:output.xml`[^9] |

Without `-maxCpuCount`, MSBuild uses one process. With `-maxCpuCount` and no number, it uses up to the number of processors of the computer.[^8]

## Configuration

**Project files.** Each project has its own file, such as `app.csproj`. Visual Studio keeps the settings that are changed in the IDE in this file.[^1]

**Directory.Build.props and Directory.Build.targets.** MSBuild looks for these files in the directory of a project and then in the directories above it, and it stops at the first one that it finds. `Directory.Build.props` is imported early and sets default properties for all the projects in the directory and below it. `Directory.Build.targets` is imported late and adds targets and late properties.[^10] A repository can put one `Directory.Build.props` file at its root to give all its projects the same settings.[^10]

**Directory.Build.rsp.** This file gives default command-line arguments to `msbuild.exe` and `dotnet build`.[^10]

## Alternatives

- [[make]]: the Unix build tool, with rules in a makefile instead of targets in XML.
- [[cmake]]: on Windows, CMake can write Visual Studio project files, which MSBuild then builds.
- [[maven]] and [[gradle]]: the build tools of Java projects.

## History and versions

Wikipedia says that MSBuild was first created in 2003, for .NET Framework 2.0 and Visual Studio 2005.[^2] MSBuild was a part of .NET Framework, and from Visual Studio 2013 it came with Visual Studio instead.[^2]

On 18 March 2015, Microsoft put the source code of MSBuild on GitHub and gave the project to the .NET Foundation.[^11] The announcement said that "MSBuild is the default build engine for Visual Studio and the .NET community on the Windows platform", and that support for Linux and Mac would come.[^11] The source is in the repository `dotnet/msbuild` on GitHub.[^12]

## References
