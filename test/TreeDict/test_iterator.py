from pystl import TreeDict


def test_iterator():
    a = TreeDict(a=1, d=7, c=5, b=3)
    s = str(a)
    assert [i for i in a] == ["a", "b", "c", "d"]
    assert [i for i in a.iterkeys()] == ["a", "b", "c", "d"]
    assert [i for i in a.itervalues()] == [1, 3, 5, 7]
    assert [i for i in a.iteritems()] == [("a", 1), ("b", 3), ("c", 5), ("d", 7)]
    assert [i for i in reversed(a)] == ["d", "c", "b", "a"]
    assert str(a) == s
