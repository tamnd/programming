# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-file-line-by-line/.

BeforeAll {
    . (Join-Path $PSScriptRoot 'Get-LineStats.ps1')
    $script:file = Join-Path $TestDrive 'input.txt'
}

Describe 'Get-LineStats' {
    It '<Name>' -ForEach @(
        @{ Name = 'lf'; Text = "alpha`nbeta`ngamma`n"; Lines = 3; Longest = 5 }
        @{ Name = 'blank-lines'; Text = "first`n`n`nlast line`n"; Lines = 4; Longest = 9 }
        @{ Name = 'empty'; Text = ''; Lines = 0; Longest = 0 }
        @{ Name = 'only-newline'; Text = "`n"; Lines = 1; Longest = 0 }
        @{ Name = 'no-final-newline'; Text = "one`ntwo`nthree"; Lines = 3; Longest = 5 }
        @{ Name = 'crlf'; Text = "one`r`ntwo`r`nthree`r`n"; Lines = 3; Longest = 5 }
        @{ Name = 'crlf-no-final-newline'; Text = "one`r`ntwo`r`nthree"; Lines = 3; Longest = 5 }
        @{ Name = 'mixed-endings'; Text = "one`r`ntwo`nthree`r`n"; Lines = 3; Longest = 5 }
        @{ Name = 'whitespace'; Text = "  two spaces`ttab `n"; Lines = 1; Longest = 17 }
        @{ Name = 'backslashes'; Text = "C:\temp\new`n"; Lines = 1; Longest = 11 }
        @{ Name = 'long-line'; Text = ('x' * 1MB) + "`nend`n"; Lines = 2; Longest = 1MB }
        @{ Name = 'non-ascii'; Text = "na`u{EF}ve caf`u{E9}`n`u{65E5}`u{672C}`u{8A9E}`u{306E}`u{30C6}`u{30AD}`u{30B9}`u{30C8}`n" + ("`u{1F642}" * 6) + "`n"; Lines = 3; Longest = 10 }
    ) {
        [System.IO.File]::WriteAllBytes($file, [System.Text.UTF8Encoding]::new($false).GetBytes($Text))
        $result = Get-LineStats -Path $file
        $result.Lines | Should -Be $Lines
        $result.Longest | Should -Be $Longest
    }

    It 'missing' {
        { Get-LineStats -Path (Join-Path $TestDrive 'missing.txt') } |
            Should -Throw -ExceptionType ([System.IO.FileNotFoundException])
    }

    It '<Name>' -ForEach @(
        @{ Name = 'invalid-byte'; Bytes = [byte[]](0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a) }
        @{ Name = 'truncated-at-end'; Bytes = [byte[]](0x6f, 0x6b, 0x0a, 0x63, 0x61, 0x66, 0xc3) }
        @{ Name = 'encoded-surrogate'; Bytes = [byte[]](0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a) }
        @{ Name = 'overlong'; Bytes = [byte[]](0xc0, 0xaf, 0x0a) }
    ) {
        [System.IO.File]::WriteAllBytes($file, $Bytes)
        { Get-LineStats -Path $file } | Should -Throw -ExceptionType ([System.Text.DecoderFallbackException])
    }

    It 'resolves a relative path from the current location' {
        [System.IO.File]::WriteAllBytes($file, [byte[]](0x61, 0x0a))
        Push-Location $TestDrive
        try { (Get-LineStats -Path 'input.txt').Lines | Should -Be 1 } finally { Pop-Location }
    }
}
