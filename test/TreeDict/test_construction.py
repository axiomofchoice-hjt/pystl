from pystl import TreeDict
from collections import OrderedDict


def test_construction():
    assert str(TreeDict()) == "TreeDict()"
    assert str(TreeDict(a=1)) == "TreeDict([('a', 1)])"
    assert str(TreeDict(a=1, b=2)) == "TreeDict([('a', 1), ('b', 2)])"
    assert str(TreeDict(a=2, b=1)) == "TreeDict([('a', 2), ('b', 1)])"
    assert str(TreeDict(a="c", b=2)) == "TreeDict([('a', 'c'), ('b', 2)])"
    assert str(TreeDict(b=2, a=1)) == "TreeDict([('a', 1), ('b', 2)])"
    assert str(TreeDict(b=1, a=2)) == "TreeDict([('a', 2), ('b', 1)])"
    assert str(TreeDict({"b": 1, "a": 2})) == "TreeDict([('a', 2), ('b', 1)])"
    assert str(TreeDict({"a": 1, "b": 2})) == "TreeDict([('a', 1), ('b', 2)])"
    assert str(TreeDict({"a": 1, "b": 2}, a=3)) == "TreeDict([('a', 3), ('b', 2)])"
    assert str(TreeDict(OrderedDict(a=1, b=2))) == "TreeDict([('a', 1), ('b', 2)])"
    assert str(TreeDict([("a", 1), ("b", 2)])) == "TreeDict([('a', 1), ('b', 2)])"
    assert str(TreeDict((["a", 1], ("b", 2)), b=3)) == "TreeDict([('a', 1), ('b', 3)])"
