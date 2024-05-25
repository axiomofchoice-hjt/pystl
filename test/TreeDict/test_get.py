from pystl import TreeDict


def test_get():
    a = TreeDict(a=1, c=5, d=7)
    s = str(a)
    assert a.get("d") == 7
    assert a.get("c") == 5
    assert a.get("a") == 1
    assert a.get("c", 4) == 5
    assert a.get("b", 4) == 4
    assert a.get("b") == None
    assert str(a) == s
