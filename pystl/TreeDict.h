#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace TreeDict {

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

struct TreeDict {
    using base_map_t = std::map<py::object, py::object>;
    base_map_t map;
    void set(py::handle key, py::handle value);
    void set(py::iterable iterable);
    void batch_set(py::iterable list);
    void batch_set(py::dict dict);
    struct KeyIter
        : Iterator<KeyIter, std::map<py::object, py::object>::iterator> {
        py::object unwrap();
    };
    struct RKeyIter
        : Iterator<RKeyIter,
                   std::map<py::object, py::object>::reverse_iterator> {
        py::object unwrap();
    };
    struct ValueIter
        : Iterator<ValueIter, std::map<py::object, py::object>::iterator> {
        py::object unwrap();
    };
    struct ItemIter
        : Iterator<ItemIter, std::map<py::object, py::object>::iterator> {
        using Iter = std::map<py::object, py::object>::iterator;
        py::object unwrap();
    };
    ItemIter iteritems();
    KeyIter iterkeys();
    ValueIter itervalues();
    KeyIter __iter__();
    RKeyIter __reversed__();
    TreeDict();
    TreeDict(py::kwargs kwargs);
    TreeDict(py::dict dict, py::kwargs kwargs);
    TreeDict(py::iterable iterable, py::kwargs kwargs);
    py::object get(py::object key, py::object default_value);
    std::vector<std::pair<py::object, py::object>> items();
    std::vector<py::object> keys();
    std::vector<py::object> values();
    void update(py::kwargs kwargs);
    void update(py::dict dict, py::kwargs kwargs);
    void update(py::iterable iterable, py::kwargs kwargs);
    void clear();
    py::object pop(py::object key);
    py::object pop(py::object key, py::object default_value);
    std::pair<py::object, py::object> popitem();
    bool has_key(py::object key);
    TreeDict copy();
    static TreeDict fromkeys(py::iterable keys, py::object default_value);
    py::object __getitem__(py::object key);
    void __setitem__(py::object key, py::object value);
    bool __contains__(py::object key);
    void __delitem__(py::object key);
    int64_t __len__();
    py::object lower_bound(py::object key);
    py::object upper_bound(py::object key);
    py::str __str__();
    py::str __repr__();
};

inline void bind(py::module &m) {
    py::class_<TreeDict::KeyIter>(m, "TreeDictKeyIterator")
        .def("__next__", &TreeDict::KeyIter::__next__, py::is_operator())
        .def("__iter__", &TreeDict::KeyIter::__iter__, py::is_operator());
    py::class_<TreeDict::RKeyIter>(m, "TreeDictReverseKeyIterator")
        .def("__next__", &TreeDict::RKeyIter::__next__, py::is_operator())
        .def("__iter__", &TreeDict::RKeyIter::__iter__, py::is_operator());
    py::class_<TreeDict::ValueIter>(m, "TreeDictValueIterator")
        .def("__next__", &TreeDict::ValueIter::__next__, py::is_operator())
        .def("__iter__", &TreeDict::ValueIter::__iter__, py::is_operator());
    py::class_<TreeDict::ItemIter>(m, "TreeDictItemIterator")
        .def("__next__", &TreeDict::ItemIter::__next__, py::is_operator())
        .def("__iter__", &TreeDict::ItemIter::__iter__, py::is_operator());
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
        .def("iteritems", &TreeDict::iteritems)
        .def("iterkeys", &TreeDict::iterkeys)
        .def("itervalues", &TreeDict::itervalues)
        .def("__iter__", &TreeDict::__iter__, py::is_operator())
        .def("__reversed__", &TreeDict::__reversed__, py::is_operator())
        .def("lower_bound", &TreeDict::lower_bound)
        .def("upper_bound", &TreeDict::upper_bound)
        .def("__str__", &TreeDict::__str__, py::is_operator())
        .def("__repr__", &TreeDict::__repr__, py::is_operator());
}
}  // namespace TreeDict
