// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

// Package makesymlink makes a symbolic link and reads its target back.
package makesymlink

import "os"

// MakeSymlink makes a symbolic link at link that holds target, and returns
// the target that the link holds, read back with os.Readlink. target is
// stored as it is: it need not exist, and a relative target is read from the
// directory of link. os.Symlink takes the target first and the new link
// second, as ln -s does. When link exists, also as a dangling link, the error
// matches fs.ErrExist, and when the directory of link does not exist, it
// matches fs.ErrNotExist.
func MakeSymlink(target, link string) (string, error) {
	if err := os.Symlink(target, link); err != nil {
		return "", err
	}
	return os.Readlink(link)
}
