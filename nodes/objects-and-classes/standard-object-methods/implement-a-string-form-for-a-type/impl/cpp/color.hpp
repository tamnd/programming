// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace color {

class Color {
  public:
    // Throws std::out_of_range when a value is not 0 to 255.
    Color(int r, int g, int b);

    // The display form, such as "#FF8000".
    std::string to_string() const;

    friend bool operator==(const Color&, const Color&) = default;

  private:
    std::uint8_t r_, g_, b_;
};

// Writes the display form, so that std::cout << c and std::ostringstream work.
std::ostream& operator<<(std::ostream& os, const Color& c);

// Returns the display form of each color; throws std::invalid_argument when the lengths differ.
std::vector<std::string> format_colors(const std::vector<int>& reds, const std::vector<int>& greens,
                                       const std::vector<int>& blues);

}  // namespace color

#endif
