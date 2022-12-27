

# 0. 服务器信息

## 跳板机

HostName:52.76.213.33

Port:10365

Username:zhoukang

Password:299223

## 远端机

一个开发机，一个测试机

|  ID  |          Host name           |      IP       |  Private IP  |
| :--: | :--------------------------: | :-----------: | :----------: |
| 1830 |    kika-engine-develop01     | 52.36.189.251 | 172.31.0.217 |
| 1842 | kika-engine-handwrite-test01 | 54.189.115.83 | 172.31.8.27  |

域名：http://hw-test.kika-backend.com:9090/OHCR/data



OS:CentOS

这里采用手写测试机

# 1. 上传源码到开发机

## 源码地址

git clone git@10.13.11.180:root/hw_data_collect_server.git

## sftp服务器

有两个sftp服务器，52.39.111.187（对应的内网ip是192.168.92.70） 和  54.189.115.83（内网ip不知道，需要问贾旭东），端口号都是10365，密码都是6tlncwB[xkrbqdvc

ip地址都是外网ip，mac本地机器可以访问

sftp服务器54.189.115.83，在开发机和测试机中无法连接，路由不通，（或许54.189.115.83对应的内网ip能通，但是内网ip不知道）；

sftp服务器52.39.111.187，在开发机路由通，测试机中路由不通，但是它的内网ip192.168.92.70能通测试机



所以，我统一使用sftp@52.39.111.187（内网中使用sftp@192.168.92.70)



## sftp文件传输

