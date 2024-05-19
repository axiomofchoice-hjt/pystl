from pystl import TreeDict
a = TreeDict({1: 2, 3: 4})
a = TreeDict([(1, 2), (3, 4)])

print(str(a))
print(a.__dir__())
print({}.__dir__())
print(set({}.__dir__()) - set(a.__dir__()))
