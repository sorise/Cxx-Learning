### [CMake Install 部署](#)

**介绍**：**cmake install**是一个CMake命令，用于在构建并编译项目后，将生成的文件安装到指定的位置。通过调用该命令，**可以将构建好的可执行文件、库文件、头文件以及其他必要的资源复制到安装目录，方便用户在系统中使用**。

-------

- [x] [1. Install-简单使用](#1-install-简单使用)
- [x] [2. 安装目标、头文件](#2-安装目标-头文件)
- [x] [3. 文件安装](#3-文件安装)
- [x] [4. 安装时执行shell](#4-安装时执行shell)
- [x] [5. find_package](#5-find_package)

-----

### [1. Install 简单使用](#)

CMake install 命令语法：

```cmake
install(TARGETS <target>... [...])
install(IMPORTED_RUNTIME_ARTIFACTS <target>... [...])
install({FILES | PROGRAMS} <file>... [...])
install(DIRECTORY <dir>... [...])
install(SCRIPT <file> [...])
install(CODE <code> [...])
install(EXPORT <export-name> [...])
install(RUNTIME_DEPENDENCY_SET <set-name> [...])
```

在上述示例中，通过设置`CMAKE_INSTALL_PREFIX`变量，指定了安装的目标目录。然后，使用`install`命令来指定需要安装的目标（例如可执行文件或库文件）以及它们的目标安装位置。还可以使用`install(DIRECTORY ...)`命令指定需要安装的整个目录。

```cmake
# 指定安装目录
set(CMAKE_INSTALL_PREFIX "/path/to/installation/directory")

# 安装可执行文件
install(TARGETS my_target DESTINATION bin)

# 安装库文件
install(TARGETS my_library DESTINATION lib)

# 安装头文件
install(DIRECTORY include/ DESTINATION include)

# 安装其他资源文件
install(FILES LICENSE.txt README.md DESTINATION doc)
```

`DESTINATION`参数指定了目标文件的相对安装路径。在示例中，`DESTINATION bin`表示将可执行文件安装到安装目录下的`bin`子目录，`DESTINATION lib`表示将库文件安装到`lib`子目录，`DESTINATION include`表示将头文件安装到`include`子目录。

最后，可以使用`install(FILES ...)`命令指定需要安装的其他文件，并使用`DESTINATION`参数指定安装位置。

在构建和编译项目后，执行`cmake install`命令将会安装指定的目标文件到相应的位置，使得用户能够方便地访问和使用这些文件。

#### 1.1 安装路径

在 CMake 中，可以使用 `CMAKE_INSTALL_PREFIX` 变量来指定安装路径。以下是一个示例：

**指令输出目录：**

```cmake
cmakeCopy Code# 设置安装路径为 /path/to/installation/directory
set(CMAKE_INSTALL_PREFIX "/path/to/installation/directory" CACHE PATH "Installation directory")

# 安装目标文件
install(TARGETS my_target DESTINATION bin)
```

#### 1.2 文件权限

安 装 的 文 件 默 认 权 限 OWNER_WRITE, OWNER_READ, GROUP_READ,WORLD_READ

指定已安装文件的权限。有效权限为 OWNER_READ、OWNER_WRITE、OWNAR_EXECUTE、GROUP_READ、GROUP-EXECUTE、WORLD_READ、WORLD_WRITE、WORLD_EXECUTE、SETUID 和 SETGID。在某些平台上没有意义的权限在这些平台上被忽略。

通过 PERMISSIONS 给文件设置权限

```cmake
install(FILES a.h DESTINATION pub
    PERMISSIONS 
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_WRITE GROUP_EXECUTE
    WORLD_READ WORLD_WRITE WORLD_EXECUTE
)
```



### [2. 安装目标、头文件](#)

目标包括静态库、动态库、可执行程序....

```cmake
install(TARGETS <target>... [...])
```

例子：DESTINATION 指定输出目录，bin是相对路径，实际目录为 **${CMAKE_INSTALL_PREFIX}/bin** 。

```cmake
install(TARGETS slib dlib ${PROJECT_NAME} DESTINATION bin)
```

* 默认安装路径
  * linux /usr/local
  * windows    `C:/Program Files (x86)`

指定 Linux 安装路径：

```shell
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=./out
```

```shell
cmake -S . -B win -DCMAKE_INSTALL_PREFIX=win_out
```

命令： 

```cmake
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr/local/out -DCMAKE_BUILD_TYPE=Release

cmake --build build
cmake --install build --config Debug  #安装 build 这个生成目录里面的目标文件。 版本是 debug 版本
```



#### 2.1 目标分类

* RUNTIME
  *  执行程序 
  * windows动态链接库dll文件
* ARCHIVE 
  * windows 动态库库导出符号
    * .lib on most Windows, .dll.a on Cygwin and MinGW
  * 静态库
    * add_library 添加 static 参数
    * windows 是 .lib, unix、linux和MinGW 是 .a
* 动态库 LIBRARY
  * linux、unix 是 .so 文件
  * mac dylib 文件
* 头文件 PUBLIC_HEADER、PRIVATE_HEADER  分为公开头文件和私有头文件
  * set_target_properties(slib PROPERTIES PUBLIC_HEADER  include/slib.h)
  * set_target_properties(slib PROPERTIES PRIVATE_HEADER  include/slib_pri.h)

 

```cmake
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION test_install/bin  # 执行程序和dll文件输出
)

install(TARGETS slib dlib
        RUNTIME DESTINATION test_install/bin  # 执行程序和dll文件输出
        ARCHIVE DESTINATION test_install/lib  # 静态库和windows动态库导出符号 .lib
        LIBRARY DESTINATION test_install/lib  # linux和mac的动态库 .so .dylib

        #头文件的安装
        PUBLIC_HEADER DESTINATION test_install/include #公开头文件
        PRIVATE_HEADER DESTINATION test_install/inc    #内部头文件
)
```

命令

```shell
### 安装目标 DESTINATION指定相对 CMAKE_INSTALL_PREFIX 的输出路径

## 默认安装路径
#    linux /usr/local
#    windows    C:/Program Files (x86)/

# Linux
#    cmake -S . -B build -DCMAKE_INSTALL_PREFIX=./out

# Windows 默认安装 Release版本
# cmake -S . -B win -DCMAKE_INSTALL_PREFIX=win_out

# cmake --build win
# cmake --install win --config Debug

# cmake --build win --config Release
# cmake --install win
```



#### 2.2 指定安装规则适用的构建配置列表（Debug，Release）

指定安装规则适用的生成配置的列表（调试、发布等）。请注意，为此选项指定的值仅适用于CONFIGURATION选项之后列出的选项。例如，要为“调试”和“发布”配置设置单独的安装路径，请执行以下操作：

```cmake
install(TARGETS target
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)
        
install(TARGETS target
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)
```

请注意，CONFIGURATION（配置）显示在RUNTIME DESTINATION（运行时间目的地）之前。

使用例子

```cmake
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION debug/bin
        )

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release RelWithDebInfo MinSizeRel
        RUNTIME DESTINATION release/bin
)

#[=[
debug release 不同输出路径
windows 编译过程
    cmake -S . -B win -DCMAKE_INSTALL_PREFIX=win_out
    cmake --build win --config Debug
    cmake --install win --config Debug
Linux 编译过程  Debug, Release, RelWithDebInfo and MinSizeRel,
    cmake -S . -B build -DCMAKE_INSTALL_PREFIX=out -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    cmake --install build --config Debug

    cmake -S . -B build -DCMAKE_INSTALL_PREFIX=out -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    cmake --install build --config Release
]=]
```



### 3. 文件安装

安装其他相关文件

```cmake
install(<FILES|PROGRAMS> files...
        TYPE <type> | DESTINATION <dir>
        [PERMISSIONS permissions...]
        [CONFIGURATIONS [Debug|Release|...]]
        [COMPONENT <component>]
        [RENAME <name>] [OPTIONAL] [EXCLUDE_FROM_ALL])
```

* OPTIONAL：如果要安装的文件不存在，请指定这不是错误。

我们可以给文件设定类型，每个类型有一个默认的安装路径。约定好的文件类型只需要指定类型即可，它会安装到默认位置。

| `TYPE` Argument | GNUInstallDirs Variable          | Built-In Default        |
| :-------------- | :------------------------------- | :---------------------- |
| `BIN`           | `${CMAKE_INSTALL_BINDIR}`        | `bin`                   |
| `SBIN`          | `${CMAKE_INSTALL_SBINDIR}`       | `sbin`                  |
| `LIB`           | `${CMAKE_INSTALL_LIBDIR}`        | `lib`                   |
| `INCLUDE`       | `${CMAKE_INSTALL_INCLUDEDIR}`    | `include`               |
| `SYSCONF`       | `${CMAKE_INSTALL_SYSCONFDIR}`    | `etc`                   |
| `SHAREDSTATE`   | `${CMAKE_INSTALL_SHARESTATEDIR}` | `com`                   |
| `LOCALSTATE`    | `${CMAKE_INSTALL_LOCALSTATEDIR}` | `var`                   |
| `RUNSTATE`      | `${CMAKE_INSTALL_RUNSTATEDIR}`   | `<LOCALSTATE dir>/run`  |
| `DATA`          | `${CMAKE_INSTALL_DATADIR}`       | `<DATAROOT dir>`        |
| `INFO`          | `${CMAKE_INSTALL_INFODIR}`       | `<DATAROOT dir>/info`   |
| `LOCALE`        | `${CMAKE_INSTALL_LOCALEDIR}`     | `<DATAROOT dir>/locale` |
| `MAN`           | `${CMAKE_INSTALL_MANDIR}`        | `<DATAROOT dir>/man`    |
| `DOC`           | `${CMAKE_INSTALL_DOCDIR}`        | `<DATAROOT dir>/doc`    |

使用：

```cmake
project(install_file)
file(WRITE a.h "")
file(WRITE b.h "")
file(WRITE c.h "")

# 文件安装到指定目录
install(FILES a.h b.h DESTINATION include)

# 目标可选 OPTIONAL 目标不存在不出错
install(FILES c.h DESTINATION inc OPTIONAL)

# 文件类型 TYPE DOC LIB INCLUDE
include(GNUInstallDirs) #需要打印 CMAKE_INSTALL_DATAROOTDIR  需要引入 GNUInstallDirs
message("CMAKE_INSTALL_DATAROOTDIR = ${CMAKE_INSTALL_DATAROOTDIR}")
install(FILES a.h TYPE DOC)     # <DATAROOT dir>/doc
install(FILES b.h TYPE LIB)     # lib
install(FILES c.h TYPE INCLUDE) # include


# 文件权限 windows目录无效
# 默认 权限 OWNER_WRITE, OWNER_READ, GROUP_READ, WORLD_READ
install(FILES a.h DESTINATION pub
    PERMISSIONS 
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_WRITE GROUP_EXECUTE
    WORLD_READ WORLD_WRITE WORLD_EXECUTE
)
```



#### 3.1 安装文件夹

安装目录：

```cmake
install(DIRECTORY dirs...
        TYPE <type> | DESTINATION <dir>
        [FILE_PERMISSIONS permissions...]
        [DIRECTORY_PERMISSIONS permissions...]
        [USE_SOURCE_PERMISSIONS] [OPTIONAL] [MESSAGE_NEVER]
        [CONFIGURATIONS [Debug|Release|...]]
        [COMPONENT <component>] [EXCLUDE_FROM_ALL]
        [FILES_MATCHING]
        [[PATTERN <pattern> | REGEX <regex>]
         [EXCLUDE] [PERMISSIONS permissions...]] [...])
```

使用：

```cmake
cmake_minimum_required(VERSION 3.22)
project(install_dir)
file(WRITE doc/index.html "")
file(WRITE doc/doc.html "")
file(WRITE doc/doc2.htm "")
file(WRITE doc/doc.cc "")
file(WRITE doc/doc.c "")
file(WRITE doc/sub/doc.html "")
file(WRITE doc/include/doc.h "")
file(WRITE doc/.svn/config "")
file(WRITE doc/.git/config "")
# DOC类型指定安装路径  share/doc
# 安装doc目录下所有文件，包含子目录中,空子目录也创建
install(DIRECTORY doc TYPE DOC)

install(DIRECTORY doc DESTINATION doc2)

# 过滤只复制*.html *.htm文件 包含子目录
install(DIRECTORY doc DESTINATION html_doc
FILES_MATCHING
PATTERN "*.html"
PATTERN "*.htm"
)

# 排除 .git 和.svn目录
install(DIRECTORY doc DESTINATION no_git_doc
PATTERN ".git" EXCLUDE
PATTERN ".svn" EXCLUDE
)


# 排除 .git 和.svn目录 安装 cc c 文件
install(DIRECTORY doc DESTINATION src
FILES_MATCHING
PATTERN "*.cc"
PATTERN "*.c"
PATTERN ".git" EXCLUDE
PATTERN ".svn" EXCLUDE
)
```



### 4. 安装时执行shell

安装时打印一些提示信息。

```cmake
project(install_code)
FILE(WRITE a.h "")
FILE(WRITE b.h "")
install(CODE "message(\"begin install\")")

install(FILES a.h TYPE INCLUDE)
install(CODE "message(\"a.h install success!\")")
install(FILES b.h TYPE INCLUDE)
install(CODE "message(\"b.h install success!\")")

# 写入安装的时间
# string(TIMESTAMP now "%Y-%m-%d %H:%M:%S")
# 获取当前时间戳，并转换时间格式，写入now变量
install (CODE [=[

string(TIMESTAMP now "%Y-%m-%d %H:%M:%S") 
message(${now})
FILE(APPEND install_log.txt "${now}\n")

]=])
```





### 5. find_package

`find_package` 是 CMake 中的一个命令，用于查找和加载其他 CMake 项目或软件包。它用于查找并加载指定软件包所需的配置文件，并提供了一种简化管理依赖关系的机制。

`find_package` 命令根据指定的软件包名称，在系统上查找该软件包的安装情况。如果找到了该软件包的安装，那么它会加载与之关联的配置文件，从而可以使用该软件包提供的功能。

以下是 `find_package` 的基本用法：

```cmake
find_package(package_name 
    [version] 
    [REQUIRED|COMPONENTS component1 component2 ...]
    [OPTIONAL_COMPONENTS optional_component1 optional_component2 ...]
)
```

- `package_name`：要查找的软件包名称。
- `version`：可指定要求的软件包版本号。
- `REQUIRED`：指定软件包为必需项。如果找不到该软件包，CMake 将报错并停止构建过程。
- `COMPONENTS`：列出软件包的组件，以便按需加载。
- `OPTIONAL_COMPONENTS`：列出可选的软件包组件。

示例用法：

```cmake
find_package(OpenCV 4.5 REQUIRED COMPONENTS core imgproc)

if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(my_target ${OpenCV_LIBS})
endif()
```

在上述示例中，使用 `find_package` 命令查找名为 "OpenCV" 的软件包，并要求版本为 4.5。同时指定 `REQUIRED` 标志，表示该软件包是必需的。

如果找到了 OpenCV 软件包，那么可以使用 `OpenCV_INCLUDE_DIRS` 变量将其头文件路径包含到项目中，使用 `OpenCV_LIBS` 变量将其库文件链接到目标可执行文件中。

根据实际情况，您需要根据所需的软件包进行适当的配置和使用。确保已正确安装所需的软件包，并在 CMakeLists.txt 文件中使用 `find_package` 命令来加载所需的功能。



**Search Modes**：

在 CMake 的 `find_package` 命令中，可以使用不同的查找模式来寻找软件包。以下是一些常见的查找模式：

1. `CONFIG` 模式：这是最常用的模式，它通过查找指定软件包所提供的 `<package-name>Config.cmake` 文件来加载配置。该文件通常由软件包的开发者提供，并包含了有关软件包的构建和链接信息。

   ```cmake
   find_package(package_name CONFIG)
   ```

2. `MODULE` 模式：该模式通过查找指定软件包的 `<package-name>Config.cmake` 或 `<package-name>-config.cmake` 文件来加载配置。此模式适用于没有提供专门的配置文件，而是提供了一个 CMake 模块文件的软件包。

   ```cmake
   find_package(package_name MODULE)
   ```

3. `COMPONENTS` 模式：当软件包被分为多个组件时，可以使用该模式加载指定的组件。这通常在一个大型的软件包中有多个独立的部分时使用。

   ```cmake
   find_package(package_name REQUIRED COMPONENTS component1 component2 ...)
   ```

   在这个模式下，可以指定 `REQUIRED` 标志来指示所需的组件是必要的。

4. `NO_MODULE` 模式：当软件包既没有提供 `Config` 文件，也没有提供模块文件时，可以使用该模式。它会查找与软件包相关的头文件和库文件，并提供变量来指示其位置。

   ```cmake
   find_package(package_name REQUIRED NO_MODULE)
   ```

   在这个模式下，可以指定 `REQUIRED` 标志来指示该软件包是必需的。

要根据特定的软件包和项目需求选择适当的查找模式。每个软件包都可能有不同的配置方式和命名约定，因此需要按照软件包的文档或说明来确定正确的查找模式和参数。

**CMAKE_MODULE_PATH**

* 用分号分隔的目录列表，用于在检查CMake附带的默认模块之前，由include或find_package命令加载的CMake模块的搜索路径。默认情况下，它为空。



使用变量：`find_package` 命令会设置一些变量，可以使用这些变量来访问软件包的信息，例如头文件路径、库文件路径等。通常，这些变量以软件包名称为前缀。

- `<package-name>_FOUND`：指示是否找到了软件包。
- `<package-name>_INCLUDE_DIRS`：软件包的头文件目录。
- `<package-name>_LIBRARIES`：软件包的链接库文件。
- `<package-name>_VERSION`：软件包的版本号。



#### 5.1 生成对于文件

```cmake
install(EXPORT <export-name> DESTINATION <dir>
        [NAMESPACE <namespace>] [FILE <name>.cmake]
        [PERMISSIONS permissions...]
        [CONFIGURATIONS [Debug|Release|...]
        [CXX_MODULES_DIRECTORY <directory>]
        [EXPORT_LINK_INTERFACE_LIBRARIES]
        [COMPONENT <component>]
        [EXCLUDE_FROM_ALL]
)
        
install(EXPORT_ANDROID_MK <export-name> DESTINATION <dir> [...])
```

使用：

```cmake
project(export_my_package)
# cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/home/xcj/xcpp
file(WRITE include/slib.h "void SLib();")
file(WRITE slib.cpp [=[
#include <iostream>
#include "slib.h"
void SLib()
{
    std::cout<<"In SLib function!\n";
}
]=])
add_library(slib STATIC slib.cpp)

#需要导出头文件
set_target_properties(slib PROPERTIES PUBLIC_HEADER include/slib.h)

#[[
Target "slib" INTERFACE_INCLUDE_DIRECTORIES property contains path:
    "/home/xcj/code/src/506export_my_package/include"
  which is prefixed in the source directory.

]]

#target_include_directories(slib PUBLIC include)

# 必须这样写
# 路径被两次引用 1 编译slib库时 2 install export写入config时
target_include_directories(slib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> #install时为空,只有编译时有值
    $<INSTALL_INTERFACE:include>  #只有install时有值 /home/xcj/xcpp/include
)

# -- Installing: /home/xcj/xcpp/lib/libslib.a
install(TARGETS slib
        EXPORT slib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)

# <PackageName>Config.cmake
# slibConfig.cmake  写入库路径 头文件路径
# /home/xcj/xcpp/lib/config
install(EXPORT slib FILE slibConfig.cmake DESTINATION config)
# 别人可以使用 find_package 引入 slib库
```



引用slib库测试

```cmake
file(WRITE main.cpp [=[
#include "slib.h"
#include <iostream>
using namespace std;
int main()
{
    cout<<"In Main"<<endl;
    SLib();
    return 0;
}
]=])
# cmake -S . -B build -DCMAKE_PREFIX_PATH=/home/xcj/xcpp/config
find_package(slib)
message("slib_FOUND = ${slib_FOUND}")
if(slib_FOUND)
    message("find slib success!")
endif()
add_executable(main main.cpp)
target_link_libraries(main slib)

# 头文件路径写在属性中
get_target_property(inc slib INTERFACE_INCLUDE_DIRECTORIES)
message("INTERFACE_INCLUDE_DIRECTORIES = ${inc}")
```