本机源码不能直接通过sftp上传到开发机，需要通过单独的sftp服务器中转，服务器的用户名是sftp，地址是52.39.111.187，端口是10365，"sftp"用户的密码是6tlncwB[xkrbqdvc

​	本机和sftp服务器连接命令：sftp -P 10365 sftp@52.39.111.187。输入密码后，本机向sftp服务器上传文件

​	开发机和sftp服务器连接命令：sftp -P 10365 sftp@52.39.111.187。输入密码后，从sftp服务器下载文件

```shell
[zhoukang@kika-engine-develop01 ~]$ sftp -P 10365 sftp@52.39.111.187
sftp@52.39.111.187's password: 
Connected to 52.39.111.187.
sftp> ls
sftp  
sftp> cd sftp
sftp> ls
engine  
sftp> cd engine
sftp> ls
cloudmodelserver.202105181150             glog                                      html.zip                                  hw_data_collect_server.zip                jsoncpp                                   
kikaime-engine-model-1155-fix.tar.gz      make_dict.zip                             model-data                                nano-5.8.tar.gz                           pinyin_cloud_server.zip                   
tcmalloc                                  valgrind-3.17.0.tar.bz2 
sftp> get -r hw_data_collect_server ~/projects/
```

## sftp命令

sftp -P 10365 sftp@52.39.111.187，连接后，输入help查看sftp支持的所有命令。

```shell
[zhoukang@kika-engine-develop01 ~]$ sftp -P 10365 sftp@52.39.111.187
sftp@52.39.111.187's password: 
Connected to 52.39.111.187.
sftp> 
sftp> help
Available commands:
bye                                Quit sftp
cd path                            Change remote directory to 'path'
chgrp grp path                     Change group of file 'path' to 'grp'
chmod mode path                    Change permissions of file 'path' to 'mode'
chown own path                     Change owner of file 'path' to 'own'
df [-hi] [path]                    Display statistics for current directory or
                                   filesystem containing 'path'
exit                               Quit sftp
get [-afPpRr] remote [local]       Download file
reget [-fPpRr] remote [local]      Resume download file
reput [-fPpRr] [local] remote      Resume upload file
help                               Display this help text
lcd path                           Change local directory to 'path'
lls [ls-options [path]]            Display local directory listing
lmkdir path                        Create local directory
ln [-s] oldpath newpath            Link remote file (-s for symlink)
lpwd                               Print local working directory
ls [-1afhlnrSt] [path]             Display remote directory listing
lumask umask                       Set local umask to 'umask'
mkdir path                         Create remote directory
progress                           Toggle display of progress meter
put [-afPpRr] local [remote]       Upload file
pwd                                Display remote working directory
quit                               Quit sftp
rename oldpath newpath             Rename remote file
rm path                            Delete remote file
rmdir path                         Remove remote directory
symlink oldpath newpath            Symlink remote file
version                            Show SFTP version
!command                           Execute 'command' in local shell
!                                  Escape to local shell
?                                  Synonym for help
```

### 常用sftp命令

help：查看

ls：列出目录内容

cd: 进入目录

put: 上传文件。上传中断后支持继续上传

put [-afPpRr] local [remote]       Upload file

reput [-fPpRr] [local] remote      Resume upload file

get: 下载文件，下载中断后支持继续下载

get [-afPpRr] remote [local]       Download file
reget [-fPpRr] remote [local]      Resume download file

bye：Quit sftp

# 2. 配置Anaconda

直接找到anaconda的清华下载地址，选择最新版本链接

https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-2021.05-Linux-x86_64.sh

## 下载Andaconda

```shell
cd ~ | mkdir packages
wget https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-2021.05-Linux-x86_64.sh --no-check-certificate
```

添加Anaconda3-2021.05-Linux-x86_64.sh可执行权限

```shell
chmod 777 Anaconda3-2021.05-Linux-x86_64.sh
```

## 安装Anaconda

```shell
./Anaconda3-2021.05-Linux-x86_64.sh
```

安装过程中可能需要配置安装路径，安装完成后需要初始化conda（在安装过程中会自动提示），跟着提示走即可。

安装完成后，linux下anaconda会自动创建一个python虚拟环境base，并进入该虚拟环境

退出默认的虚拟环境

```shell
(base) [zhoukang@kika-engine-develop01 ~]$ conda env list
# conda environments:
#
base                  *  /home/zhoukang/packages/
(base) [zhoukang@kika-engine-develop01 ~]$ conda deactivate
[zhoukang@kika-engine-develop01 ~]$
```

## 创建虚拟环境

创建hw_data_collect_server项目需要的虚拟环境，新建的虚拟环境名为”hw_data_collect_server_env“

```shell
[zhoukang@kika-engine-develop01 ~]$ cd ~/projects/hw_data_collect_server
[zhoukang@kika-engine-develop01 hw_data_collect_server]$ conda env create -n hw_data_collect_server_env -f ./env.yml
```

此时会自动下载env.yml文件中列出的包清单

## 启动虚拟环境

```shell
[zhoukang@kika-engine-develop01 hw_data_collect_server]$ conda activate hw_data_collect_server_env
(hw_data_collect_server_env) [zhoukang@kika-engine-develop01 hw_data_collect_server]$ 
```

# 3. 手写服务

## 启动手写服务

进入hw_data_collect_server/mysite目录

```shell
(hw_data_collect_server_env) [zhoukang@kika-engine-develop01 hw_data_collect_server]$ cd mysite/
(hw_data_collect_server_env) [zhoukang@kika-engine-develop01 mysite]$ ls
db.sqlite3  manage.py  mysite  online_casia
(hw_data_collect_server_env) [zhoukang@kika-engine-develop01 mysite]$ python manage.py runserver 0.0.0.0:8080
```

启动过程中可能会报错，或者web页面出现“[系统信息] 获取语料失败，请重试或联系李鸿亮”。

需要建立四个文件夹

./save_data

./save_data/alphabet

./save_data/Chinese

./save_data/English

因为资源请求失败，导致http返回状态码500



## 进入数据采集页面

在浏览器中打开http://localhost:8080/OHCR/data，或者http://52.36.189.251:8080/OHCR/data，ip是开发机的外网ip，也是启动手写服务所在的机器ip，端口8080是手写服务设定的端口。

可以用谷歌浏览器打开。

```shell
http://localhost:8080/OHCR/data
```

## 启动自动可视化采集数据的功能

进入hw_data_collect_server/data_tool目录，运行以下三条命令，可以启动自动可视化采集数据的功能

```shell
python vis_monitor.py ../save_data/English/ ../save_data_vis/English/ English
python vis_monitor.py ../save_data/Chinese/ ../save_data_vis/Chinese Chinese
python vis_monitor.py ../save_data/alphabet/ ../save_data_vis/alphabet/ alphabet
```

运行时可能会报如下错误：

![image-20211026105935656](/Users/xm210408/Library/Application Support/typora-user-images/image-20211026105935656.png)

提示缺少libGL.so.1文件，是OpenGL相关的库文件。需要在管理员权限下安装：

```shell
sudo yum install mesa-libGL
```

# 4. 数据采集需求

## 采集数据对象

英语

## 采集数据来源

基于app，每个app下面的规模

隐私问题

场景



字符之间的相关关系

海外测试数据：推特、web、facebook等

## 前期英语10万条

## 采集系统是否需要翻译成英语

体验官人员

范围、人员个数、每个人每天能够采集的规模

支持触摸和手写



1千万条语料，清洗后，提取10万条数据

```
int g_enModelParam[] = {6, 128, 512, 4, 104, 104, 128, 208};
int g_esModelParam[] = {6, 128, 512, 4, 104, 104, 128, 224};
char* g_enParamFile = "./param.bin";
char* g_enCharsetFile = "./char_set_208.txt";

char* g_enParamFile = "./param_es.bin";
char* g_enCharsetFile = "./char_set_es_224.txt";

int modelType = -1;


int* g_modelParam = nullptr;
char* g_paramFile = nullptr;
char* g_charsetFile = nullptr;
```

