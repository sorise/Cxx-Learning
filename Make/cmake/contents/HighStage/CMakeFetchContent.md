## FetchContent
CMake从3.11版本以后提供的功能，FetchContent 提供了一种简洁的方法来管理依赖项的获取和构建过程，它通常用于从远程源代码管理系统（如**GitHub**、**GitLab**等）拉取源代码，并将其包含到您的 CMake 构建过程中。



### 1. 使用流程：

(1). **引入FetchContent模块**： 在项目的顶级CMakeLists.txt文件中添加对FetchContent模块的引用。

```cmake
cmake_minimum_required(VERSION 3.11) # 确保CMake版本大于等于3.11
project(MyProject)

include(FetchContent) # 引入FetchContent模块
```

(2). **声明要获取的内容**： 使用 **FetchContent_Declare** 函数声明你要获取的外部项目及其源代码位置。通常，这可以通过URL指向一个Git仓库或者直接指定本地路径。

```cmake
FetchContent_Declare(
    MyDependency # 给这个外部项目一个内部名称
    GIT_REPOSITORY https://github.com/some/dependency.git # Git仓库地址
    GIT_TAG v1.0.0 # 指定要检出的标签或分支
)
```

(3). **获取并配置依赖**：使用 FetchContent_MakeAvailable 或者 FetchContent_Populate 加载并配置项目。FetchContent_MakeAvailable 是一个便捷函数，它会调用 FetchContent_Populate 并随后调用 add_subdirectory 来自动构建和链接依赖项目。

```cmake
FetchContent_MakeAvailable(MyDependency)
```

