# -*- coding: utf-8 -*-

class Student:
    def __init__(self, name):
        self.name = name
        self._name = name
        self.__name = name

    def __print(self):
        print("__print : __name = ", self.__name)

    def _print(self):
        print("_print : _name = ", self._name)

    def print(self):
        print("print : name = ", self.name)


def test():
    student = Student("zhangsan")

    print("类成员命名:无下划线, 普通变量/函数，可以类外访问，在ide中会智能提示")
    print("student.name:", student.name)
    student.print()

    print("\n类成员命名:_开头, protected，受保护的变量/函数，不建议类外访问(可以类外访问)，在ide中不会智能提示")
    print("student._name:", student._name)
    student._print()

    print("\n类成员命名:__开头, private，私有的变量/函数，不能类外访问(还是可以通过特殊方式来访问，不建议这样)，在ide中不会智能提示")
    # print(student.__name)
    # student.__print()
    print("student._Student__name:", student._Student__name)
    student._Student__print()

    print("\n类成员命名:__开头，__结尾, 内置变量/函数，可以类外访问，在ide中会智能提示，不要自定义这类变量")
    print("__dict__:", student.__dict__)


if __name__ == "__main__":
    test()