// author : zhoukang
// date   : 2022-07-12 11:56:58

#include "Application/CryptoStream.h"
#include "Application/BufferedStream.h"
#include "Framework/FileStream.h"
#include "iostream"

int main() {
    std::cout<<"单独对文件流read/write/seek---"<<std::endl;
    // 可以单独对FileStream进行操作，它是独立的个体
    Stream* fileStream = new FileStream;
    fileStream->read();
    fileStream->write();
    fileStream->seek();

    std::cout<<"\n对文件流read/write/seek操作进行加密---"<<std::endl;
    // 对FileStream进行扩展
    CryptoStream* cryptoFileStream = new CryptoStream(fileStream);
    cryptoFileStream->read();
    cryptoFileStream->write();
    cryptoFileStream->seek();

    std::cout<<"\n对文件流read/write/seek操作进行缓冲和加密---"<<std::endl;
    BufferedStream* bufferedStream = new BufferedStream(cryptoFileStream);
    bufferedStream->read();
    bufferedStream->write();
    bufferedStream->seek();
    return 0;
}

