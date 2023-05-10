## [CMake 入门体验](#)
**介绍**： CMake的入门使用方式！

-----


### [CMake 注释](#)
CMake支持单行注释和多行注释！

```cmake
# my_script.cmake
# author:               kicker
# create time :         2023/03/28 10:38:11
# last alter time:      2023/03/28 10:38:11
# last alter author:    kicker

string(TIMESTAMP CURRENT_DATETIME "%Y/%m/%d %H:%M:%S")
message(STATUS "running at: ${CURRENT_DATETIME}")

#[[
    多行注释
    message(STATUS "running at: ${CURRENT_DATETIME}")
]]#

#[=[
    多行注释
    message(STATUS "running at: ${CURRENT_DATETIME}")
]=]#
```



### [单目录项目](#)
一个最简单的CMakeLists.txt 文件内容！
```txt
cmake_minimum_required(VERSION 3.22)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp)
```


### [多目录项目](#)
```txt
cmake_minimum_required(VERSION 3.22.1)

project(network)

set(CNameCoding true)

if(CNameCoding)
    message("is true ok")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

#c++ 版本设置为 c++17
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./src SRC_CPP)

message("list source:" ${SRC_CPP})

add_executable(network main.cpp ${SRC_CPP})
```

### [连接](#)

```
.
├── CMakeLists.txt
├── doc
├── include
│   ├── prints.h
│   └── toolkits.hpp
├── main.cpp
└── src
    └── prints.cpp
```

cmake
```
cmake_minimum_required(VERSION 3.22.1)

project(network)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)

message("list source:" ${SRC_CPP})

add_executable(network main.cpp)

add_library(prints SHARED ./src/prints.cpp)
#什么都不写就是静态库
#中间写一个 SHARED 就是动态库

target_link_libraries(network prints)
#将动态库连接到 network
```


目录:
```
├── CMakeLists.txt
├── doc
├── include
│   ├── prints.h
│   └── toolkits.cpp
├── levelDB
│   ├── CMakeLists.txt
│   ├── include
│   │   └── leveldb.h
│   └── src
│       └── leveldb.cpp
├── main.cpp
└── src
    └── prints.cpp

```

```
cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR) 

project(network 
    VERSION         1.0.0
    DESCRIPTION     "CMake Learning Demo" 
    HOMEPAGE_URL    "https://gitee.com/cicer/c-learning.git"
    LANGUAGES        C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(levelDB levelDB EXCLUDE_FROM_ALL)

include_directories(include )

add_executable(${PROJECT_NAME} main.cpp)

add_library(prints SHARED ./src/prints.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC prints levelDB)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/levelDB/include)
```