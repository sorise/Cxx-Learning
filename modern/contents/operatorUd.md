### [用户定义字面量](#)

**介绍**: 通过定义用户定义的后缀，允许整数、浮点数、字符及字符串字面量产生用户定义类型的对象。[https://zh.cppreference.com/w/cpp/language/user_literal](#)



```cpp
//将字符串指针传唤为字符串对象
std::string operator ""_r(const char* str, size_t size) {
	return {str};
}	
//返回字符串指针的长度
size_t operator ""_len(const char* str, size_t size) {
	return strlen(str);
}

int main()
{	
	std::cout << "asdasd"_len << std::endl; //6
	std::string name = "remix"_r;

	std::cout << name << std::endl;

}
```

