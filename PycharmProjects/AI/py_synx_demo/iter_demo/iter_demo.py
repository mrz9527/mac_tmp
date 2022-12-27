# -*- coding: utf-8 -*-

"""
迭代器相关方法有两个：
    iter()。用于创建一个迭代器对象
    next()。用于遍历迭代器对象中的下一个元素

迭代器：
    是一个可以记住遍历位置的对象。
    迭代器对象从集合的第一个元素开始访问，直到所有的元素被访问结束。迭代器只能往前，不会后退。
    遍历到最后一个元素后，继续next遍历，会出现异常，需要处理异常。

遍历迭代器中的元素:
    iter + next : iter()用于创建迭代器对象，next()用于获取迭代器对象的下一个元素。(通过next()遍历的方式，需要处理异常StopException）
    iter + for  : iter()用于创建迭代器对象，通过for循环来遍历迭代器对象中的元素

迭代器和内置容器的区别：
比如迭代器iter object和列表list object。
    迭代器只能循环遍历一次。迭代器每前进一次(next），迭代器头部就移动一个位置。
    list可以多次循环遍历。无论遍历多少次，list的头部永远在首个位置。

    迭代器不能通过Len()来获取元素的个数，只有一直next到最后出现异常，才能知道长度
    list可以通过len()来获取元素的个数
"""

print(__doc__)
print("--------------------------------------------------------------")
print("案例: list多次循环输出")
print("--------------------------------------------------------------")
lst = [1, 2, 3, 4]
print("lst = [1, 2, 3, 4], type(lst) = ", type(lst))
print("for循环遍历lst:")
for x in lst:
    print(x, end="\t")
print("\n再次for循环遍历lst:")
for x in lst:
    print(x, end="\t")

print("--------------------------------------------------------------")
print("案例: 迭代器多次循环输出")
print("--------------------------------------------------------------")
it = iter([1, 2, 3, 4])
print("it = iter([1, 2, 3, 4]), type(it) = ", type(it))
print("for循环遍历it:")
for x in it:
    print(x, end="\t")
print("\n再次for循环遍历it:")
for x in it:
    print(x, end="\t")
