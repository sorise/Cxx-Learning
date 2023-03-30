### [CMake 库管理](#)
**介绍**: 如何管理自己的库文件，如何引入第三方库，管理第三方依赖，如何将自己的工程打包发布成第三方库！ 由于C++的依赖管理和包管理十分困难，所以学习CMake处理显得十分重要！

-----
### [1. CXX 依赖与包管理](#)
这是十分复杂的，因为C++天生就没有自己的依赖管理里和库管理工具，更没有模块的概念！为了因为这个尴尬的处境，微软推出了**vcpkg**, C++ 20推出了**module**方案，
但是各种原因，目前为止，这对于C++而言，仍然是一个非常糟糕的缺点，限制了C++生态的发展。 

目前使用第三方库有几种方法：
* 直接使用第三方库的源代码(不推荐)，例如Catch2的2.X版本，直接引入一个hpp头文件即可！
* 查找已安装的软件包，使用指令 **find_package**, 这需要学会编写查找模块，才能看懂别人的查找cmake代码。
* 使用FindPkgonfig发现旧版本包(已经过时)。
* 使用 [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html#id1) 直接到git上面拉取第三方库。
* 使用ExternalProject。

#### [1.1 库管理相关指令](#)
与库操作相关的指令。

|命令|解释|
|:----|:----|
|include_directories|用于指定头文件搜索路径，优点是简单直接，缺点是无法进行权限控制，一旦被执行后，后续的所有代码都能搜索到对应的文件路径。|
|link_directories|向工程添加多个特定的库文件搜索路径。|
|target_include_directories|该命令可以指定目标（exe或者so文件）需要包含的头文件路径|
|target_link_libraries|用于将可执行文件或库文件链接到库文件或可执行文件。|
|find_file|查找单个文件|
|find_library|查找库文件|
|find_package|找到一个包（通常由项目外部的东西提供），并加载其包特定的详细信息。|
|find_path|查找包含特定文件的路径|
|find_program|查找可执行程序|

#### [1.2 使用库的底层逻辑](#)
如果我们引入一个库，一般需要如下的内容： **头文件** **静态库** **动态库**。也就是如果一个库的内容包含了上述三部分，
那么就都需要引入到项目中来。


### [2. find_package和FetchContent](#)
将依赖项引入构建的主要方法是find_package()命令和 FetchContent 模块。FindPkgConfig模块有时也会被使用，尽管它缺乏其他两个模块的一些集成。

项目所需的包可能已经构建好，并且可以在用户系统的某个位置使用。该包可能也是由CMake构建的，也可能完全使用了不同的构建系统。它甚至可能只是一
个根本不需要构建的文件集合。CMake为这些场景提供了find_package（）命令。它搜索已知的位置，以及项目或用户提供的其他提示和路径。它还支持
包组件和可选的包。提供了结果变量，允许项目根据是否找到包或特定组件来自定义自己的行为。

在大多数情况下，项目通常应该使用基本签名。大多数情况下，这将只涉及包名称，可能是版本约束，以及REQUIRED关键字（如果依赖项不是可选的）。还可以指定一组封装组件。

```cmake
find_package(Catch2)
find_package(GTest REQUIRED)
find_package(Boost 1.79 COMPONENTS date_time)
```

**find_package命令支持两种主要的搜索方法：** 根据使用的参数，find_package（）可能使用上述方法中的一个或两个。
* **Module mode**: 并非所有软件包都支持CMake。许多没有提供支持配置模式所需的文件。对于这种情况，Find模块文件可以由项目或CMake单独提供。Find模块通常是一种启发式实现，它知道包通常提供什么以及如何将该包呈现给项目。由于Find模块通常与软件包分开分发，因此它们不那么可靠。它们通常是单独维护的，并且可能遵循不同的发布时间表，因此很容易过时。
* **Config mode**: 使用此方法，命令会查找通常由包本身提供的文件。这是两者中更可靠的方法，因为包的详细信息应该始终与包同步。

**配置文件**: 第三方库需要提高一个cmake配置文件，说明如何引入第三方库的可执行文件、库、头文件等，由find_package（）命令读取。通常的存放位置为`lib/cmake/<PackageName>`,
此文件是库的入口点，也可以是其他位置！

如下命令：
```cmake
find_package(SomeThing
  NAMES
    SameThingOtherName   # Another name for the package
    SomeThing            # Also still look for its canonical name
)
```
配置文件必须命名为\<PackageName\>config.cmake或\<LowercasePackageName\>-config.cmake。
名为\<PackageName\>ConfigVersion.cmake或\<LowercasePackageName\>-config-version.cmake的单独可选文件也可能存在于同一目录中。
CMake使用此文件来确定包的版本是否满足find_package（）调用中包含的任何版本约束。在调用find_package（）时指定版本是可选的，即使存在\<PackageName\>ConfigVersion.cmake文件
也是如此。

#### [2.1 find_package 模块模式-搜索配置文件](#)
在该模式下，Cmake会搜索一个名为Find\<PackageName\>.cmake的文件，其中\<PackageName\>为待搜索包的名称。

搜索路径的顺序依次是：
* 从变量 **CMAKE_MODULE_PATH** 指定的路径中进行查找。
* 从Cmake安装路径中查找。Cmake会在其安装路径下提供很多.cmake文件，例如/XXX/cmake/Modules/目录下（不同的系统安装目录可能不一致）

如果找到文件Find\<PackageName\>.cmake，Cmake会读取并处理该文件，简而言之，它负责检查一些条件（如版本号是否满足等）是否满足，并
在找到包后，返回给调用者一些变量，用以获取包的详细信息。

一般来说，Find\<PackageName\>.cmake文件不是随包本身一起提供的，更多的是外部针对已有包的重新包装，例如操作系统、Cmake程序、甚
至是调用find_package命令的工程针对已有的包提供针对该包的.cmake文件。


#### [2.2 find_package 配置模式-搜索配置文件](#)
该模式下，CMake会搜索\<lowercasePackageName\>-config.cmake文件或\<PackageName\>Config.cmake文件。

如果find_package命令中指定了具体的版本，也会搜索\<lowercasePackageName\>-config-version.cmake或\<PackageName\>ConfigVersion.cmake文件，因此
配置模式下通常会提供配置文件和版本文件（注意形式上要保持一致），并且作为包的一部分一起提供给使用者。

该模式下对.cmake文件的搜索路径的顺序比较复杂。

#### [2.3 find_package 基本签名](#)
find_package用于查找包（通常是使用三方库），并返回关于包的细节（使用包所依赖的头文件、库文件、编译选项、链接选项等）。
find_package命令有两种格式，基本命令格式和完整命令格式，先介绍基本命令格式。
```cmake
find_package(<PackageName> [version] [EXACT] [QUIET] [MODULE]
    [REQUIRED] [[COMPONENTS] [components...]]
    [OPTIONAL_COMPONENTS components...]
    [REGISTRY_VIEW  (64|32|64_32|32_64|HOST|TARGET|BOTH)]
    [GLOBAL]
    [NO_POLICY_SCOPE]
    [BYPASS_PROVIDER]
)
```

* **PackageName**：待查找包的名称。此外它还决定两种搜索模下的.cmake文件名称：例如模块模式下的名称为Find`<PackageName>.cmake`，而配置模式下为`<lowercasePackageName>-config.cmake` `<lowercasePackageName>-config-version.cmake`。
* **version**：待查找包的版本号要求，版本号为点分格式，由四个部分组成，每个部分都是一个数字，均为可选：major `[.minor[.patch[.tweak]]]`，
例如1.1.1.1、1.0、等。同样也可以指定版本范围（CMake 3.19及之后才支持），格式为：`versionMin...[<]versionMax，versionMin和versionMax`均
是 `major[.minor[.patch[.tweak]]]`形式的版本号，默认情况下会包含这个指定区间两端的版本号，但如果指定了`<`，那么会排除掉versionMax，例
如1.1.1.1...1.1.2.0、1.1.1.1...`<`1.1.2.0等。
* **MODULE**：该选项指定find_package命令只使用模块模式搜索方式查找。未指定该选项时，find_package会优先使用模块模式搜索，仍未找到包时，会切换成配置模式搜索。

* **EXACT**：该选项要求待查找包的版本必须与指定的版本精确匹配，因此如果指定的是一个版本范围，不能使用该参数。
* **QUIET**：禁止输出信息，正常情况当找到包时，CMake会打印一些信息，指定该选项时会禁止掉这些打印。例外是当同时指定QUIET时，如果找不到包，仍然会输出错误信息并终止执行过程。
* **REQUIRED**：当未找到满足条件的包（例如版本号不匹配，或指定组件未找到等），会终止CMake的执行过程，并输出一条错误信息。如果未指定该选项，即使未找到满足条件的包，CMake的执行过程也会继续。
* **COMPONENTS**：指定要查找的组件。通常一个包可能包含多个组件（可以理解为多个库，例如把C++的std看成一个包的概念，那么vector就是std下的其中一个组件），我们的工程可能会依赖包下的具体某个组件，因此可以通过这个选项来检测这些组件是否存在。通常的约定是，该选项后的组件应该都找到时才认为包找到，否则认为未找到满足条件的包。这个约束会依赖包的.cmake来实现，通过find_package命令传入的COMPONENTS可以通过
