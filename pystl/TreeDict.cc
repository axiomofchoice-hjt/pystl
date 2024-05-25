#include "TreeDict.h"

namespace TreeDict {
void TreeDict::set(py::handle key, py::handle value) {
    map.insert_or_assign(key.cast<py::object>(), value.cast<py::object>());
}
void TreeDict::set(py::iterable iterable) {
    py::list list(iterable);
    if (py::len(list) != 2) {
        throw py::value_error();
    }
    set(list[0], list[1]);
}
void TreeDict::batch_set(py::iterable list) {
    for (auto item : list) {
        set(item.cast<py::iterable>());
    }
}
void TreeDict::batch_set(py::dict dict) {
    for (auto item : dict) {
        set(item.first, item.second);
    }
}
py::object TreeDict::KeyIter::unwrap() { return to->first; }
py::object TreeDict::RKeyIter::unwrap() { return to->first; }
py::object TreeDict::ValueIter::unwrap() { return to->second; }
py::object TreeDict::ItemIter::unwrap() {
    return py::make_tuple(to->first, to->second);
}

TreeDict::ItemIter TreeDict::iteritems() {
    return ItemIter{map.begin(), map.end()};
}
TreeDict::KeyIter TreeDict::iterkeys() {
    return KeyIter{map.begin(), map.end()};
}
TreeDict::ValueIter TreeDict::itervalues() {
    return ValueIter{map.begin(), map.end()};
}
TreeDict::KeyIter TreeDict::__iter__() { return iterkeys(); }
TreeDict::RKeyIter TreeDict::__reversed__() {
    return RKeyIter{map.rbegin(), map.rend()};
}
TreeDict::TreeDict() {}
TreeDict::TreeDict(py::kwargs kwargs) { batch_set(kwargs); }
TreeDict::TreeDict(py::dict dict, py::kwargs kwargs) {
    batch_set(dict);
    batch_set(kwargs);
}
TreeDict::TreeDict(py::iterable iterable, py::kwargs kwargs) {
    batch_set(iterable);
    batch_set(kwargs);
}
py::object TreeDict::get(py::object key, py::object default_value) {
    if (map.find(key) != map.end()) {
        return map.at(key);
    } else {
        return default_value;
    }
}
std::vector<std::pair<py::object, py::object>> TreeDict::items() {
    std::vector<std::pair<py::object, py::object>> res;
    for (auto it = map.begin(); it != map.end(); ++it) {
        res.push_back(*it);
    }
    return res;
}
std::vector<py::object> TreeDict::keys() {
    std::vector<py::object> res;
    for (auto it = map.begin(); it != map.end(); ++it) {
        res.push_back(it->first);
    }
    return res;
}
std::vector<py::object> TreeDict::values() {
    std::vector<py::object> res;
    for (auto it = map.begin(); it != map.end(); ++it) {
        res.push_back(it->second);
    }
    return res;
}
void TreeDict::update(py::kwargs kwargs) { batch_set(kwargs); }
void TreeDict::update(py::dict dict, py::kwargs kwargs) {
    batch_set(dict);
    batch_set(kwargs);
}
void TreeDict::update(py::iterable iterable, py::kwargs kwargs) {
    batch_set(iterable);
    batch_set(kwargs);
}
void TreeDict::clear() { map.clear(); }
py::object TreeDict::pop(py::object key) {
    auto it = map.find(key);
    if (it != map.end()) {
        auto res = it->second;
        map.erase(it);
        return res;
    } else {
        throw py::key_error();
    }
}
py::object TreeDict::pop(py::object key, py::object default_value) {
    auto it = map.find(key);
    if (it != map.end()) {
        auto res = it->second;
        map.erase(it);
        return res;
    } else {
        return default_value;
    }
}
std::pair<py::object, py::object> TreeDict::popitem() {
    if (map.empty()) {
        throw py::key_error();
    }
    auto it = map.begin();
    auto res = *it;
    map.erase(it);
    return res;
}
bool TreeDict::has_key(py::object key) { return map.contains(key); }
TreeDict TreeDict::copy() {
    TreeDict res;
    res.map = map;
    return res;
}
TreeDict TreeDict::fromkeys(py::iterable keys, py::object default_value) {
    TreeDict res;
    for (auto key : keys) {
        res.set(key, default_value);
    }
    return res;
}
py::object TreeDict::__getitem__(py::object key) {
    if (map.find(key) != map.end()) {
        return map.at(key);
    } else {
        throw py::key_error();
    }
}
void TreeDict::__setitem__(py::object key, py::object value) {
    map[key] = value;
}
bool TreeDict::__contains__(py::object key) { return map.contains(key); }
void TreeDict::__delitem__(py::object key) { map.erase(key); }
int64_t TreeDict::__len__() { return map.size(); }
py::object TreeDict::lower_bound(py::object key) {
    return map.lower_bound(key)->first;
}
py::object TreeDict::upper_bound(py::object key) {
    return map.upper_bound(key)->first;
}
py::str TreeDict::__str__() {
    if (map.empty()) {
        return "TreeDict()";
    }
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
py::str TreeDict::__repr__() { return __str__(); }
};  // namespace TreeDict
