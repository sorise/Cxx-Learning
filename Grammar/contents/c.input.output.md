### [c 语言输入输出](#) 
非常简单的,几个小函数的使用罢了

------

- [x] [1.输出函数](#target1)
- [x] [2.输入函数](#target2)
- [x] [3.Sleep(ms)](#target3)
- [x] [4.屏幕的任意位置输出字符](#target4)

------

### [1.输出函数](#top) <b id="target1"></b>
在C语言中，有三个函数可以用来在显示器上输出数据。
* puts()：只能输出字符串，并且输出结束后会自动换行。
* putchar()：函数把字符输出到屏幕上，并返回相同的字符。这个函数在同一个时间内只会输出一个单一的字符。您可以在循环内使用这个方法，以便在屏幕上输出多个字符。
* printf()：可以输出各种类型的数据，在前面的很多章节中都进行了介绍。` `stdio.h 是一个头文件 (标准输入输出头文件) and #include 是一个预处理命令，用来引入头文件。 当编译器遇到 printf() 函数时，如果没有找到 stdio.h 头文件，会发生编译错误。

```c
int puts( const char *string );

//用返回值判断puts函数是否处理正常，非负数是正常。 

int putchar(int c) 

int printf(const char *format, ...) 

//函数把输出写入到标准输出流 stdout ，并根据提供的格式产生输出。
//format 可以是一个简单的常量字符串，但是您可以分别指定 %s、%d、%c、%f
//等来输出或读取字符串、整数、字符或浮点数。还有许多其他可用的格式选项

```

* `printf() 格式控制符的完整形式如下：  %[flag][width][.precision]type`
  * `type 表示输出类型，比如 %d、%f、%c、%lf，type 就分别对应 d、f、c、lf；再如，%-9d中 type 对应 d。type 这一项必须有，这意味着输出时必须要知道是什么类型。`
  * `width 表示最小输出宽度，也就是至少占用几个字符的位置；例如，%-9d中 width 对应 9，表示输出结果最少占用 9 个字符的宽度。`
  * `.precision 表示输出精度，也就是小数的位数。 `
  * `flag 是标志字符。例如，%#x中 flag 对应 #，%-9d中 flags 对应-。下表列出了 printf() 可以用的 flag：`
  
|`控制符号`|`说明`|
|---|---|
|`%c`|`输出一个单一的字符`|
|`%hd、%d、%ld`|`以十进制、有符号的形式输出 short、int、long 类型的整数`|
|`%hu、%u、%lu`|`以十进制、无符号的形式输出 short、int、long 类型的整数`|
|`%ho、%o、%lo`|`以八进制、不带前缀、无符号的形式输出 short、int、long 类型的整数`|
|`%#ho、%#o、%#lo`|`以八进制、带前缀、无符号的形式输出 short、int、long 类型的整数`|
|`%hx、%x、%lx %hX、%X、%lX`|`以十六进制、不带前缀、无符号的形式输出 short、int、long 类型的整数。如果 x 小写，那么输出的十六进制数字也小写；如果 X 大写，那么输出的十六进制数字也大写。`|
|`%s`|`输出一个字符串`|
|`%p`|`C语言中有一个控制符%p，专门用来以十六进制形式输出地址，不过 %p 的输出格式并不统一，有的编译器带0x前缀，有的不带。`|
|`%e、%le %E、%lE`|`以指数的形式输出 float、double 类型的小数。如果 e 小写，那么输出结果中的 e 也小写；如果 E 大写，那么输出结果中的 E 也大写。`|
|`%g、%lg %G、%lG`|`以十进制和指数中较短的形式输出 float、double 类型的小数，并且小数部分的最后不会添加多余的 0。如果 g 小写，那么当以指数形式输出时 e 也小写；如果 G 大写，那么当以指数形式输出时 E 也大写。`|
|`%f、%lf `|`以十进制的形式输出 float、double 类型的小数`| 
|`%#hx、%#x、%#lx %#hX、%#X、%#lX`|`以十六进制、带前缀、无符号的形式输出 short、int、long 类型的整数。如果 x 小写，那么输出的十六进制数字和前缀都小写；如果 X 大写，那么输出的十六进制数字和前缀都大写。`|


|`标志`| `含  义`|
|:---|:----|
|`-`|`-表示左对齐。如果没有，就按照默认的对齐方式，默认一般为右对齐。`|
|`+`|`用于整数或者小数，表示输出符号（正负号）。如果没有，那么只有负数才会输出符号。`|
|`空格`|`用于整数或者小数，输出值为正时冠以空格，为负时冠以负号。`|
|`#`|`对于八进制（%o）和十六进制（%x / %X）整数，# 表示在输出时添加前缀；八进制的前缀是 0，十六进制的前缀是 0x / 0X。对于小数（%f / %e / %g），# 表示强迫输出小数点。如果没有小数部分，默认是不输出小数点的，加上 # 以后，即使没有小数部分也会带上小数点。`|

`假如现在老师要我们输出一个 4×4 的整数矩阵，为了增强阅读性，数字要对齐，怎么办呢？我们显然可以这样做`
```c
#include <stdio.h>
int main()
{
    int a1=20, a2=345, a3=700, a4=22;
    int b1=56720, b2=9999, b3=20098, b4=2;
    int c1=233, c2=205, c3=1, c4=6666;
    int d1=34, d2=0, d3=23, d4=23006783;

    printf("%-9d %-9d %-9d %-9d\n", a1, a2, a3, a4);
    printf("%-9d %-9d %-9d %-9d\n", b1, b2, b3, b4);
    printf("%-9d %-9d %-9d %-9d\n", c1, c2, c3, c4);
    printf("%-9d %-9d %-9d %-9d\n", d1, d2, d3, d4);

    return 0;
}
//%-9d中，d表示以十进制输出，9表示最少占9个字符的宽度，宽度不足以空格补齐，-表示左对齐
```

```c
#include <stdio.h>
int main(){
    int m = 192, n = -943;
    float f = 84.342;
    printf("m=%10d, m=%-10d\n", m, m);  //演示 - 的用法
    printf("m=%+d, n=%+d\n", m, n);  //演示 + 的用法
    printf("m=% d, n=% d\n", m, n);  //演示空格的用法
    printf("f=%.0f, f=%#.0f\n", f, f);  //演示#的用法

    return 0;
}
```
```c
//结果
m=       192, m=192      
m=+192, n=-943
m= 192, n=-943
f=84, f=84.
```


```c
#include "stdio.h"

int main(int argc, char const *argv[])
{
    printf("%e \n",56321.1254);
    printf("%E \n",56321.1254);
    //Sleep(5000);
    printf("%-+8.3f \n",985.6321655);
    
    putchar('A');
    putchar(26); //
    putchar(100);//d
    //getchar();//等待输入
    return 0;
}
```
##### puts(str)
```c
#include <stdio.h>
int main()
{
    puts(
        "asdasd。"
        "asdasdasdasd"
        "asdasdssssssssssssssss"
    );
    return 0;
}
```
`在 puts 函数中，可以将一个较长的字符串分割成几个较短的字符串，这样会使得长文本的格式更加整齐。`

##### 关于 %%

`%% 才能输出百分号哦 一个%什么也不会输出`
```c
printf("%"); //
printf("%%"); //%
```


##### [2.输入函数](#top) <b id="target2"></b>
* `scanf()：和 printf() 类似，scanf() 可以输入多种类型的数据。`
* `getchar()、getche()、getch()：这三个函数都用于输入单个字符。`
* `gets()：获取一行数据，并作为字符串处理。`

`scanf 是 scan format 的缩写，意思是格式化扫描，也就是从键盘获得用户输入，和 printf 的功能正好相反。`

`对于 scanf()，输入数据的格式要和控制字符串的格式保持一致。`

`scanf() 不会跳过不符合要求的数据，遇到不符合要求的数据会读取失败，而不是再继续等待用户输入。`

```c
#include "stdio.h"

int main(int argc, char const *argv[])
{
    /* code */
    int age;
    float height,score;
    char introduce[20] = {0};

    printf("please input your age: \n");
    scanf("%d", &age);
    printf("you age:%d \n",age);
    printf("please input your score: \n");
    scanf("%f %f", &score, &height);
    printf("your score:%.2f and height:%.2f \n", score, height);
    printf("introduce: \n");
    scanf("%s", &introduce);
    printf("introduce: %s \n", introduce);
    getchar();
    return 0;
}
```

##### scanf 连续输入
`秘诀在于缓冲区 我们从键盘输入的数据并没有直接交给 scanf()，而是放入了缓冲区中，直到我们按下回车键，scanf() 才到缓冲区中读取数据。如果缓冲区中的数据符合 scanf() 的要求，那么就读取结束；如果不符合要求，那么就继续等待用户输入，或者干脆读取失败。`

```c
scanf("%d, %d, %d", &a, &b, &c);
printf("a+b+c=%d\n", a+b+c);
```

`由于缓冲区的存在，才使得我们能够多输入一些数据，或者一次性输入所有数据，这可以认为是缓冲区的一点优势。然而，缓冲区也带来了一定的负面影响，甚至会导致很奇怪的行为`

```c
#include "stdio.h"

int main(int argc, char const *argv[])
{
    int age, count;
    scanf("age=%d", &age);
    scanf("count=%d", &count);

    printf("age: %d count: %d", age, count);
    getchar();getchar();
    return 0;
}
```
`输入样例:`
```c
age=99 //一回车
a=99, b=27 //直接输出了
```
`按下回车键，程序竟然运行结束了，只有第一个 scanf() 成功读取了数据，第二个 scanf() 仿佛没有执行一样，根本没有给用户任何机会去输入数据`

`这样输出就没有错`
```c
age=58count=42
age: 58 count: 42
```
`但是 age=58 count=42 中间加一个空格，又会错误`


|`格式控制符`	|`说明`|
|:----|:----|
|`%c`|`	读取一个单一的字符`|
|`%hd、%d、%ld`|`读取一个十进制整数，并分别赋值给 short、int、long 类型`|
|`%ho、%o、%lo`|`读取一个八进制整数（可带前缀也可不带），并分别赋值给 short、int、long 类型`|
|`%hx、%x、%lx`|`读取一个十六进制整数（可带前缀也可不带），并分别赋值给 short、int、long 类型`|
|`%hu、%u、%lu`	|`读取一个无符号整数，并分别赋值给 unsigned short、unsigned int、unsigned long 类型`|
|`%f、%lf`|	`读取一个十进制形式的小数，并分别赋值给 float、double 类型`|
|`%e、%le	`|`读取一个指数形式的小数，并分别赋值给 float、double 类型`|
|`%g、%lg`|	`既可以读取一个十进制形式的小数，也可以读取一个指数形式的小数，并分别赋值给 float、double 类型`|
|`%s`|	`读取一个字符串（以空白符为结束）`|

##### 指定输入的长度
```c
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{   
    int x,y,z;
    scanf("%2d%4d%d",&x,&y,&z);
    printf("x = %d, y = %d, z = %d\n",x,y,z); 
    system("pause");
    return 0;
}
/*
输入:1234567  ---> x = 12 y = 3456 z = 7
*/
```

##### getchar()
`最容易理解的字符输入函数是 getchar()，它就是scanf("%c", c)的替代品，除了更加简洁，没有其它优势了；或者说，getchar() 就是 scanf() 的一个简化版本。`
```c
#include <stdio.h>
int main()
{
    char c;
    c = getchar();
    printf("c: %c\n", c);
    return 0;
}
```
##### getche()
`getche() 就比较有意思了，它没有缓冲区，输入一个字符后会立即读取，不用等待用户按下回车键，这是它和 scanf()、getchar() 的最大区别。请看下面的代码：`

```c
#include <stdio.h>
#include <conio.h>
int main()
{
    char c = getche();
    printf("c: %c\n", c);
    return 0;
}
```
#####  getch()
`getch() 也没有缓冲区，输入一个字符后会立即读取，不用按下回车键，这一点和 getche() 相同。getch() 的特别之处是它没有回显，看不到输入的字符。所谓回显，就是在控制台上显示出用户输入的字符；没有回显，就不会显示用户输入的字符，就好像根本没有输入一样。`
```c
#include <stdio.h>
#include <conio.h>
int main()
{
    char c = getch();
    printf("c: %c\n", c);
    return 0;
}
```

##### [3.Sleep(ms)](#top) <b id="target3"></b>
`暂停函数Windows版本 引入  #include<Windows.h>`

```c
Sleep(5000); //程序暂停五秒 Windows 中
```
`Linux 版本 引入 #include<unistd.h>`
```c
sleep(5); //程序暂停五秒 
```

##### gets()
`gets() 是专用的字符串输入函数，与 scanf() 相比，gets() 的主要优势是可以读取含有空格的字符串。`

```c
#include <stdio.h>
int main()
{
    char author[30], lang[30], url[30];
    gets(author);
    printf("author: %s\n", author);
    gets(lang);
    printf("lang: %s\n", lang);
    gets(url);
    printf("url: %s\n", url);

    return 0;
}
```


|`函数`| `缓冲区`|	`头文件`|	`回显`|	`适用平台`|
|---|---|---|---|---|
|`getchar()`|	`有`|	`stdio.h`|	`有`|	`Windows、Linux、Mac OS 等所有平台`|
|`getche()`|	`无`	|`conio.h`|	`有`|	`Windows`|
|`getch()`|	`无`	|`conio.h`|`无`|	`Windows`|


##### [4.屏幕的任意位置输出字符](#top) <b id="target4"></b>

```c
#include<Windows.h>
#include "stdio.h"

void GPS(int x, int y)//定位光标位置
{
 COORD pos;
 HANDLE hOutput;
 pos.X = y * 2+10;
 pos.Y = x+3;
 hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleCursorPosition(hOutput, pos);
}

int main(int argc, char const *argv[])
{
 printf("%e \n",56321.1254);
 printf("%E \n",56321.1254);
 //Sleep(5000);
 printf("%-+8.3f \n",985.6321655);
 GPS(20,10);
 putchar('A');//看看这个A 输入到哪里去了
 putchar(26); //
 putchar(100);//d
 getchar();//等待输入
 return 0;
}
```

