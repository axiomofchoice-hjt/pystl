from pystl import TreeDict

a = TreeDict({1: 2, 3: 4})
a = TreeDict([(1, 2), (5, 6), (3, 4)])

print(str(a))
print(a.__dir__())
print({}.__dir__())
print(set({}.__dir__()) - set(a.__dir__()))
for i in a.__iter__():
    print(i)

for i in reversed(a):
    print(i)
