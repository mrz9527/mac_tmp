# 	triton教程

# 1.下载和安装

## 1.0 注意版本关系

```
tensorflow 环境需要的cuda版本（模型推理需要用对应的cuda 版本)
cuda版本和nvidia driver版本的对应关系
triton server image和 gpu的对应关系

先看模型的框架和版本(比如tf的版本、onnx的版本、pytorch的版本)
	来确定对应的cuda版本（查看官网）
	再确定triton docker images版本（triton镜像版本和cuda\tensorflow等版本的对应关系）
```

## 1.1 nvidia驱动安装

根据gpu型号来安装nvidia驱动

* **查看nvidia显卡型号**

```sh
# dl2
$ lspci | grep -i nvidia
03:00.0 VGA compatible controller: NVIDIA Corporation GM200 [GeForce GTX TITAN X] (rev a1)
03:00.1 Audio device: NVIDIA Corporation Device 0fb0 (rev a1)
04:00.0 VGA compatible controller: NVIDIA Corporation GM200 [GeForce GTX TITAN X] (rev a1)
04:00.1 Audio device: NVIDIA Corporation Device 0fb0 (rev a1)
81:00.0 VGA compatible controller: NVIDIA Corporation GM200 [GeForce GTX TITAN X] (rev a1)
81:00.1 Audio device: NVIDIA Corporation Device 0fb0 (rev a1)
82:00.0 VGA compatible controller: NVIDIA Corporation GM200 [GeForce GTX TITAN X] (rev a1)
# dl5
ka-dl5:~ #lspci | grep -i nvidia
04:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
04:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
05:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
05:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
08:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
08:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
09:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
09:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
84:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
84:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
85:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
85:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
88:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
88:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
89:00.0 VGA compatible controller: NVIDIA Corporation GP102 [GeForce GTX 1080 Ti] (rev a1)
89:00.1 Audio device: NVIDIA Corporation GP102 HDMI Audio Controller (rev a1)
```

* **下载nvidia显卡驱动**

```
nvidia驱动下载地址：https://www.nvidia.com/Download/index.aspx?lang=en-us

dl2服务器：
GeForce GTX TITAN X
linux64位
推荐的是的nvdia驱动版本515.76，495.46驱动版本也是可以用的，dl5机器就安装的495.46驱动版本

驱动文件：NVIDIA-Linux-x86_64-515.76.run
```

* **nvidia驱动安装**

```
nvidia下载页面包含了nvidia驱动安装方法
sh ./NVIDIA-Linux-x86_64-515.76.run
```

驱动安装成功后，可以使用nvidia-smi查看。

```sh
# dl5 已经安装了460.39驱动，cuda driver version版本默认为11.2
zhoukang@kika-dl5:~ #nvidia-smi
Fri Nov 18 10:57:02 2022       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 460.39       Driver Version: 460.39       CUDA Version: 11.2     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  GeForce GTX 108...  Off  | 00000000:04:00.0 Off |                  N/A |
| 17%   24C    P0    58W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   1  GeForce GTX 108...  Off  | 00000000:05:00.0 Off |                  N/A |
| 16%   29C    P0    58W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   2  GeForce GTX 108...  Off  | 00000000:08:00.0 Off |                  N/A |
| 13%   27C    P0    58W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   3  GeForce GTX 108...  Off  | 00000000:09:00.0 Off |                  N/A |
| 17%   25C    P0    58W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   4  GeForce GTX 108...  Off  | 00000000:84:00.0 Off |                  N/A |
| 22%   29C    P0    57W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   5  GeForce GTX 108...  Off  | 00000000:85:00.0 Off |                  N/A |
| 28%   25C    P0    58W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   6  GeForce GTX 108...  Off  | 00000000:88:00.0 Off |                  N/A |
| 39%   27C    P0    58W / 250W |      0MiB / 11178MiB |      0%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
|   7  GeForce GTX 108...  Off  | 00000000:89:00.0 Off |                  N/A |
| 44%   26C    P0    57W / 250W |      0MiB / 11178MiB |      3%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                                  |
|  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
|        ID   ID                                                   Usage      |
|=============================================================================|
|  No running processes found                                                 |
+-----------------------------------------------------------------------------+

# nvidia 驱动为460.39，支持的最高cuda版本（cuda driver version)是11.2，也就是实际的cuda版本(cuda runtime version)必须小于等于11.2，大于11.2将不能正常工作。
# https://stackoverflow.com/questions/53422407/different-cuda-versions-shown-by-nvcc-and-nvidia-smi
```

