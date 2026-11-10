// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

// Package run runs a program with an argument list and no shell.
package run

import (
	"errors"
	"fmt"
	"os/exec"
	"strings"
)

// Run runs argv and returns the exit status and the text of the two output
// streams. A missing program gives an error that wraps exec.ErrNotFound. A
// status that is not 0 is a result, not an error.
func Run(argv []string) (int, string, string, error) {
	cmd := exec.Command(argv[0], argv[1:]...)
	// A nil Stdin reads from os.DevNull. Two writers that are not files make
	// the package copy each pipe on its own goroutine, so no pipe fills.
	var stdout, stderr strings.Builder
	cmd.Stdout = &stdout
	cmd.Stderr = &stderr
	err := cmd.Run()
	var exitErr *exec.ExitError
	if err != nil && !errors.As(err, &exitErr) {
		return 0, "", "", err
	}
	status := cmd.ProcessState.ExitCode()
	if status == -1 {
		return 0, "", "", fmt.Errorf("%s: %s", argv[0], cmd.ProcessState)
	}
	return status, stdout.String(), stderr.String(), nil
}
