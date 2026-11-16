// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

#ifndef CLAMP_HPP
#define CLAMP_HPP

// Returns value limited to the range from low to high. The name is in a
// namespace of its own, apart from std::clamp.
namespace limits {

int clamp(int value, int low, int high);

}  // namespace limits

#endif