## 1.2 cuda安装

* **cuda版本选择**

​	cuda安装的版本受限于nvidia驱动版本、深度学习框架的版本

```
cuda版本和nvidia驱动版本对应关系
https://docs.nvidia.com/cuda/cuda-toolkit-release-notes/index.html

tensorflow对应的cuda和cudnn版本
https://tensorflow.google.cn/install/source

onnx对应的cuda和cudnn版本
https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html
```

* **cuda需要的环境**
  https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html

```
在 Linux 上安装 CUDA 工具包和驱动程序之前必须采取一些措施：
验证系统是否具有支持 CUDA 的 GPU。	
	查看系统的GPU型号：lspci | grep -i nvidia
	查看cuda支持的gpu型号：https://developer.nvidia.com/cuda-gpus
验证系统正在运行受支持的 Linux 版本。
	查看系统的linux版本：uname -m && cat /etc/*release
	查看cuda支持的linux版本：https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html
验证系统是否安装了 gcc。	
	gcc --version
验证系统是否安装了正确的内核头文件和开发包。
	uname -r。
  centos安装: sudo yum install kernel-devel-$(uname -r) kernel-headers-$(uname -r)
  ubuntu安装: sudo apt-get install linux-headers-$(uname -r)
下载 NVIDIA CUDA 工具包。
处理冲突的安装方法。
```

* **cuda下载和安装**

```
下载地址：https://developer.nvidia.com/cuda-downloads
```

官方提供了三种安装方式，一般选择runfile方式安装。

```
rpm(local)
rpm(network)
runfile(local)
```

* **查看nvidia gpu算力**

https://developer.nvidia.com/cuda-gpus

```
# dl2
GeForce GTX TITAN X	5.2
# dl5
GeForce GTX 1080 Ti	6.1
```

## 1.3 docker版本

不同docker版本，使用gpu的方式不一样。以docker 19.03为分界线，推荐使用docker 19.03及以上docker版本。

https://www.cnblogs.com/linhaifeng/p/16108285.html

https://docs.nvidia.com/deeplearning/frameworks/user-guide/index.html

```
docker 19.03版本直接使用docker来启动gpu。
docker 19.03版本之前，需要借助nvidia-docker来启动gpu
```

```sh
# dl2
$ docker --version
Docker version 20.10.7, build f0df350

# dl3
$ docker --version
Docker version 18.09.2, build 6247962

$ nvidia-docker version
NVIDIA Docker: 2.8.0
Client:
 Version:           18.09.2
 API version:       1.39
 Go version:        go1.10.4
 Git commit:        6247962
 Built:             Tue Feb 26 23:56:24 2019
 OS/Arch:           linux/amd64
 Experimental:      false

Server:
 Engine:
  Version:          18.09.2
  API version:      1.39 (minimum version 1.12)
  Go version:       go1.10.4
  Git commit:       6247962
  Built:            Tue Feb 12 22:47:29 2019
  OS/Arch:          linux/amd64
  Experimental:     false
```

## 1.4 triton server 

triton server可以通过源码来安装部署，也可以借助镜像文件来安装部署，推荐使用容器来安装部署。

* **triton server镜像版本**

triton server镜像版本的选择与cuda版本、深度学习框架版本有关。具体对象关系为：

```
https://docs.nvidia.com/deeplearning/frameworks/support-matrix/index.html#framework-matrix-2021
```

* **拉取triton server镜像**

triton server镜像地址
https://catalog.ngc.nvidia.com/orgs/nvidia/containers/tritonserver

