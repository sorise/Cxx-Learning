### [CMake 命令及其参数](#)

**介绍**: CMake常用命令，包括设置缓存变量，构建系统、项目安装、编译器参数等等！

-----





### [1. CMake 常用命令](#)



|命令|解释|
|:----|:----|
|**cmake -S . -B build**|CMakeLists.txt在当前目录，使用外部构建，输出到build目录中。|
|**cmake --build build**|进行编译，编译目录是build。|
|**cmake --build build -v**|进行编译，编译目录是build, 可以看一些警告信息。|
|**cmake -S . -B build -G "Unix Makefiles"**|使用-G 指定生成器！|
|**cmake -S . -B build -D CMAKE_BUILD_TYPE=Release**|进行编译，编译目录是build，并且指示编译类型|
|**cmake --system-information information.txt**|关于系统的所有信息转储到屏幕或文件中。|
|**cmake -P my_script.cmake**|执行一个Cmake脚本！|



#### [2.1 CMake 常用参数](#)

**cmake命令的选项实在太多了，我们挑几个常用的学习下。**

* **-S** `<path-to-source>` 指定源码目录
* **-B** `<path-to-build>` 指定编译目录
* **-C** `<initial-cache>`  预加载一个脚本填充缓存文件。　　
* **-D** `<var>[:<type>]=<value>` 创建一个CMake的缓存条目，直白点说就是向cmake传递变量。
* **-Wdev**: 使能开发者警告信息输出功能,允许那些为CMakeLists.txt文件的作者准备的警告信息。
* **-Wno-dev**: 抑制开发者警告。抑制那些为CMakeLists.txt文件的作者准备的警告信息。
* **-U** `<globbing_expr>`: 从CMake的缓存文件中删除一条匹配的条目。
* **-G** : 指定一个makefile生成器,在具体的平台上，CMake可以支持多个原生的构建系统。makefile生成器的职责是生成特定的构建系统。可能的生成工具的名称将在生成工具一节给出。
* **-P <file>**: 处理脚本模式。将给定的cmake文件按照CMake语言编写的脚本进行处理。不执行配置和生成步骤，不修改缓存信息。如果要使用-D选项定义变量，-D选项必须在-P选项之前。

#### [2.2 CMake build参数](#)

|命令|解释|
|:----|:----|
|--build `<path>`|要生成的项目二进制目录。|
|-j `[<jobs>]`, --parallel `[<jobs>]`| 生成时要使用的最大并发进程数。如果省略了`＜jobs＞`，则使用本机构建工具的默认编号。 |



**-E**: CMake命令行模式。
为了真正做到与平台无关，CMake提供了一系列可以用于所有系统上的的命令。以-E参数运行CMake会帮助你获得这些命令的用法。可以使用的命令有：chdir, copy, copy_if_different copy_directory, compare_files, echo, echo_append, environment, make_directory, md5sum, remove_directory, remove, tar, time, touch, touch_nocreate, write_regv, delete_regv, comspec, create_symlink。

**-i**: 以向导模式运行CMake。

向导模式是在没有GUI时，交互式地运行cmake的模式。cmake会弹出一系列的提示，要求用户回答关于工程配置的一行问题。这些答复会被用来设置cmake的缓存值。

**--L[A][H]** : 列出缓存的变量中的非高级的变量。
-L选项会列出缓存变量会运行CMake，并列出所有CMake的内有被标记为INTERNAL或者ADVANCED的缓存变量。这会显示当前的CMake配置信息，然后你可以用-D选项改变这些选项。修改一些变量可能会引起更多的变量被创建出来。如果指定了A选项，那么命令也会显示高级变量。如果指定了H选项，那么命令会显示每个变量的帮助信息。

**--build** \<dir\>: 构建由CMake生成的工程的二进制树。相当于执行make。

**–graphviz=[file]**: 生成依赖的graphviz图。
生成一个graphviz软件的输入文件，其中包括了项目中所有库和可执行文件之间的依赖关系。

**–system-information [file]**: 输出与该系统相关的信息。
输出范围比较广的、与当前使用的系统有关的信息。如果在一个CMake工程的二进制构建树的顶端运行该命令，它还会打印一些附加信息，例如缓存，日志文件等等。

**–debug-output**: 将cmake设置为调试模式。
在cmake运行时，打印额外的信息；比如使用message(send_error)调用得到的栈跟踪信息。

**–trace**: 将cmake设置为跟踪模式。
用message(send_error )调用，打印所有调用生成的跟踪信息，以及这些调用发生的位置。（这句话含义不是很确定—译注。）


#### [2.3 CMake 安装参数](#)
