#include <pybind11/attr.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <map>
#include <utility>
#include <vector>

namespace py = pybind11;

template <typename Iter>
class Iterator {
   private:
    Iter it;
    Iter end;

   public:
    Iterator(Iter it, Iter end) : it(it), end(end) {}
    Iterator<Iter> __iter__() const { return Iterator<Iter>(it, end); }
    py::object __next__() {
        if (it == end) {
            throw py::stop_iteration();
        }
        auto res = it->first;
        ++it;
        return res;
    }
};

class TreeDict {
   private:
    std::map<py::object, py::object> map;

   public:
    using iter_t = Iterator<std::map<py::object, py::object>::iterator>;
    using reverse_iter_t =
        Iterator<std::map<py::object, py::object>::reverse_iterator>;
    iter_t __iter__() { return iter_t(map.begin(), map.end()); }
    reverse_iter_t __reversed__() {
        return reverse_iter_t(map.rbegin(), map.rend());
    }
    TreeDict() {}
    explicit TreeDict(const std::map<py::object, py::object> &dict)
        : map(dict) {}
    explicit TreeDict(
        const std::vector<std::pair<py::object, py::object>> &list)
        : map(list.begin(), list.end()) {}
    py::object __getitem__(py::object key) const { return map.at(key); }
    void __setitem__(py::object key, py::object value) { map[key] = value; }
    bool __contains__(py::object key) const { return map.contains(key); }
    void __delitem__(py::object key) { map.erase(key); }
    int64_t __len__() const { return map.size(); }

    std::string __str__() const {
        std::string res = "TreeDict({";
        for (auto it = map.begin(); it != map.end(); ++it) {
            res += it->first.attr("__str__")().cast<std::string>();
            res += ": ";
            res += it->second.attr("__str__")().cast<std::string>();
            if (std::next(it) != map.end()) {
                res += ", ";
            }
        }
        res += "})";
        return res;
    }

    std::string __repr__() const {
        std::string res = "TreeDict({";
        for (auto it = map.begin(); it != map.end(); ++it) {
            res += it->first.attr("__repr__")().cast<std::string>();
            res += ": ";
            res += it->second.attr("__repr__")().cast<std::string>();
            if (std::next(it) != map.end()) {
                res += ", ";
            }
        }
        res += "})";
        return res;
    }

    // py::object test(py::dict dict) { return ; }
};

PYBIND11_MODULE(pystl, m) {
    m.doc() = "example_pb bindings";

    py::class_<TreeDict::iter_t>(m, "TreeDictKeyIterator")
        .def("__next__", &TreeDict::iter_t::__next__, py::is_operator())
        .def("__iter__", &TreeDict::iter_t::__iter__, py::is_operator());
    py::class_<TreeDict::reverse_iter_t>(m, "TreeDictReverseKeyIterator")
        .def("__next__", &TreeDict::reverse_iter_t::__next__, py::is_operator())
        .def("__iter__", &TreeDict::reverse_iter_t::__iter__,
             py::is_operator());
    py::class_<TreeDict>(m, "TreeDict")
        .def(py::init<>())
        .def(py::init<const std::map<py::object, py::object> &>())
        .def(py::init<const std::vector<std::pair<py::object, py::object>> &>())
        .def("__setitem__", &TreeDict::__setitem__, py::is_operator())
        .def("__contains__", &TreeDict::__contains__, py::is_operator())
        .def("__delitem__", &TreeDict::__delitem__, py::is_operator())
        .def("__getitem__", &TreeDict::__getitem__, py::is_operator())
        .def("__len__", &TreeDict::__len__, py::is_operator())
        .def("__iter__", &TreeDict::__iter__, py::is_operator())
        .def("__reversed__", &TreeDict::__reversed__, py::is_operator())
        .def("__str__", &TreeDict::__str__, py::is_operator())
        .def("__repr__", &TreeDict::__repr__, py::is_operator())
        // .def("test", &TreeDict::test)
        ;
}
