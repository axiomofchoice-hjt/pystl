#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace utils {
namespace py = pybind11;

template <typename Base, typename base_iter_t>
struct Iterator {
   public:
    using Iter = base_iter_t;
    Iter to;
    Iter end;

    Base __iter__() const { return Base{to, end}; }
    py::object __next__() {
        if (to == end) {
            throw py::stop_iteration();
        }
        auto res = static_cast<Base *>(this)->unwrap();
        ++to;
        return res;
    }
};
}  // namespace utils
