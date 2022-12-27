"""
Flask程序运行过程
1.当客户端想要获取资源时，一般会通过浏览器发起http请求
2.此时，web服务器会把来自客户端的所有请求都交给Flask程序实例
3. 程序实例使用Werkzeug来做路由分发（URL请求和视图函数之间的对应关系）
4. 根据每个URL请求，找到具体的视图函数，并进行调用
    在Flask程序中，路由的实现一般是通过程序实例的装饰器来实现的，装饰器的作用是将路由映射到视图函数中
5. Flask调用视图函数后，可以返回两种类容：
    字符串内容：将视图函数的返回值作为响应的内容，返回给客户端（浏览器）
    html模板内容：获取到数据后，把数据传入HTML模板文件中，模板引擎负责渲染http响应数据，然后返回响应数据给客户端（浏览器）

其他：
1. 限定路由请求方式，默认只支持GET
    @app.route('/', methods=['GET','POST']
2. 路由参数
    比如有个订单的路由'/orders',在订单下有成千上个具体订单号
    /orders/00001
    ...
    /orders/99999
    我们不可能针对每个订单号都定义一个路由，所以可以通过参数来做，将订单号当做路由orders的参数
    路由的参数由尖括号<>确定，尖括号内需要给定一个名字，（名字可以任意）
    @app.route('/orders/<order_id>'])
3. 路由参数的类型限定
    比如订单号只能是整数，不能是字符串,(默认路由参数是unicode编码的字符串）
    /orders/123
    /orders/aaa
    "orders/aaa"显然不符合订单号类型要求
    @app.route('/orders/<int:order_id>')
    原理是：将参数强转为int，如果成功，再进行路由，如果强转不成功，则找不到路由

Jinja2模板引擎
    视图函数的主要作用是生成请求的响应，实际上，视图函数有两个作用：处理业务逻辑和返回响应内容。在大型应用中，把业务逻辑和表现内容放在一起，
    会增加代码的复杂度和维护成本。模板的作用，就是承担视图函数的一个作用，即返回响应内容。

    模板是一个包含响应文本的文件，其中使用占位符（变量）表示动态部分，告诉模板引擎其具体的值需要从使用的数据中获取。
    使用真实值替换变量，再返回最终得到的字符串，这个过程称为渲染
    Flask是使用Jinja2这个模板引擎来渲染模板的

    使用模板的好处：
    视图函数只负责业务逻辑和数据处理（业务逻辑方面）
    而模板则取到视图函数的数据结果进行展示（视图展示方面）
    代码结果清晰，耦合度低

    渲染模板函数：
    render_template:第一个参数是模板的文件名，后面的参数采用键值对的形式，表示模板中变量对应的真实值。

    模板中：
    注释
        在pycharm中，在模板文件（比如index.html中），直接使用ctrl+/即可注释
        {# 注释内容 #}

    变量代码块
        {{}}来表示变量名，这种语法叫做变量代码块
        Jinja2模板中的变量代码块可以是任意python类型或者对象，只要它能够被python的str()方法转换为一个字符串就可以，比如：
        {{ post.tittle }}
        {{your_dict['key']}}
        {{your_list[0]}}
        还可以是常量
        {{ "hello" }}

    控制代码块
        用{%%}定义的控制代码块，可以实现一些语言层次功能，比如循环或者if语句
        {% if user %}
            {{ user }}
        {% else %}
            hello!
        {% endif %}

        {% for index in indexs %}
            {{ index }}
        {% endfor %}

    过滤器
    过滤器本质就是函数，有时候不仅仅只是需要输出变量的值，还需要修改变量的显示，甚至格式化、运算等，而在模板中不能直接使用python中的方法，
    那么就要用到过滤器
    过滤器种类：字符串过滤器、列表过滤器
    字符串过滤器：
        safe:禁用转义
            {{ '<em>hello</em>' | safe }}
        capitalize:把变量的首字母转成大写，其余字母小写
            {{ 'hello' | capitalize }}
        lower:把值转成小写
            {{ 'HELLO' | lower }}
        upprer:把值转成大写
        tittle:把值中的每个单词的首字母都转成大写
        reverse:字符串反转
        format:格式化输出
            {{ '%s is %d' | format('name', 17) }}
        striptags:渲染之前把值中所有的html标签都删掉
            {{ '<em>hello</em>' | striptags }}
        truncate:字符串截断
            {{ 'hello,world' | truncate(5) }}

    列表操作过滤器
        first:取第一个元素
            {{ [1,2,3,4,5] | first }}
        last: 取最后一个元素
        length:获取列表长度
        sum:列表求和
        sort:列表排序

    语句块过滤
    使用方法：
        变量名|过滤器
        {{ variable | filter_name(*args) }}
        如果没有任何参数传给过滤器，可以把括号省略掉
        {{ bariable | filter_name }}
    过滤器链式调用
        变量名|过滤器1 | 过滤器2 | ...|...

"""

# 1.导入Flask扩展
# render_template 用于视图函数中返回html文件，以及封装了该模板引擎
from flask import Flask, render_template, request
import time

# 2.创建Flask应用程序实例
# 需要传入导入名字，作用是为了确定资源所在路径
app = Flask(__name__)


# 3.定义路由和视图函数
# 路由定义是通过装饰器实现的，在浏览器中输入url，找到路由，执行对应的视图函数
# 视图函数返回两种内容，一种是字符串，一种是html文件
@app.route('/', methods=['GET'])
def root():
    return render_template('index.html')


@app.route('/demo.html')
def index():
    current_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
    my_list = [1, 3, 5, 7, 9]
    my_dict = {"name": "zhou.shi.qi", "age": "4.5"}
    return render_template('demo.html', current_time=current_time, my_list=my_list, my_dict=my_dict)


# 路由的参数有尖括号<>确定，尖括号内给个参数名字
@app.route('/orders/<int:order_id>')
def get_order_id(order_id):
    return f"order_id = {order_id}"

# 手写主界面
@app.route('/index.html', methods=['GET','POST'])
def hand_write():
    if request.method == "GET":
        return render_template('index.html')
    else:
        points = request.form.get("data")
        return "9090"


# 4.启动程序
if __name__ == '__main__':
    # 开启调试模式
    app.run(debug=True)
