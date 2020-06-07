# s4
A股相关工具，version4

争取跨平台，但目前先在Windows(VS)上开发/测试，有Linux需求的请自行测试，



## 要求 requirement

- Visual Studio 2017
- QT 5.11 (Install to E:\Qt\5.11.1 for examples.)
- cmake 3.9
- python 3.7
- ~~activeTcl https://www.activestate.com/products/tcl/downloads/~~

## 安装 install

```shell
git clone https://github.com/chinsaiki/s4.git
git submodule update --init --recursive

cd s4

# Call VS Developer Command Prompt, VS2017 default:
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
#"G:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

# 
cd build
cmake -A x64 -D_QT_PATH="E:\Qt\5.11.1\msvc2017_64" ..
#cmake -A x64 -D_QT_PATH="G:\Qt\5.11.1\msvc2017_64" ..
make
make install
```

## 数据库:sqlite
简单，响应快。
表的实际列需要比tableIO内实现的列多，暂不支持读取列变少的表。
