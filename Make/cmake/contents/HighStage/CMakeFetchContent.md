## [FetchContent](#)
CMake从3.11版本以后提供的功能，FetchContent 提供了一种简洁的方法来管理依赖项的获取和构建过程，它通常用于从远程源代码管理系统（如**GitHub**、**GitLab**等）拉取源代码，并将其包含到您的 CMake 构建过程中。

**特点**

FetchContent模块具有以下显著特点：
- **即时下载**：依赖在CMake配置阶段下载，意味着不需要预先下载或维护依赖的副本，从而简化了项目的依赖管理。
- **无需子模块或子树**：与Git子模块或子树相比，FetchContent避免了将第三方代码直接纳入源代码管理系统的需要，减少了项目复杂性。
- **灵活的依赖管理**：可以根据项目需要，指定依赖的特定版本、分支或提交，确保了依赖的一致性和可追踪性。
- **简化的集成过程**：FetchContent自动处理下载和更新过程，开发者只需关注如何使用这些依赖。

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
        MyDependency   #要获取的依赖项的名称
        GIT_REPOSITORY "https://github.com/sorise/muse-timer.git" # Git仓库地址
        GIT_TAG master  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
        SOURCE_DIR dependency/MyDependency
)
```

(3). **获取并配置依赖**：使用 **FetchContent_MakeAvailable** 或者 **FetchContent_Populate** 加载并配置项目。**FetchContent_MakeAvailable** 是一个便捷函数，它会调用 **FetchContent_Populate** 并随后调用 **add_subdirectory** 来自动构建和链接依赖项目。

```cmake
FetchContent_MakeAvailable(MyDependency)
```

(4).**使用依赖**：当FetchContent完成项目拉取和配置后，你可以在自己的CMakeLists.txt中像对待任何其他内部子目录一样使用该依赖提供的目标和变量。

```cmake
target_link_libraries(MyTarget PRIVATE MyDependency::MyDependencyTarget)
```

(5).**头文件目录**: **FetchContent_Declare**命令会生成一些变量来描述所声明的内容（content）。这些变量包括：

1. `<content_name>_POPULATED`: 一个布尔值，指示内容是否已经被拉取并配置。
2. `<content_name>_SOURCE_DIR`: 指向拉取内容的源码目录的路径。
3. `<content_name>_BINARY_DIR`: 指向拉取内容的二进制目录的路径。

这些变量的命名规则为`<content_name>`加上特定后缀，以便在处理多个内容时区分不同的内容。你可以使用这些变量来引用拉取内容的各种属性，如源码目录路径、二进制目录路径等。



### 2. 注意事项

当你使用`FetchContent`模块拉取依赖项时，默认情况下，它们将被存储在你的CMake构建目录下的一个名为`_deps`的子目录中。这个目录通常位于你的构建目录的根目录下。

例如，如果你的CMake项目的构建目录是`/path/to/your_project/build`，那么拉取的依赖项将存放在`/path/to/your_project/build/_deps`目录下。在这个`_deps`目录中，每个被拉取的依赖项将被存储在一个单独的子目录中，其名称通常是该依赖项的Git仓库的名称或者由`FetchContent_Declare`命令中的`<content_name>`指定的名称。

### 3. 相关命令

#### 3.1 FetchContent_Populate

`FetchContent_Populate` 函数是 FetchContent 模块中的一个函数，用于将通过 FetchContent 获取的外部依赖项的源代码解压到指定的目录中。通常，您在使用 `FetchContent` 模块时，会在下载依赖项后使用 `FetchContent_Populate` 函数来将其解压到指定目录中。

```cmake
FetchContent_Populate(
  <name>
  [QUIET]
  [SUBBUILD_DIR <subBuildDir>]
  [SOURCE_DIR <srcDir>]
  [BINARY_DIR <binDir>]
  ...
)
```

- `<name>`: 通过 `FetchContent_Declare` 声明的依赖项的名称，用于指定要解压的依赖项。
- `[SOURCE_DIR <dir>]`: 可选参数，用于指定要将依赖项解压到的目录。如果未提供此参数，则默认将依赖项解压到当前的二进制目录下的 `<name>-prefix` 子目录中。

构建所声明的 name 依赖， 该命令会定义三个变量：
1. **\<lowercaseName\>_POPULATED** : 依赖是否已被构建
2. **\<lowercaseName\>_SOURCE_DIR** : 依赖存储路径
3. **\<lowercaseName\>_BINARY_DIR** : 依赖 Build 路径

```cmake
cmake_minimum_required(VERSION 3.11)

# 引入FetchContent模块
include(FetchContent)

# 声明要通过FetchContent获取的外部项目
FetchContent_Declare(
    DependencyName # 内部使用的项目名称
    GIT_REPOSITORY https://github.com/author/repo.git # Git仓库地址
    GIT_TAG v1.0.0 # 指定要检出的标签或分支
)

# 执行下载、提取操作并将项目准备好进行构建
FetchContent_Populate(DependencyName)

# 此时，源代码已经被下载并解压到了构建目录下，可通过以下变量访问：
# DependencyName_SOURCE_DIR - 包含源代码的目录
# DependencyName_BINARY_DIR - 应该用于放置生成文件的目录

# 现在可以添加子目录，以便CMake处理这个依赖项的构建
add_subdirectory(${DependencyName_SOURCE_DIR} ${DependencyName_BINARY_DIR})

# 如果外部项目提供了CMake配置文件，它会被自动处理，并且可以使用它导出的目标、库等
target_link_libraries(MyTarget PRIVATE DependencyName::ExportedTarget)
```
FetchContent_GetProperties(\<name\> [SOURCE_DIR \<srcDirVar\>] [BINARY_DIR \<binDirVar\>] [POPULATED \<doneVar\>]): 获取与外部依赖 name 相关的属性。

在CMake的构建过程中，`_deps`目录下的`src`、`build`和`subbuild`等子目录是`FetchContent`模块在拉取和构建依赖项时使用的临时目录。这些子目录的作用如下：

1. `src`目录：存放从远程仓库拉取下来的依赖项的源代码。
2. `build`目录：用于构建依赖项时生成的临时构建文件。
3. `subbuild`目录：用于支持多配置生成器（如Visual Studio）的子目录。在这些配置中，构建需要分开进行以支持不同的生成配置（如Debug和Release）。

这些临时目录的存在是为了支持`FetchContent`模块的工作流程，使得它能够有效地管理和构建依赖项。在构建完成后，你通常不需要关心这些临时目录，因为`FetchContent`会自动处理它们。



### 4. 变量

**FETCHCONTENT_FULLY_DISCONNECTED**

当这个变量被设置为 `ON` 时，CMake 将不会尝试从远程仓库更新依赖项。这意味着依赖项将会保持在本地，并且不会检查是否有新的更新。这个设置可以减少网络开销，并确保构建过程的稳定性，但是可能会导致你的依赖项不会及时更新到最新版本。

### 5. Visual Studio 的问题

**visual studio** 在使用 FetchContent由一些很奇怪的问题，比如目录路径问题。

在visual studio中使用cmake，打开测试以后，无法找到被测试库，因为被测试库所在目录为build/Debug，而生成的测试可执行文件在 build/tests/Debug中。如何解决visual studio这一bug