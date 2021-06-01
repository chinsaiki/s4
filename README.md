# s4
A股相关工具，version4

争取跨平台，但目前先在Windows(VS)上开发/测试，有Linux需求的请自行测试，



## 要求 requirement

- Visual Studio 2017
- QT 5.11 (Install to E:\Qt\5.11.1 for examples.)
- cmake 3.9
- python 3.7
- ~~activeTcl https://www.activestate.com/products/tcl/downloads/~~
- ~~tushare 开源接口挂了~~

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

#! 首次执行cmake会找不到build下的一些QT的.h文件，此时执行一次"cmake --build ."将自动生成.h，然后再执行一次cmake ..即可
cmake -A x64 -D_QT_PATH="E:\Qt\5.11.1\msvc2017_64" ..
# cmake -A x64 -D_QT_PATH="d:\Qt\5.11.2\msvc2017_64" ..
#cmake -A x64 -D_QT_PATH="G:\Qt\5.11.1\msvc2017_64" ..

#build：
cmake --build .

#install：
cmake --build . --target install --config Release

```

* 若Linux 出现 Failed to find "GL/gl.h" in "/usr/include/libdrm" :
  sudo yum install mesa-libGL-devel mesa-libGLU-devel

## 数据库:sqlite
简单，响应快。
表的实际列需要比tableIO内实现的列多，暂不支持读取列变少的表。

## Thirdparty && Reference

- asio : https://github.com/chriskohlhoff/asio
- SQLiteCpp : https://github.com/SRombauts/SQLiteCpp
- logger : https://github.com/gabime/spdlog
- json : https://github.com/nlohmann/json
- sqlite3-viewer : https://github.com/darkest-days/sqlite3-viewer
