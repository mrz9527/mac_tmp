
#include<iostream>
#include "memory"
#include "stdio.h"


using namespace std;

//class Node
//{
//public:
//    int _data;
//    shared_ptr<Node> _next;
//    shared_ptr<Node> _prev;
//    explicit Node(int val):_data(val){printf("Construct _data = %d\n", _data);};
//    ~Node(){printf("Deconstruct _data = %d\n", _data);};
//};

class Node
{
public:
    int _data;
    weak_ptr<Node> _next;
    weak_ptr<Node> _prev;
    explicit Node(int val):_data(val){printf("Construct _data = %d\n", _data);};
    ~Node(){printf("Deconstruct _data = %d\n", _data);};
};

int main()
{
    //shared_ptr<Node> sp1 = make_shared<Node>(5);
    //shared_ptr<Node> sp2 = make_shared<Node>(2);
    //sp1->_next = sp2;
    //sp2->_prev = sp1;
    //
    //cout << "sp1 reference count = " << sp1.use_count() << endl;
    //cout << "sp1->_next reference count = " << sp1->_next.use_count() << endl;
    //cout << "sp1->_prev reference count = " << sp1->_prev.use_count() << endl;
    //
    //cout << "sp2 reference count = " << sp2.use_count() << endl;
    //cout << "sp2->_next reference count = " << sp2->_next.use_count() << endl;
    //cout << "sp2->_prev reference count = " << sp2->_prev.use_count() << endl;

    std::string str1("hello,world");
    printf("str1:%s\n",str1.c_str());

    std::move(str1);
    printf("str1:%s\n",str1.c_str());
    return 0;
}
