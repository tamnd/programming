---
tag: F9W5PX
kind: tool
slug: dotnet-build
name: dotnet build
aliases: [dotnet build, dotnet clean, build configuration, Debug and Release, target framework, TargetFrameworks, output path, bin and obj]
area: tools-and-workflow/build-systems
level: core
summary: >-
  dotnet build is the command of the .NET CLI that builds a project or a solution with MSBuild. It restores packages, compiles for each target framework and configuration, and writes to bin and obj.
role: [build]
licence: mit
repo: https://github.com/dotnet/sdk
homepage: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
see_also: [dotnet, msbuild, csharp, build-graph, incremental-builds, build-a-project, maven, gradle]
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
    title: Microsoft Learn, dotnet build command
    locator: Description; Implicit restore; MSBuild; Arguments; Options; Examples
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-command-line-reference?view=visualstudio
    title: Microsoft Learn, MSBuild command-line reference
    locator: -getProperty; -getTargetResult
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/common-msbuild-project-properties?view=visualstudio
    title: Microsoft Learn, Common MSBuild project properties
    locator: BaseIntermediateOutputPath; BaseOutputPath; Configuration; DefineConstants; IntermediateOutputPath; Optimize
  - url: https://learn.microsoft.com/en-us/dotnet/standard/frameworks
    title: Microsoft Learn, Target frameworks in SDK-style projects
    locator: How to specify a target framework; Preprocessor symbols
  - url: https://learn.microsoft.com/en-us/visualstudio/ide/understanding-build-configurations?view=visualstudio
    title: Microsoft Learn, Understand build configurations
    locator: the introduction (Debug and Release; solution and project configurations; output folders)
  - url: https://learn.microsoft.com/en-us/dotnet/core/project-sdk/msbuild-props
    title: Microsoft Learn, MSBuild reference for .NET SDK projects
    locator: TargetFramework; AppendTargetFrameworkToOutputPath
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-clean
    title: Microsoft Learn, dotnet clean command
    locator: Description
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build-server
    title: Microsoft Learn, dotnet build-server command
    locator: Synopsis; Commands; Options
  - url: https://learn.microsoft.com/en-us/visualstudio/msbuild/obtaining-build-logs-with-msbuild?view=visualstudio
    title: Microsoft Learn, Obtain build logs with MSBuild
    locator: Save a binary log
  - url: https://learn.microsoft.com/en-us/dotnet/core/sdk/artifacts-output
    title: Microsoft Learn, Artifacts output layout
    locator: the introduction; How to configure
---

## What it is

`dotnet build` is the command of the [[dotnet]] CLI that builds a project, a solution or a file-based app, and its dependencies, into a set of binaries.[^1] The binaries are the code of the project in Intermediate Language (IL), in a `.dll` file, and, depending on the project, an executable, the debug symbols in a `.pdb` file, a `.deps.json` file with the dependencies, a `.runtimeconfig.json` file with the shared runtime and its version, and the libraries that the project depends on.[^1] For an executable project that targets .NET Core 3.0 or later, the build copies the library dependencies to the output folder, so the documentation says that "the build output should be deployable" when the project has no logic that is only for publishing.[^1]

`dotnet build` uses [[msbuild]] to do the build, and it supports parallel and incremental builds.[^1] It also takes the options of MSBuild, such as `-p` to set a property.[^1] The page [[msbuild]] shows how the .NET SDK imports its targets into each project file. The page [[dotnet]] shows the CLI as a whole, and this page is about the build: the configurations, the target frameworks, the output paths and the incremental build.

## Install and first use

`dotnet build` comes with the .NET SDK. With no argument, MSBuild searches the current directory for a project file or a solution file, a `.sln` or `.slnx` file.[^1] A build needs the file `project.assets.json`, which lists the dependencies of the project. `dotnet restore` makes this file, and `dotnet build` runs a restore implicitly first. The option `--no-restore` stops the implicit restore.[^1]

The script below makes a console program with one source file. The source uses the preprocessor symbol `DEBUG`, so the program prints a different line in each configuration. The script builds the program in the `Debug` and in the `Release` configuration. In each build, the MSBuild options `-getProperty` and `-getTargetResult` make `dotnet build` write the values of two properties and the result of the target `Build` as JSON, after the build.[^2] A short Python program prints the three values from the JSON. Then the script runs the program that the build made. The run used the .NET SDK 10.0.400 on the build machine of this site.