```sh
# 拉取镜像
# dl3: CUDA Version: 11.2
$ docker pull nvcr.io/nvidia/tritonserver:21.02-py3

# dl2: CUDA Version: 11.5
$ docker pull nvcr.io/nvidia/tritonserver:21.12-py3
```

## 1.5 triton client

triton client可以通过源码的方式来安装，也可以通过镜像的方式来安装，我这里采用的是镜像的方式

```
# 拉取镜像
# dl3: CUDA Version: 11.2
$ docker pull nvcr.io/nvidia/tritonserver:21.02-py3-sdk

# dl2: CUDA Version: 11.5
$ docker pull nvcr.io/nvidia/tritonserver:21.12-py3-sdk
```

# 2.triton操作

## 2.1 triton server启动

```sh
# docker 19.03之后  
$ docker run --gpus=2 --rm -p8000:8000 -p8001:8001 -p8002:8002 -v/home/zhoukang/model_repository:/models nvcr.io/nvidia/tritonserver:21.02-py3 tritonserver --model-repository=/models --strict-model-config=false
```

```sh
# docker 19.03之前

# 查看nvidia设备
#ls /dev/nvidia*
/dev/nvidia0  /dev/nvidia1  /dev/nvidia2  /dev/nvidia3  /dev/nvidia4  /dev/nvidia5  /dev/nvidia6  /dev/nvidia7  /dev/nvidiactl  /dev/nvidia-uvm  /dev/nvidia-uvm-tools

$ docker run -ti --device /dev/nvidia0:/dev/nvidia0 --device /dev/nvidia1:/dev/nvidia1 --rm -p8000:8000 -p8001:8001 -p8002:8002 -v/home/zhoukang/model_repository:/models nvcr.io/nvidia/tritonserver:21.12-py3 tritonserver --model-repository=/models
```

* **triton服务启动参数**

```
--model-repository=/models
		指定模型仓库的路径
--strict-model-config
		默认为true，模型必须搭配配置文件config.pbtxt
		false：模型可以没有配置文件config.pbtxt，此时triton服务会根据模型文件，自动检测相关配置参数。(仅部分模型文件不需要配置文件)
--model-control-mode
		默认为none，模型仓库在triton服务启动时加载，不会检测模型文件是否更改。
		poll:模型仓库在triton服务启动时加载，之后通过轮询检测模型文件是否发生改变。通过`--repository-poll-secs`设置轮询时间。
		explicit:通过使用triton提供的模型控制(model control)APIs，来加载和卸载模型。
```

```sh
# 指定triton服务的模型仓库路径
$ tritonserver --model-repository=/models
```

```sh
# 可以不使用模型的config.pbtxt，此时triton服务根据模型会自动解析得到config，
$ tritonserver --strict-model-config=false
```

```sh
# 通过triton服务提供的api，动态卸载和加载模型文件
$ tritonserver --model-repository=/models --strict-model-config=false --model-control-mode explicit

# 通过http+post方式动态加载densenet_onnx模型文件
$ curl -X POST http://localhost:8000/v2/repository/models/densenet_onnx/load

# 卸载densenet_onnx模型文件
$ curl -X POST http://localhost:8000/v2/repository/models/densenet_onnx/unload
```

## 2.2 triton client开启

```sh
$ docker run -it --name=triton_client --rm --net=host -v/home/zhoukang/triton_client_resource_dir:/triton_client_resource_dir  nvcr.io/nvidia/tritonserver:21.12-py3-sdk
```

## 2.3 获取triton服务的健康状态

```sh
# 或者 curl -v localhost:8000/v2/health/live
$ curl -v localhost:8000/v2/health/ready
*   Trying ::1:8000...
* TCP_NODELAY set
* Connected to localhost (::1) port 8000 (#0)
> GET /v2/health/ready HTTP/1.1
> Host: localhost:8000
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Length: 0
< Content-Type: text/plain
< 
* Connection #0 to host localhost left intact
```

## 2.4 获取模型的健康状态

```sh
# 模型有版本时的请求方式
$ curl -v localhost:8000/v2/models/{model_name}/versions/{model_version}/ready
# 模型没有版本时的请求方式
$ curl -v localhost:8000/v2/models/{model_name}/ready
```

