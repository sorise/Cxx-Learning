#####  [C语言 string.h](#top) <b id="top"></b> 
----
`一些常用的字符串函数 strlen() 就不说了太简单了`

- [x] [`1.strcspn()`](#target1)
- [x] [`2.strchr() strrchr()`](#target2)
- [x] [`3.strcat()`](#target3)
- [x] [`4.strcpy()`](#target4)
- [x] [`5.strspn()`](#target5)
- [x] [`6.strpbrk()`](#target6)
 
---
##### [1.size_t strcspn(const char* str, const char* reject)](#top) <b id="target1"></b>
`C语言 size_t strcspn(const char* str, const char* reject) 函数用来返回从字符串 str 开头算
起，连续有几个字符都不在 reject 中；也就是说，str 中连续有几个字符和 reject 没有交集。`

`参数说明：`
```
str：要检索的字符串。
reject：该字符串包含了要在 str 中进行匹配的字符列表。
```

`返回值`：`返回从字符串 str 开头算起，连续不在 reject 中的字符的个数；也可以理解为，str 中第一次出现 reject 中字符的位置。`

```c
#include <stdio.h>
#include <string.h>
int main(){
    char str[50] = { "http://c.biancheng.net" };
    char keys[50] = { "?.,:\"\'-!" };
    int i = strcspn(str, keys);
    printf("The firsr punctuation in str is at position %d.\n", i);
    return 0;
}
```
`The firsr punctuation in str is at position 4.`

##### [2.char* strchr(const char* str, int c)](#top) <b id="target2"></b>
`C语言 strchr() 函数用于查找给定字符串中某一个特定字符。`

`参数说明：`
```
str：被查找的字符串。
c：要查找的字符。
```

`【实例】使用C语言 strchr() 函数在网址中查找j字符。 `
```c
#include <stdio.h>
#include <string.h>
int main(){
    const char *str = "http://ester.jxkikeris.you.father.com";
    int c = 'j';
    char *p = strchr(str, c);
    puts(p);
    if (p) {
        puts("Found");
    } else {
        puts("Not found");
    }
    return 0;
}
/*
jxkiker.is.jake.jather.com
Found
*/
```
##### char* strrchr(const char* str, int c)
`完全相同只是 strchr 是从前往后寻找,strrchr 是从后往前寻找`

##### [3.char* strcat(char* strDestination, const char* strSource)](#top)  <b id="target3"></b>
`strcat() 函数用来将两个字符串连接（拼接）起来。`

```c
#include <stdio.h>
#include <string.h>
int main(){
    char str1[101] = { 0 };
    char str2[50] = { 0 };
    gets(str1);
    gets(str2);
    strcat(str1, str2);
    puts(str1);
    return 0;
}
```


##### [4.char* strcpy(char* strDestination, const char* strSource)](#top)   <b id="target4"></b>
`strcpy() 函数用于对字符串进行复制（拷贝）。`

`参数说明：`
```
strDestination：目的字符串。
strSource：源字符串。
```

```c
#include <stdio.h>
#include <string.h>
int main(){
    char dest[50] = { 0 };
    char src[50] = { "jxiickersdads" };
    strcpy(dest, src);
    puts(dest);
    return 0;
}
//jxiickersdads
```

##### [5.size_t strspn(const char *s, const char *accept)](#top) <b id="target5"></b>
`从上面的示例代码中可以看出，strspn 函数从字符串参数 s 的开头计算连续的字符，而这些字符完全
是 accept 所指字符串中的字符。简单地说，如果 strspn 函数返回的数值为 n，则代表字符串 s 开头连续有 
n 个字符都属于字符串 accept 内的字符。`

```c
int main(void)
{
    char str[] = "I welcome any ideas from readers， of course.";
    printf("I wel：%d\n",strspn(str,"I wel"));
    printf("Iwel：%d\n",strspn(str,"Iwel"));
    printf("welcome：%d\n",strspn(str,"welcome"));
    printf("5：%d\n",strspn(str,"5"));
    return 0;
}
/*
I wel：5
Iwel：1
welcome：0
5：0
*/
```

##### [6.char *strpbrk(const char *s1,const char *s2)](#top) <b id="target6"></b>
`如果要查找多个字符，需要使用 strpbrk 函数。该函数在源字符串（s1）中按从前到后顺序找出最先含有搜索字符串（s2）中任一字符的位
置并返回，空字符 null('\0') 不包括在内，若找不到则返回空指针。其函数原型的一般格式如下：`

```c
#include <stdio.h>
#include <string.h>
int main(void)
{
    char str[] = "I welcome any ideas from readers, of course.";
    char *rc=strpbrk(str,"kkk");//（返回NULL）
    char *va=strpbrk(str,"ideas");//elcome any ideas from readers, of course.
    printf("%s\n",rc);
    printf("%s\n",va);
    return 0;
}
```