```bash run timeout=30 id=dotnet-build-configs
# Build one program in the Debug and in the Release configuration.
# -getProperty and -getTargetResult make dotnet build write JSON.
d=$(mktemp -d)
cd "$d"
cat > app.csproj <<'XML'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
  </PropertyGroup>
</Project>
XML
cat > Program.cs <<'CS'
#if DEBUG
Console.WriteLine("This is a Debug build.");
#else
Console.WriteLine("This is a Release build.");
#endif
CS
for c in Debug Release; do
  echo "--- $c"
  dotnet build -c $c -getProperty:Optimize -getProperty:DefineConstants \
    -getTargetResult:Build --tl:off -nologo | python3 -c '
import json, sys
r = json.load(sys.stdin)
print("Optimize:", r["Properties"]["Optimize"])
print("DefineConstants:", r["Properties"]["DefineConstants"])
print("Build:", r["TargetResults"]["Build"]["Result"])'
  dotnet bin/$c/net10.0/app.dll
done
cd / && rm -rf "${d:?}"
```

```output for=dotnet-build-configs
--- Debug
Optimize: false
DefineConstants: TRACE;DEBUG;NET;NET10_0;NETCOREAPP;NET5_0_OR_GREATER;NET6_0_OR_GREATER;NET7_0_OR_GREATER;NET8_0_OR_GREATER;NET9_0_OR_GREATER;NET10_0_OR_GREATER;NETCOREAPP1_0_OR_GREATER;NETCOREAPP1_1_OR_GREATER;NETCOREAPP2_0_OR_GREATER;NETCOREAPP2_1_OR_GREATER;NETCOREAPP2_2_OR_GREATER;NETCOREAPP3_0_OR_GREATER;NETCOREAPP3_1_OR_GREATER
Build: Success
This is a Debug build.
--- Release
Optimize: true
DefineConstants: TRACE;RELEASE;NET;NET10_0;NETCOREAPP;NET5_0_OR_GREATER;NET6_0_OR_GREATER;NET7_0_OR_GREATER;NET8_0_OR_GREATER;NET9_0_OR_GREATER;NET10_0_OR_GREATER;NETCOREAPP1_0_OR_GREATER;NETCOREAPP1_1_OR_GREATER;NETCOREAPP2_0_OR_GREATER;NETCOREAPP2_1_OR_GREATER;NETCOREAPP2_2_OR_GREATER;NETCOREAPP3_0_OR_GREATER;NETCOREAPP3_1_OR_GREATER
Build: Success
This is a Release build.
```

The two configurations gave different values to the properties `Optimize` and `DefineConstants`. `Optimize` turns on the optimizations of the compiler, and `DefineConstants` gives the symbols for conditional compilation, as the switches `/optimize` and `/define` of the compiler do.[^3] In `Debug`, the symbol `DEBUG` was defined, so the compiler kept the first `Console.WriteLine` and dropped the second. The other symbols, such as `NET10_0` and `NET8_0_OR_GREATER`, come from the target framework `net10.0`.[^4]

## Concepts of the tool

**Configurations.** A build configuration is a named set of settings. `Debug` and `Release` are the two standard configurations, and a project or a solution can have more.[^5] The property `Configuration` gives the configuration that MSBuild builds, and the option `-c` of `dotnet build` sets it. The default for most projects is `Debug`.[^1][^3] The configuration and the platform also select the output folder: for example, a build of `Debug` for x86 can go to `bin/Debug/x86`.[^5]

**Target frameworks.** The element `TargetFramework` of a project file gives one target framework moniker (TFM), such as `net10.0`. The element `TargetFrameworks`, plural, gives a list, separated by semicolons, for a project that targets more than one framework.[^4] The SDK reads the TFM, which is an alias, and sets the full moniker properties from it.[^6] For each target framework, the build defines preprocessor symbols: the name of the TFM in uppercase, with an underscore for each dot, such as `NETSTANDARD2_0` for `netstandard2.0`, and from the .NET 5 SDK also symbols such as `NET8_0_OR_GREATER`.[^4] A multi-targeted project uses these symbols in `#if` blocks, and conditions on the property `TargetFramework` in the project file, to use a different API for each framework.[^4] The option `-f` builds only one of the frameworks of the project.[^1]

**Output paths.** The default output directory of `dotnet build` is `./bin/<configuration>/<framework>/`, and `-o` changes it.[^1] The intermediate files go under `obj`, the default value of `BaseIntermediateOutputPath`, in a folder for each configuration.[^3] The .NET SDK appends the target framework and, if there is one, the runtime identifier to the output path. With `AppendTargetFrameworkToOutputPath` set to `false`, the path has no TFM, but then the outputs of two frameworks can overwrite each other.[^6] `dotnet clean` removes the outputs of the last build from `bin` and from `obj`. It is an MSBuild target, and it removes only the outputs that the build made.[^7]