```sh
root@kika-dl3:/workspace# curl -v localhost:8000/v2/models/densenet_onnx/versions/2/ready
*   Trying ::1:8000...
* TCP_NODELAY set
* Connected to localhost (::1) port 8000 (#0)
> GET /v2/models/densenet_onnx/versions/2/ready HTTP/1.1
> Host: localhost:8000
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Length: 0
< Content-Type: text/plain
< 
* Connection #0 to host localhost left intact
```

## 2.5 获取模型的metedata

```sh
# 模型有版本时的请求方式
$ curl -v localhost:8000/v2/models/{model_name}/versions/{model_version}
# 模型没有版本时的请求方式
$ curl -v localhost:8000/v2/models/{model_name}
```

```sh
root@kika-dl3:/workspace# curl -v localhost:8000/v2/models/densenet_onnx/versions/2      
*   Trying ::1:8000...
* TCP_NODELAY set
* Connected to localhost (::1) port 8000 (#0)
> GET /v2/models/densenet_onnx/versions/2 HTTP/1.1
> Host: localhost:8000
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Type: application/json
< Content-Length: 200
< 
* Connection #0 to host localhost left intact
{"name":"densenet_onnx","versions":["2"],"platform":"onnxruntime_onnx","inputs":[{"name":"data_0","datatype":"FP32","shape":[3,224,224]}],"outputs":[{"name":"fc6_1","datatype":"FP32","shape":[1000]}]}root@kika-dl3:/workspace# 
```

## 2.6 获取模型的配置

```sh
# 模型有版本时的请求方式
$ curl -v localhost:8000/v2/models/{model_name}/versions/{model_version}/config
# 模型没有版本时的请求方式
$ curl -v localhost:8000/v2/models/{model_name}/config
```

```sh
# curl -v localhost:8000/v2/models/densenet_onnx/versions/2       
*   Trying ::1:8000...
* TCP_NODELAY set
* Connected to localhost (::1) port 8000 (#0)
> GET /v2/models/densenet_onnx/versions/2 HTTP/1.1
> Host: localhost:8000
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Type: application/json
< Content-Length: 200
< 
* Connection #0 to host localhost left intact
{"name":"densenet_onnx","versions":["2"],"platform":"onnxruntime_onnx","inputs":[{"name":"data_0","datatype":"FP32","shape":[3,224,224]}],"outputs":[{"name":"fc6_1","datatype":"FP32","shape":[1000]}]}
```

## 2.7 动态加载和卸载模型

```sh
# 通过http+post方式动态加载模型文件
$ curl -X POST http://localhost:8000/v2/repository/models/{model_name}/load
# 卸载densenet_onnx模型文件
$ curl -X POST http://localhost:8000/v2/repository/models/{model_name}/unload
```

需要结合--model-control-mode explicit使用

```sh
# 通过triton服务提供的api，动态卸载和加载模型文件
$ tritonserver --model-repository=/models --strict-model-config=false --model-control-mode explicit
```

## 2.8获取模型的推理请求统计数据

```sh
# 指定版本的模型推理统计数据
curl -v localhost:8000/v2/models/{model_name}/versions/{model_version}/stats
# 所有版本的模型推理统计数据
curl -v localhost:8000/v2/models/{model_name}/stats
# 所有版本的所有模型的推理统计数据
curl -v localhost:8000/v2/models/stats
```

```sh
# curl -v localhost:8000/v2/models/densenet_onnx/versions/2/stats
*   Trying ::1:8000...
* TCP_NODELAY set
* Connected to localhost (::1) port 8000 (#0)
> GET /v2/models/densenet_onnx/versions/2/stats HTTP/1.1
> Host: localhost:8000
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Type: application/json
< Content-Length: 528
< 
* Connection #0 to host localhost left intact
{"model_stats":[{"name":"densenet_onnx","version":"2","last_inference":1668653704052,"inference_count":3,"execution_count":3,"inference_stats":{"success":{"count":3,"ns":1022772579},"fail":{"count":0,"ns":0},"queue":{"count":3,"ns":391893},"compute_input":{"count":3,"ns":1467957},"compute_infer":{"count":3,"ns":1019916323},"compute_output":{"count":3,"ns":179330}},"batch_stats":[{"batch_size":1,"compute_input":{"count":3,"ns":1467957},"compute_infer":{"count":3,"ns":1019916323},"compute_output":{"count":3,"ns":179330}}]}]}
```

