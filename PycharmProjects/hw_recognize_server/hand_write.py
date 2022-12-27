# -*- coding: utf-8 -*-

import ctypes

so = ctypes.CDLL("./handwriteResource/libolhctInference.so")
print(so)
