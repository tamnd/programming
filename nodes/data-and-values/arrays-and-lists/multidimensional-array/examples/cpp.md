```cpp run id=array-vector-and-matrix-class
#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

// A small dense matrix: one vector and a row-major index formula.
class Matrix {
    std::size_t rows_, cols_;
    std::vector<double> data_;

public:
    Matrix(std::size_t r, std::size_t c) : rows_(r), cols_(c), data_(r * c) {}
    double& operator()(std::size_t r, std::size_t c) { return data_[r * cols_ + c]; }
    const std::vector<double>& data() const { return data_; }
};

int main() {
    // std::array of std::array is one block, like a C array.
    std::array<std::array<int, 3>, 2> a{{{1, 2, 3}, {4, 5, 6}}};
    std::cout << sizeof a << ' ' << a[1][2] << ' ' << (&a[1][0] - &a[0][0]) << '\n';

    // vector<vector<int>> has a separate heap block per row.
    std::vector<std::vector<int>> grid(2, std::vector<int>(3, 0));
    grid[0][1] = 5;
    grid[1].push_back(4);
    std::cout << grid[0].size() << ' ' << grid[1].size() << ' ' << grid[0][1] << '\n';

    Matrix m(2, 3);
    for (std::size_t r = 0; r < 2; r++)
        for (std::size_t c = 0; c < 3; c++) m(r, c) = 10.0 * r + c;
    for (double x : m.data()) std::cout << x << ' ';
    std::cout << m(1, 2) << '\n';
}
```

```output
24 6 3
3 4 5
0 1 2 10 11 12 12
```

- Verdict: stdlib. std::array of std::array is one block of 24 bytes, like a C array. vector<vector<int>> has a separate block per row, so one row can grow. A small Matrix class keeps one vector and computes r * cols + c.
