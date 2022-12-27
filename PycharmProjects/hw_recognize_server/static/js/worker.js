onmessage = function (e) {
    // post发送数据
    var httpRequest = new XMLHttpRequest();
    httpRequest.open('POST', '/index.html', true);
    httpRequest.setRequestHeader("Content-type", 'application/json');
    var sendData = {"data": e.data};
    httpRequest.send(JSON.stringify(sendData));

    // 获取服务器返回结果
    httpRequest.onreadystatechange = function () {
        if (httpRequest.readyState == 4) {
            //根据服务器的响应内容格式处理响应结果
            if (httpRequest.getResponseHeader('content-type') === 'application/json') {
                var result = JSON.parse(httpRequest.responseText);
                //根据返回结果判断验证码是否正确
                if (result.code === -1) {
                    postMessage("result.code === -1")
                }
            } else {
                postMessage(httpRequest.responseText);
            }
        }
    }
}
