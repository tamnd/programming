```csharp run id=unixfilemode-and-readonly
using System;
using System.IO;

File.WriteAllText("data.txt", "x");

// UnixFileMode is a set of flags with the octal values of Unix.
var mode = UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.GroupRead;
File.SetUnixFileMode("data.txt", mode);
Console.WriteLine($"mode: {Convert.ToString((int)File.GetUnixFileMode("data.txt"), 8)}");

// The read-only attribute removes the write bits on Unix.
var info = new FileInfo("data.txt");
info.IsReadOnly = true;
info.Refresh();
Console.WriteLine($"read-only: {info.IsReadOnly}, mode {Convert.ToString((int)info.UnixFileMode, 8)}");
info.IsReadOnly = false;
info.Refresh();
Console.WriteLine($"writable:  {!info.IsReadOnly}, mode {Convert.ToString((int)info.UnixFileMode, 8)}");

// A new file can get its mode at creation, through FileStreamOptions.
var options = new FileStreamOptions {
    Mode = FileMode.CreateNew, Access = FileAccess.Write,
    UnixCreateMode = UnixFileMode.UserRead | UnixFileMode.UserWrite,
};
using (new FileStream("secret.txt", options)) { }
Console.WriteLine($"secret.txt: {File.GetUnixFileMode("secret.txt")}");
```

```output for=unixfilemode-and-readonly
mode: 640
read-only: True, mode 440
writable:  True, mode 640
secret.txt: UserWrite, UserRead
```

- Verdict: stdlib. `UnixFileMode` is an enum of flags whose values are the Unix bits, and `File.SetUnixFileMode` and `File.GetUnixFileMode` work on Unix only. `FileInfo.IsReadOnly` uses the read-only attribute of Windows, and on Unix, .NET maps it to the write bits. `FileStreamOptions.UnixCreateMode` gives the mode of a new file, and the umask of the process still removes bits from it.
