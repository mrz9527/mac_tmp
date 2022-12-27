# -*- coding: utf-8 -*-

"""
自定义类，实现支持迭代器
需要实现两个方法:
    __iter__() : 用于返回一个迭代器对象
    __next__() ： 用于遍历迭代器对象中的元素，在__next__中设置迭代器终止条件，确保迭代器next不会出现无限next。
"""


class MyIter:
    def __iter__(self):
        self.a = 0
        return self

    def __next__(self):
        if self.a < 30:
            x = self.a
            self.a += 1
            return x
        else:
            raise StopIteration


my_iter = MyIter()
my_iter = iter(my_iter)
count = 0
while (True):
    if count > 200:
        break
    try:
        print(next(my_iter), end="\t")
    except StopIteration:
        break
    count += 1
