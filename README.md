# s4
A股相关工具，version4

争取跨平台，但目前先在Windows(VS2017/2019)上开发/测试，不定期在Linux(CentoOS8 gcc8.3)上编译测试

## Todo list
- TDX K线文件读取
- K线浏览，并叠加TDX导的出历史成交txt，便于复盘
- L1/L2行情可视化


## 要求 requirement

- Visual Studio 2017 or 2019
- QT 5.11+ (Install to E:\Qt\5.11.1 for examples.)
- cmake 3.9
- python 3.7
- ~~activeTcl https://www.activestate.com/products/tcl/downloads/~~
- ~~tushare 开源接口挂了~~

## 安装 install

### get source:
```shell
git clone https://github.com/chinsaiki/s4.git
git submodule update --init --recursive
```

### build
- windows
```shell
cd s4/build

# Call VS Developer Command Prompt
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

cmake -A x64 -D_QT_PATH="\path\to\Qt\5.1x.x\msvc2017_64" ..

#build：
cmake --build .

#install：
cmake --build . --target install --config Release
```

- linux
```shell
cd s4\build

cmake .. -D_QT_PATH=/path/to/Qt5.12.9/5.1x.x/gcc_64

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
