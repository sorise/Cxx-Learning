### [find_library](#)
find_library是一个CMake命令，用于查找指定的库文件。它的使用方式如下：

```cmake
find_library(<VAR> name
    [PATHS path1 path2 ...]
    [DOC "docstring"]
    [NO_DEFAULT_PATH]
    [NO_CMAKE_FIND_ROOT_PATH]
    [NO_CMAKE_SYSTEM_PATH]
    [CMAKE_FIND_ROOT_PATH_BOTH | CMAKE_FIND_ROOT_PATH_FIRST | CMAKE_FIND_ROOT_PATH_LAST]
    [CMAKE_FRAMEWORK_PATH | CMAKE_APPBUNDLE_PATH | CMAKE_LIBRARY_PATH_FLAG | CMAKE_PROGRAM_PATH_FLAG]
    [HINTS path1 path2 ...]
    [PATH_SUFFIXES suffix1 suffix2 ...]
    [VERSION version]
    [REQUIRED]
    [QUIET]
)
```
**参数说明**:
* **\<VAR\>**：指定变量的名称，在查找过程中如果找到库文件，则该变量会被设置为库文件的绝对路径。如果没找到库，结果将为\<VAR\>-NOTFOUND
* **name**：要查找的库文件的名称。可以是带扩展名的完整文件名，也可以是不带扩展名的名字。
* **PATHS**：指定在哪些路径下进行查找。可以提供多个路径，以空格分隔。
* **DOC**：可选参数，用于提供关于查找的文档字符串。
* **NO_DEFAULT_PATH**：禁止查找默认路径。
* **NO_CMAKE_FIND_ROOT_PATH**：禁止在CMAKE_FIND_ROOT_PATH路径下查找。
* **NO_CMAKE_SYSTEM_PATH**：禁止在CMAKE_SYSTEM_PATH路径下查找。
* **CMAKE_FIND_ROOT_PATH_BOTH / CMAKE_FIND_ROOT_PATH_FIRST / CMAKE_FIND_ROOT_PATH_LAST**：指定查找路径的优先级。
* **CMAKE_FRAMEWORK_PATH / CMAKE_APPBUNDLE_PATH / CMAKE_LIBRARY_PATH_FLAG / CMAKE_PROGRAM_PATH_FLAG**：指定额外的路径类型。
* **HINTS**：提供额外的查找路径的提示。
* **PATH_SUFFIXES**：指定库文件的后缀名。可以提供多个后缀名，以空格分隔。
* **VERSION**：指定要查找的库文件的版本号。
* **REQUIRED**：如果设置了此选项，则库文件必须被找到，否则会报错。
* **QUIET**：禁止在查找过程中输出详细信息。


```cmake
find_library(LIB_MYLIB mylib
    PATHS /path/to/lib
    PATH_SUFFIXES debug release
    VERSION 1.0
    REQUIRED
)
```

上述示例表示在/path/to/lib路径下查找名为mylib的库文件，查找的优先级是先查找debug后缀，再查找release后缀，要求版本号为1.0，并且必须找到该库文件，否则会报错。如果找到该库文件，则将其路径保存在变量LIB_MYLIB中。

使用find_library可以方便地在CMake工程中查找特定的库文件路径，并将其路径保存在变量中，方便后续的库文件链接操作。