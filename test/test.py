from pystl import TreeDict

a = TreeDict({"a": 2, "b": 4}, a=1)
print(a)
a = TreeDict([("a", 2), ("b", 6)])
print(a)
a = TreeDict([(1, 2), (5, 6), (3, 4)])

print({i for i in a})

print(str(a))
for i in a.__iter__():
    print(i)

for i in reversed(a):
    print(i)

print(a.get(7))
print(a.get(7, 1))