## 2.9 获取系统能够共享内存

```sh
curl -v localhost:8000/v2/systemsharedmemory/region/{region_name}/status
curl -v localhost:8000/v2/systemsharedmemory/status
```

## 2.10 客户端发送推理请求

```sh
# post请求,需要指定请求头和数据
v2/models/{model_name}/versions/{model_version}/infer
v2/models/{}/infer
```

```sh
$ /workspace/install/bin/image_client -m densenet_onnx -c 3 -s INCEPTION /workspace/images/mug.jpg
Request 0, batch size 1
Image '/workspace/images/mug.jpg':
    15.346230 (504) = COFFEE MUG
    13.224326 (968) = CUP
    10.422965 (505) = COFFEEPOT
```

## 2.11 性能分析

```sh
# 在客户端容器内，执行性能分析
$ perf_analyzer -m densenet_onnx --concurrency-range 64 --max-threads 32 -u localhost:8001 -i gRPC
WARNING: Overriding max_threads specification to ensure requested concurrency range.
*** Measurement Settings ***
  Batch size: 1
  Using "time_windows" mode for stabilization
  Measurement window: 5000 msec
  Using synchronous calls for inference
  Stabilizing using average latency

Request concurrency: 64

  Client: 
    Request count: 58
    Throughput: 11.6 infer/sec
    Avg latency: 3850564 usec (standard deviation 111457 usec)
    p50 latency: 3873342 usec
    p90 latency: 3986121 usec
    p95 latency: 4003064 usec
    p99 latency: 4018885 usec
    Avg gRPC time: 3828611 usec ((un)marshal request/response 198 usec + response wait 3828413 usec)
  Server: 
    Inference count: 100
    Execution count: 100
    Successful request count: 100
    Avg request latency: 3826947 usec (overhead 351 usec + queue 3707280 usec + compute input 29 usec + compute infer 119259 usec + compute output 28 usec)

Inferences/Second vs. Client Average Batch Latency
Concurrency: 64, throughput: 11.6 infer/sec, latency 3850564 usec
```

# 3.模型库model repository

```
Triton 推理服务器从一个或多个模型存储库中提供模型，这些模型存储库在服务器启动时指定。当 Triton 运行时，可以按照模型管理中的描述修改正在服务的模型。
```

```sh
# triton server启动时指定model repository的路径，可以一次指定多个
$ tritonserver --model-repository=<model-repository-path>
```

## 3.1 模型仓库的布局

这些存储库路径是在使用 --model-repository 选项启动 Triton 时指定的。--model-repository 选项可以指定多次以包含来自多个存储库的模型。组成模型存储库的目录和文件必须遵循所需的布局。假设存储库路径指定如下。

```
$ tritonserver --model-repository=<model-repository-path>
```

```
<model-repository-path>/
    <model-name>/
      [config.pbtxt]
      [<output-labels-file> ...]
      <version>/
        <model-definition-file>
      <version>/
        <model-definition-file>
      ...
    <model-name>/
      [config.pbtxt]
      [<output-labels-file> ...]
      <version>/
        <model-definition-file>
      <version>/
        <model-definition-file>
      ...
    ...
```

第一级目录: model_repository。里面可以放入多个模型。
		第二级目录: 具体模型的名字作为目录名。
				第三级目录：模型的版本目录。目录下放入模型文件。	
				config.pbtxt:模型的配置。定义了一系列参数，决定了模型和服务的具体功能
				densenet_labels.txt:可选项。把分类模型的输出转换成一个实际的label标签的字符串。用在分类模型中。

## 3.2 模型文件类型

```
tensorRT: model.plan
ONNX:model.onnx
torchscripts:model.pt
tensorflow:model.graphdef 或者 model.savedmodel
python: model.py
DALI:model.dali
openVINO:model.xml和model.bin
custom: model.so
```

