#include <pybind11/attr.h>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <iostream>
#include <map>
#include <tuple>
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
    void set(py::handle key, py::handle value) {
        map[key.cast<py::object>()] = value.cast<py::object>();
    }
    void set(py::iterable iterable) {
        py::list list(iterable);
        if (py::len(list) != 2) {
            throw py::value_error();
        }
        set(list[0], list[1]);
    }
    void batch_set(py::iterable list) {
        for (auto item : list) {
            set(item.cast<py::iterable>());
        }
    }
    void batch_set(py::dict dict) {
        for (auto item : dict) {
            set(item.first, item.second);
        }
    }

   public:
    using iter_t = Iterator<std::map<py::object, py::object>::iterator>;
    using reverse_iter_t =
        Iterator<std::map<py::object, py::object>::reverse_iterator>;
    iter_t __iter__() { return iter_t(map.begin(), map.end()); }
    reverse_iter_t __reversed__() {
        return reverse_iter_t(map.rbegin(), map.rend());
    }
    TreeDict() {}
    TreeDict(py::kwargs kwargs) { batch_set(kwargs); }
    explicit TreeDict(py::dict dict, py::kwargs kwargs) {
        batch_set(dict);
        batch_set(kwargs);
    }
    explicit TreeDict(py::iterable iterable, py::kwargs kwargs) {
        batch_set(iterable);
        batch_set(kwargs);
    }
    py::object get(py::object key, py::object default_value) {
        if (map.find(key) != map.end()) {
            return map.at(key);
        } else {
            return default_value;
        }
    }
    std::vector<std::pair<py::object, py::object>> items() {
        std::vector<std::pair<py::object, py::object>> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.push_back(*it);
        }
        return res;
    }
    std::vector<py::object> keys() {
        std::vector<py::object> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.push_back(it->first);
        }
        return res;
    }
    std::vector<py::object> values() {
        std::vector<py::object> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.push_back(it->second);
        }
        return res;
    }
    void update(py::kwargs kwargs) { batch_set(kwargs); }
    void update(py::dict dict, py::kwargs kwargs) {
        batch_set(dict);
        batch_set(kwargs);
    }
    void update(py::iterable iterable, py::kwargs kwargs) {
        batch_set(iterable);
        batch_set(kwargs);
    }
    void clear() { map.clear(); }
    py::object pop(py::object key) {
        auto it = map.find(key);
        if (it != map.end()) {
            auto res = it->second;
            map.erase(it);
            return res;
        } else {
            throw py::key_error();
        }
    }
    py::object pop(py::object key, py::object default_value) {
        auto it = map.find(key);
        if (it != map.end()) {
            auto res = it->second;
            map.erase(it);
            return res;
        } else {
            return default_value;
        }
    }
    std::pair<py::object, py::object> popitem() {
        if (map.empty()) {
            throw py::key_error();
        }
        auto it = map.begin();
        auto res = *it;
        map.erase(it);
        return res;
    }
    bool has_key(py::object key) { return map.contains(key); }
    TreeDict copy() {
        TreeDict res;
        res.map = map;
        return res;
    }
    static TreeDict fromkeys(py::iterable keys, py::object default_value) {
        TreeDict res;
        for (auto key : keys) {
            res.set(key, default_value);
        }
        return res;
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
    py::str __str__() {
        py::str res = "TreeDict([";
        for (auto it = map.begin(); it != map.end(); ++it) {
            res += py::str("({}, {})")
                       .format(py::repr(it->first), py::repr(it->second));
            if (std::next(it) != map.end()) {
                res += py::str(", ");
            }
        }
        res += py::str("])");
        return res;
    }

    py::str __repr__() { return __str__(); }
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
        .def(py::init<py::kwargs>())
        .def(py::init<py::dict, py::kwargs>(), py::arg("mapping"))
        .def(py::init<py::iterable, py::kwargs>(), py::arg("iterable"))
        .def("get", &TreeDict::get, py::arg("key"),
             py::arg("default") = py::none())
        .def("items", &TreeDict::items)
        .def("keys", &TreeDict::keys)
        .def("values", &TreeDict::values)
        .def("update", py::overload_cast<py::kwargs>(&TreeDict::update))
        .def("update",
             py::overload_cast<py::dict, py::kwargs>(&TreeDict::update),
             py::arg("mapping"))
        .def("update",
             py::overload_cast<py::iterable, py::kwargs>(&TreeDict::update),
             py::arg("iterable"))
        .def("clear", &TreeDict::clear)
        .def("pop", py::overload_cast<py::object>(&TreeDict::pop),
             py::arg("key"))
        .def("pop", py::overload_cast<py::object, py::object>(&TreeDict::pop),
             py::arg("key"), py::arg("default"))
        .def("popitem", &TreeDict::popitem)
        .def("has_key", &TreeDict::has_key, py::arg("key"))
        .def("copy", &TreeDict::copy)
        .def_static("fromkeys", &TreeDict::fromkeys, py::arg("iterable"),
                    py::arg("value") = py::none())
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
