### [CMake 单元测试工具](#)

**介绍**: CMake 提供了ctest来提供单元测试功能，它可以和gtest配合使用！

-----





```cmake
# 启动单元测试
enable_testing()
# 进入单元测试 子目录
add_subdirectory(tests)

#添加一个单元测试文件
add_executable(serializerUnit BinarySerializerUnit.cpp)

#添加一个 test
add_test(
    NAME "serializerUnit"
    COMMAND serializerUnit
)
```





```cmake
add_test(NAME <name> COMMAND <command> [<arg>...]
         [CONFIGURATIONS <config>...]
         [WORKING_DIRECTORY <dir>]
         [COMMAND_EXPAND_LISTS])
```

* arg 属于程序 main函数的参数

参数传递





成功失败判断方法：

* main函数返回值 0 成功
* PASS_REGULAR_EXPRESSION
* FAIL_REGULAR_EXPRESSION



**ctest -C Debug** 说明测试版本

```cmake
file(WRITE test_ctest.cpp [=[
#include <iostream>
using namespace std;
int main(int argc,char *argv[])
{
    if(argc<=1) //没有传递参数
        return -1;
    cout<<argv[1]<<endl;
    return 0;
}
]=])
add_executable(test_ctest test_ctest.cpp)

# 添加一个测试用例
add_test(NAME test_return_-1   # 测试的名称，用于控制台输出显示
COMMAND test_ctest 
)

add_test(NAME test_return_0   # 测试的名称，用于控制台输出显示
COMMAND test_ctest test_return_0
)

add_test(NAME test_success # 检测控制台输出
 COMMAND test_ctest success
) 
set_tests_properties(test_success
PROPERTIES 
PASS_REGULAR_EXPRESSION  success  #成功的字符串匹配正则
)
add_test(NAME test_failed # 检测控制台输出
 COMMAND test_ctest failed 
) 
set_tests_properties( test_failed
PROPERTIES 
FAIL_REGULAR_EXPRESSION  failed  #失败的字符串匹配正则
)

enable_testing()
```



#### execute_process