**Incremental build.** The build of a .NET project is a chain of MSBuild targets, and MSBuild skips a target when all its outputs are up to date with its inputs, as the page [[msbuild]] shows. One of the targets of the .NET SDK is `CoreCompile`. The script below builds a program twice with no change between the builds. With the verbosity `n`, normal, the log names the targets, and `grep` keeps only the lines about `CoreCompile` and the line with the output file.[^1]

```bash run timeout=30 id=dotnet-build-incremental
# Build a program twice, and show what MSBuild did with the target CoreCompile.
d=$(mktemp -d)
cd "$d"
cat > app.csproj <<'XML'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
  </PropertyGroup>
</Project>
XML
echo 'Console.WriteLine("Hello");' > Program.cs
for n in 1 2; do
  echo "--- build $n"
  dotnet build --tl:off -nologo -v n 2>&1 | grep -E 'CoreCompile|-> ' | sed -e "s|$d|.|g" -e 's/^ *//'
done
cd / && rm -rf "${d:?}"
```

```output for=dotnet-build-incremental
--- build 1
CoreCompile:
app -> ./bin/Debug/net10.0/app.dll
--- build 2
CoreCompile:
Skipping target "CoreCompile" because all output files are up-to-date with respect to the input files.
app -> ./bin/Debug/net10.0/app.dll
```

In the second build, MSBuild skipped `CoreCompile`, because no input of the target had changed. The option `--no-incremental` "Marks the build as unsafe for incremental build": it turns off incremental compilation and forces a clean rebuild.[^1] A build can also start persistent build servers, such as the MSBuild build server and the VB/C# compiler build server. `dotnet build-server shutdown` stops them, and the option `--disable-build-servers` of `dotnet build` makes a build ignore them, for example when a cache is possibly corrupted.[^8][^1]

**Project references.** A project can reference other projects of the same solution. `dotnet build` builds these dependencies first, and `--no-dependencies` builds only the given project and ignores its project-to-project (P2P) references.[^1]

## Common tasks

| Task | Command |
|---|---|
| Build the project or solution in the current directory | `dotnet build`[^1] |
| Build the `Release` configuration | `dotnet build --configuration Release`[^1] |
| Build for one target framework of a multi-targeted project | `dotnet build -f net10.0`[^1] |
| Build for a runtime, here Linux on x64 | `dotnet build --runtime linux-x64`[^1] |
| Set an MSBuild property for the build | `dotnet build -p:Version=1.2.3.4`[^1] |
| Run the target `Clean` | `dotnet build -t:Clean`[^1] |
| Remove the outputs of the last build | `dotnet clean`[^7] |
| Build again from nothing | `dotnet build --no-incremental`[^1] |
| Build without a restore | `dotnet build --no-restore`[^1] |
| Write a binary log of the build | `dotnet build -bl`[^1][^9] |

A binary log, made with `-bl` or `-binaryLogger`, keeps the messages of the build at the diagnostic level, the contents of the imported files, and the values of all properties and items. The documentation says that a large build can be measurably slower with it.[^9]

## Configuration

**Project properties.** The properties of the build go in a `PropertyGroup` in the project file. These elements select one target framework, or two:[^4]

```xml
<TargetFramework>net10.0</TargetFramework>
<TargetFrameworks>net8.0;net10.0</TargetFrameworks>
```

A property can also come from the command line with `-p`.[^1] The page [[msbuild]] explains why such a global property overrides the value in the project file.

**Output layout.** `BaseOutputPath` changes the base of the output path: MSBuild then uses `$(BaseOutputPath)\$(Configuration)\` as `OutputPath`.[^3] From .NET 8, the artifacts output layout puts the outputs of all the projects into one directory, `artifacts`, next to the `Directory.Build.props` file. The property `UseArtifactsOutput` set to `true` in that file turns it on.[^10] The folders below `artifacts` are the type of output, the project and a pivot from the configuration, the framework and the runtime, such as `artifacts/bin/MyApp/debug` or `artifacts/bin/MyApp/debug_net8.0` for a multi-targeted project.[^10]

## Alternatives

- [[msbuild]]: `dotnet build` is MSBuild with the defaults of the .NET SDK. `dotnet msbuild` gives all of MSBuild.
- [[maven]] and [[gradle]]: the build tools of Java, with a lifecycle or a task graph instead of MSBuild targets.

The page [[build-a-project]] builds the same kind of program with the build tool of each tier 1 language, and [[incremental-builds]] compares how build tools decide that a step is up to date.

## History and versions

The documentation of `dotnet build` gives some changes between SDK versions:

- An executable project that targets .NET Core 3.0 or later gets its library dependencies in the output folder.[^1]
- The option `--disable-build-servers` came with the .NET 7 SDK.[^1]
- The .NET 10 SDK added the argument `FILE`, the path of a file-based app, as in `dotnet build MyProject.cs`.[^1]
- The artifacts output layout came with .NET 8.[^10]

## References
