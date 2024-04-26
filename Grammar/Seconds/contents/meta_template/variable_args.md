## [元模板：可变参数模板](#)
**介绍**：`...` 的妙用。



```cpp
template<std::size_t... Index>
constexpr std::size_t test() {
    return (Index + ...);
}
```
