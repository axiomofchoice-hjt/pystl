#include <pybind11/attr.h>
#include <pybind11/detail/common.h>
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
    explicit TreeDict(std::map<py::object, py::object> dict) : map(dict) {}
    explicit TreeDict(py::iterable iterable) {
        for (auto it = iterable.begin(); it != iterable.end(); ++it) {
            py::iterable item = it->cast<py::iterable>();
            std::vector<py::object> list;
            for (auto i = item.begin(); i != item.end(); ++i) {
                list.push_back(i->cast<py::object>());
            }
            if (list.size() != 2) {
                throw py::value_error();
            }
            __setitem__(list[0], list[1]);
        }
    }
    py::object get(py::object key, py::object default_value) {
        if (map.find(key) != map.end()) {
            return map.at(key);
        } else {
            return default_value;
        }
    }
    py::object __getitem__(py::object key) {
        if (map.find(key) != map.end()) {
            return map.at(key);
        } else {
            throw py::key_error();
        }
    }
    void __setitem__(py::object key, py::object value) { map[key] = value; }
    bool __contains__(py::object key) { return map.contains(key); }
    void __delitem__(py::object key) { map.erase(key); }
    int64_t __len__() { return map.size(); }

    std::string __str__() {
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

    std::string __repr__() {
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
    m.doc() = "pystl";

    py::class_<TreeDict::iter_t>(m, "TreeDictKeyIterator")
        .def("__next__", &TreeDict::iter_t::__next__, py::is_operator())
        .def("__iter__", &TreeDict::iter_t::__iter__, py::is_operator());
    py::class_<TreeDict::reverse_iter_t>(m, "TreeDictReverseKeyIterator")
        .def("__next__", &TreeDict::reverse_iter_t::__next__, py::is_operator())
        .def("__iter__", &TreeDict::reverse_iter_t::__iter__,
             py::is_operator());
    py::class_<TreeDict>(m, "TreeDict")
        .def(py::init<>())
        .def(py::init<std::map<py::object, py::object>>(), py::arg("mapping"))
        .def(py::init<py::iterable>(), py::arg("iterable"))
        .def("get", &TreeDict::get, py::arg("key"),
             py::arg("default") = py::none())
        .def("__getitem__", &TreeDict::__getitem__, py::arg("key"),
             py::is_operator())
        .def("__setitem__", &TreeDict::__setitem__, py::arg("key"),
             py::arg("value"), py::is_operator())
        .def("__contains__", &TreeDict::__contains__, py::is_operator())
        .def("__delitem__", &TreeDict::__delitem__, py::is_operator())
        .def("__len__", &TreeDict::__len__, py::is_operator())
        .def("__iter__", &TreeDict::__iter__, py::is_operator())
        .def("__reversed__", &TreeDict::__reversed__, py::is_operator())
        .def("__str__", &TreeDict::__str__, py::is_operator())
        .def("__repr__", &TreeDict::__repr__, py::is_operator())
        // .def("test", &TreeDict::test)
        ;
}
