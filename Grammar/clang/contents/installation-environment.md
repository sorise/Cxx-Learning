## C语言 编程环境
**介绍**： 初学者如果并不想配置那么麻烦的东西，可以直接下载 Devc++ 或者 CodeBlock 基本上什么都不用做只需要下一步安装就可以了，老鸟不用看，会使用CMake、XMake不用看。

----

- [`点击下载：CodeBlock`](http://www.codeblocks.org/downloads)
- [`点击下载：Devc++`](https://sourceforge.net/projects/orwelldevcpp/) `推荐使用`


-----
一下内容推荐计算机专业大三学完编译原理以后再看。


### VsCode 搭建编程环境
宇宙级编辑器，非常之流行，使用此可以得到很好的代码提示等等效果，下面我们来搭建一个这样的环境。

### 1. 第一步插件
1. `C/C++`: 写c/c++ 必安插件 具有智能提示，代码Debug等功能
2. `Code Runner`: 帮你快速运行c代码。

### 2. 编译环境
仅仅使用vscode 和插件是无法编译代码的，所以我们需要一个编译器，gcc来帮我们编译c语言程序 可以使用mingw-w64、MSYS2、Cygwin，请百度安装 `mingw`。

### [3.gcc的基本用法](#)
* `gcc [options] [filenames]` : 其中，`options` 就是编译器所需要的参数，`filenames` 给出相关的文件名称，最常用的有以下参数：
  * `-c` 只编译，不链接成为可执行文件,编译器只是由输入的 .c 等源代码文件生成 .o 为后缀的目标文件，通常用于编译不包含主程序的子程序文件。
  * `-o` `output_filename 确定输出文件的名称为output_filename。同时这个名称不能和源文件同名。
        如果不给出这个选项，gcc就给出默认的可执行文件 a.out 。
  * `-g` 产生符号调试工具（GNU的 gdb）所必要的符号信息。想要对源代码进行调试，就必须加入这个选项。
  * `-S` 把文件编译成为汇编源文件，如下，执行完后生成test.s
  * `-E` gcc -E source_file.c 只执行到预编译。直接输出预编译结果。
  * `-x` 设定文件所使用的语言, 使后缀名无效, 对以后的多个有效 gcc -x c hello.pig 
  * `-fexec-charset=gbk`: 在运行的时候无法正确输出中文,因为gcc在编译的时候默认不支持中文的，所以在编译的时候要添加一个选项。
### 4.命令行运行c
* `1. 编译c `： `gcc test.c -o test`: `只是编译而已 会产生可执行文件，但是不会执行`
* `2. 运行可执行文件` :` .exe 程序  start test.exe `

`例如 Devc++ 就是帮我们一下执行上面两步,生成可执行文件，并且运行它`

#### 4.1 在linux 里面运行 c语言
1. `编程c程序`  `vi test.c`
```c
#include "stdio.h"

int main(int argc, char const *argv[])
{
   printf("hello World \n");
}
```
2. `退出保存` `gcc test.c -o test 编译成可执行文件那么 linux怎么执行 c呢？`

```shell
./test  //就可以执行了
```

### [important: vscode 调试C](#)

* 打开vs code 点击左边第五个icon搜索C/C++ extension下载,并重启。
* Ctrl+Shift+P 选择 `Tasks: Configure Task<任务:(配置任务)>`
* 选择使用模板创建task.json文件`
* **`请注意文件目录不能使用中文，否则会报错文件路径不存在`**
* 选择other,进行如下设置

`文件目录`:`vscode文件夹 和 main.c 在一个目录下`

```
vscode
     |
     --launch.json
     |
     --task.json
main.c
main.exe
```
首先新建 `tasks.json` 内容为:
```json
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build main",
            "type": "shell",
            "command": "gcc",
            "args":[
                "${fileDirname}\\${fileBasenameNoExtension}.c",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe",
                "-g",
            ]
        }
    ]
}
```

* 切换到C程序文件处,选择顶部状态栏 `----->` 调试
* 选择 `C++ (GDB/LLDB)` ，进行如下设置`

`launch.json`
```node
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "gcc.exe build and debug active file",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}\\${fileBasenameNoExtension}.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            /* 如果未 false 那么在vscode的控制台里面调试 不打开 黑窗口  true 使用黑窗口调试 */
            "MIMode": "gdb",
            "miDebuggerPath": "C:/TDM-GCC-32/bin/gdb32.exe", /* 注意位置 还有 64位GCC 直接使 gdb.exe */
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build main" /* 注意 这个名称要和 tasks.json 里面的 label 一样*/
        }
    ]
}
```

* miDebuggerPath: 你的 gcc 程序的安装位置
* program 调试文件路径 要和 tasks.json 里面的一样

##### vscode 各种预定义的变量
```node
${workspaceFolder} :表示当前workspace文件夹路径，也即/home/Coding/Test

${workspaceRootFolderName}:表示workspace的文件夹名，也即Test

${file}:文件自身的绝对路径，也即/home/Coding/Test/.vscode/tasks.json

${relativeFile}:文件在workspace中的路径，也即.vscode/tasks.json

${fileBasenameNoExtension}:当前文件的文件名，不带后缀，也即tasks

${fileBasename}:当前文件的文件名，tasks.json

${fileDirname}:文件所在的文件夹路径，也即/home/Coding/Test/.vscode

${fileExtname}:当前文件的后缀，也即.json

${lineNumber}:当前文件光标所在的行号
```