## 3.3 config.pbtxt 模型配置

config.pbtxt 文件描述了模型的[模型配置](https://github.com/triton-inference-server/server/blob/main/docs/user_guide/model_configuration.md)。对于某些模型，config.pbtxt 是必需的，而对于其他模型，它是可选的。config.pbtxt中有关详细信息参数，请参阅 [自动生成的模型配置](https://github.com/triton-inference-server/server/blob/main/docs/user_guide/model_configuration.md#auto-generated-model-configuration) 。

```
platform: "tensorrt_plan"
  max_batch_size: 8
  input [
    {
      name: "input0"
      data_type: TYPE_FP32
      dims: [ 16 ]
    },
    {
      name: "input1"
      data_type: TYPE_FP32
      dims: [ 16 ]
    }
  ]
  output [
    {
      name: "output0"
      data_type: TYPE_FP32
      dims: [ 16 ]
    }
  ]
```

```
tensorRT、tensorflow savedmodel、onnx，这三种，可以不使用config.pbtxt配置文件(通过tritonserver --strict-model-config=false)的方式，此时，模型的配置，triton会直接从模型文件中读取。
```

![image-20221013191714483](/Users/xm210408/Library/Application Support/typora-user-images/image-20221013191714483.png)

# 4. triton client c++说明

InferInput : 推理输入

InferRequestedOutput ： 推理请求输出

InferOptions ： 保存推理请求的选项参数

InferStat ： 推理统计

InferResult ： 解析推理结果

RequestTimers : 请求各个阶段的时间戳

InferenceServerClient:推理服务客户端 基类
		成员变量类型InferStat

InferRequest: 推理请求  基类
		成员变量类型RequestTimers

InferenceServerHttpClient : public InferenceServerClient

# 4. triton client镜像中client c++ demo单独编译

单独编译会出问题，使用下面的方法来代替

```
// 针对容器中单独编译triton client c++ demo的方法
https://github.com/triton-inference-server/server/issues/2691
```

```shell
mkdir -p /workspace/src/clients/c++/examples/build;
cd /workspace/src/clients/c++/examples/build;
sed -i 's/src\/clients\/c++\/library/\/workspace\/install\/include/g' ../image_client.cc;
mv ../CMakeLists.txt ../CMakeLists.txt.bak;
cmake .. && make
```

cmakelists.txt

```cmake
cmake_minimum_required (VERSION 3.18)
project(TritonExample)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Werror -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_definitions(-Wfatal-errors)

install(
  FILES
    ${CMAKE_SOURCE_DIR}/../../TRITON_VERSION
  DESTINATION .)

find_package(RapidJSON CONFIG REQUIRED)
add_library(
    json-utils-library EXCLUDE_FROM_ALL OBJECT
    json_utils.h json_utils.cc)

target_include_directories(
  json-utils-library
  PRIVATE /workspace
  PUBLIC ${RapidJSON_INCLUDE_DIRS})

set(CMAKE_PREFIX_PATH /workspace/builddir;/workspace/builddir/curl/install)
find_package(CURL REQUIRED)
find_package(TritonCommon REQUIRED)
find_package(TRITON REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(
  image_client
  image_client.cc
  $<TARGET_OBJECTS:json-utils-library>)

target_include_directories(
  image_client
  PRIVATE /workspace/install/include
  PRIVATE /workspace
  PRIVATE ${OpenCV_INCLUDE_DIRS})

link_directories(/workspace/instll/lib)

target_link_libraries(
  image_client
  PRIVATE TRITON::grpcclient_static
  PRIVATE TRITON::httpclient_static
  PRIVATE ${OpenCV_LIBS}
)

install(
  TARGETS image_client
  RUNTIME DESTINATION bin)
```

# 5. 使用自定义triton client

```
https://github.com/triton-inference-server/server/issues/2321
```

# 6. triton视频教程

https://www.bilibili.com/video/BV1234y157Xh/?spm_id_from=pageDriver&vd_source=b1edc0d53b051f430c488f5ab77f8074
