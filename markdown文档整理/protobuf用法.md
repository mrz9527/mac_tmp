## .proto格式

ClassInfo.proto文件

```protobuf
syntax = "proto3";	// protobuf协议版本
package tutorial;		// 域名空间

message Subject {
  string major = 1;
  float score = 2;
}

message Student {
  int32 id = 6;
  string name = 1;
  int32 age = 2;

  enum GenderType {
    GT_BOY = 0;
    GT_GIRL = 1;
  }

  GenderType gender = 3;
  string homeAddress = 4;

  repeated Subject subjects = 5;
}

message ClassInfo {
  repeated Student students = 1;
}
```

### .proto编译

```shell
# 使用protoc编译当前目录下ClassInfo.proto文件，输出为编译后的cpp文件，输出路径也是当前目录
protoc -I=./ --cpp_out=./ ClassInfo.proto
```

## Protobuf用法

```
google::protobuf::DescriptorPool
		描述池，一般通过类的静态方法来构造自身，作用：查找各种描述
google::protobuf::MessageFactory  
		Message工厂，一般通过类的静态方法来构造自身，作用：通过Message的描述Descriptor来创建Message对象

FileDescriptor 	 .proto文件描述
Descriptor：			 .proto中的Message描述
FieldDescriptor	 Message中的Field描述

Reflection				反射
google::protobuf::util	probuf提供的工具类
```

### google::protobuf::DescriptorPool

描述池
**构造：通过类的静态方法来创建**

```c++
// 构造描述池DescriptorPool
const DescriptorPool* descriptorPool = DescriptorPool::generated_pool();
```

**作用：用来查找描述**

```c++
// 查找message描述、file描述、field描述
std::string messageName;
const Descriptor* messageDescriptor = descriptorPool->FindMessageTypeByName(messageName);
std::string fileName;
const FileDescriptor* fileDescriptor2 = descriptorPool->FindFileByName(fileName);
std::string fieldName;
const FieldDescriptor* fieldDescriptor2 = descriptorPool->FindFieldByName(fieldName);
```

### google::protobuf::MessageFactory

Message工厂

**构造：通过类的静态方法来创建**

```c++
// 构造Message工厂
MessageFactory* factory = MessageFactory::generated_factory();
```

**作用：获取Message类型并创建message对象**

```c++
// 获取message的类型
const Message* messageType = factory->GetPrototype(messageDescriptor);
// 创建Message实例
Message* messageInstance = messageType->New();
```

### google::protobuf::Descriptor

message描述符

```
获取google::protobuf::Descriptor的两种方法。
	1. 已知message对象的前提下，auto messageDescriptor = message.GetDescriptor();
	2. 已知message的类型名字, std::string messageName;
			2.1创建描述池
				auto descritprPool = google::protobuf::DescriptorPool::generated_pool();
			2.2通过描述池和message类型名字
				auto messageDescriptor = descritprPool->FindMessageTypeByName(messageName);
```

**作用：获取message中field描述符**

```c++
// 获取字段field的数量
int fieldCount = messageDescriptor->field_count();
for (int i = 0; i < fieldCount; ++i) {
  // 获取字段field描述符
  auto fieldDescriptor = messageDescriptor->field(i);
}
```

### google::protobuf::FieldDescriptor

获取字段field描述符

```
fieldDescriptor = messageDescriptor->field(index);
```

获取字段field的名字

```
std::string fieldName = fieldDescriptor->name();
```

**判断字段是否是数组**

```
fieldDescriptor->is_repeated();
```

**获取字段的数据类型**

```
auto cppType = fieldDescriptor->cpp_type();
```

protobuf支持的数据类型

```c++
  enum CppType {
    CPPTYPE_INT32 = 1,     // TYPE_INT32, TYPE_SINT32, TYPE_SFIXED32
    CPPTYPE_INT64 = 2,     // TYPE_INT64, TYPE_SINT64, TYPE_SFIXED64
    CPPTYPE_UINT32 = 3,    // TYPE_UINT32, TYPE_FIXED32
    CPPTYPE_UINT64 = 4,    // TYPE_UINT64, TYPE_FIXED64
    CPPTYPE_DOUBLE = 5,    // TYPE_DOUBLE
    CPPTYPE_FLOAT = 6,     // TYPE_FLOAT
    CPPTYPE_BOOL = 7,      // TYPE_BOOL
    CPPTYPE_ENUM = 8,      // TYPE_ENUM
    CPPTYPE_STRING = 9,    // TYPE_STRING, TYPE_BYTES
    CPPTYPE_MESSAGE = 10,  // TYPE_MESSAGE, TYPE_GROUP

    MAX_CPPTYPE = 10,  // Constant useful for defining lookup tables
                       // indexed by CppType.
  };
```

### google::protobuf::Reflection

message的反射。

```
获取反射google::Protobuf::Relection
	1. 根据message来获取反射
	auto reflection = message.GetReflection();
```

**判断message中字段field是否存在**

因为Message是可以添加字段的，支持向前兼容和向后兼容，新添加的字段在前一个版本中不存在，所以需要校验字段field

```c++
// HasField函数只能判断singular field，不能用于判断repeated field（错误使用会直接崩溃）
reflection->HasField(message, fieldDescriptor);
```

**获取message中字段field的值**

```
// 每个字段field的数据类型，都有相应的Get方法。
auto fieldValue = reflection->GetXXX(message, fieldDescriptor);
```

**修改message中字段field的值**

```
// 每个字段field的数据类型，都有相应的Set方法。
reflection->SetXXX(message, fieldDescriptor, fieldValue);
```

**添加新的元素到repeated field数组**

```
reflection->AddXXX(message, fieldDescriptor, fieldValue);
```

### google::protobuf::util

实现protobuf和json的互转

```c++
google::protobuf::util::MessageToJsonString(message, &strJson);
google::protobuf::util::Status status = google::protobuf::util::JsonStringToMessage(strJson, message);
```







![image-20221026142525064](/Users/xm210408/Library/Application Support/typora-user-images/image-20221026142525064.png)

![image-20221026142610065](/Users/xm210408/Library/Application Support/typora-user-images/image-20221026142610065.png)

![image-20221026142637301](/Users/xm210408/Library/Application Support/typora-user-images/image-20221026142637301.png)
