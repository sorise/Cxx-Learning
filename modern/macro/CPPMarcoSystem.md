### [各种操作系统的宏定义和检测方法](#)


**系统宏定义**：
* **UNIX**   `_unix`  `_unix_`
* **Linux**   `_linux` `_linux_`
* **Windows32**   `_WIN32`
* **Windows64**  `_WIN64`
* **IOS**        `__APPLE__`
* **android**    `__ANDROID__`


**编译器宏定义**:
* GCC           `__GNUC__`
* Visual C++   `_MSC_VER`
* Borland C++  `__BORLANDC__`

Unix风格的操作系统
```cpp
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
    /* UNIX-style OS. ------------------------------------------- */
#endif
```

一旦你知道它是Unix，你可以找到它是POSIX和POSIX版本：
```cpp
#include <unistd.h>
#if defined(_POSIX_VERSION)
    /* POSIX compliant */
#endif
```
BSD派生系统

```cpp
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    #include <sys/param.h>
    #if defined(BSD)
        /* BSD (DragonFly BSD, FreeBSD, OpenBSD, NetBSD). ----------- */

    #endif
#endif
```

Linux
```cpp
#if defined(__linux__)
    /* Linux  */
#endif
```


Apple操作系统
```cpp
#if defined(__APPLE__) && defined(__MACH__)
    /* Apple OSX and iOS (Darwin) */
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        /* iOS in Xcode simulator */
    #elif TARGET_OS_IPHONE == 1
        /* iOS on iPhone, iPad, etc. */    
    #elif TARGET_OS_MAC == 1
        /* OS X */
    #endif
#endif
```


Windows与Cygwin
```cpp
#if defined(__CYGWIN__) && !defined(_WIN32)
    /* Cygwin POSIX under Microsoft Windows. */
#endif
```

非POSIX Windows
```cpp
#if defined(_WIN64)
    /* Microsoft Windows (64-bit) */
#elif defined(_WIN32)
    /* Microsoft Windows (32-bit) */
#endif
```