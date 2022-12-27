# ubuntu卸载、重装docker



1. 在dl5服务器上解决了triton容器中无法识别gpu的问题，之前是想通过nvidia-docker的方式来识别gpu没有成功，现在是升级了docker版本之后，解决了识别gpu的问题。
2. onnx模型部署到triton服务上，现在已经能正常运行了
3. 敏感词的一个服务，跑起来之后，需要发一个post请求，没有请求成功，需要再看一下