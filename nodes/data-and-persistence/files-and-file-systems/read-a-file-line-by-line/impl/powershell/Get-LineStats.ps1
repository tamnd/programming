# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-file-line-by-line/.

function Get-LineStats {
    <#
    .SYNOPSIS
    Reads a UTF-8 text file line by line.
    .OUTPUTS
    An object with Lines, the number of lines, and Longest, the code points in the longest line.
    Throws if the file does not exist or the bytes are not valid UTF-8.
    #>
    [CmdletBinding()]
    param([Parameter(Mandatory)][string] $Path)

    # .NET resolves a relative path from the process directory, not from $PWD.
    $full = $PSCmdlet.GetUnresolvedProviderPathFromPSPath($Path)
    # Get-Content puts U+FFFD in place of a bad byte sequence. This decoder throws.
    $strict = [System.Text.UTF8Encoding]::new($false, $true)
    $lines = 0L
    $longest = 0L
    try {
        # ReadLines opens the file now, but reads it only as foreach asks for lines.
        $reader = [System.IO.File]::ReadLines($full, $strict)
    } catch [System.Management.Automation.MethodInvocationException] {
        # Throw the FileNotFoundException itself, not the wrapper of the method call.
        throw $_.Exception.InnerException
    }
    foreach ($line in $reader) {
        $lines++
        $length = [System.Linq.Enumerable]::Count($line.EnumerateRunes())
        if ($length -gt $longest) { $longest = $length }
    }
    [pscustomobject]@{ Lines = $lines; Longest = $longest }
}
