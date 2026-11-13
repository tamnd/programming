// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

// Package renamefile gives a file a new name with one atomic rename call.
package renamefile

import "os"

// RenameFile gives the file at src the name dst, and replaces a file at dst.
// os.Rename is one rename call, so it is atomic. Across file systems it gives
// a *os.LinkError with syscall.EXDEV and does not copy.
func RenameFile(src, dst string) error {
	return os.Rename(src, dst)
}
