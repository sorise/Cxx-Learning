### [C++ 类型转换工具](#)

##### [long to string](#)

```cpp
std::string long_to_string(long number){
    char buffer[128] = {'\0'};
    int len = 0;
    bool positive = true;
    if (number < 0) positive = false; 
    number = abs(number);
    if(number == 0) {
        buffer[len++] = '0';
    }
    while (number != 0)
    {
        int last = number % 10;
        char c = last + 48;
        buffer[len++] = c;
        number /= 10;
    }
    buffer[len] = '\0';
    printf("buffer: %s len: %d\n", buffer, len);
    std::string result = "";
    for (int i =  0; i < len; i++)
    {
        result.insert(0,1, buffer[i]);
    }
    if(!positive) result.insert(0,1, '-');
    return result;
}
```