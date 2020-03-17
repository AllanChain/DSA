# 农夫过河问题的深度优先搜索求解上机报告

## 问题理解

农夫过河问题，其实就是通与不通、下一步走法有特殊规律的迷宫问题。可以结合书本迷宫问题和农夫过河问题给出解答。

## 设计和实现

看起来深度优先需要用栈来解决，但受到书本用 `route` 数组记录前驱的启发，可以把该数组当成一张迷宫地图，并制作成一个数组中引用实现的栈。事先计算好安不安全（通与不通），分别以 -1 和 -2 记录。每走一步就记录当前位置的前驱（压入），每退一步就读取前驱设为当前位置（弹出），但不修改 `route`，这样就自动做了标记。最后从终点依次读取前驱即可。

## 编程环境

|Key      |Value       |Version|
|:--------|:-----------|:------|
|System   |WSL Ubuntu  | 19.10 |
|Language |C           |       |
|Compiler |g++         | 9.2.1 |
|Debugger |gdb         | 8.3   |
|Formatter|clang-format| 9.0.0 |
|Editor   |VS Code     | 1.43.0|

## 上机问题与解决

### `sizeof`: type name not allowed

错误部分如下：

```c
stack = (LinkStack *)malloc(sizeof LinkStack);
```

解决与 `struct` 类似，加上括号即可：

```c
stack = (LinkStack *)malloc(sizeof(LinkStack));
```

但是为什么这里加一个括号会有影响呢？有点反人类啊，好比跟我说 `a * b` 跟 `a * (b)` 不一样。

<https://stackoverflow.com/questions/12924694>

说这是一个特定的用法。。不跟括号是计算表达式，跟括号是计算类型。

是我菜了。

### C 里怎么快乐地使用布尔类型？

<https://stackoverflow.com/a/1921557/8810271>

```c
#include <stdbool.h>
```

即可

### 什么时候用 `short` 呢？

之前因为比较抠门，很多变量使用了 `short` 类型。但是看到 <https://stackoverflow.com/a/24371078/8810271>，说 `int` 是一个比较正常的大小，计算机处理比较高效，甚至 `short` 不一定比 `int` 占用更少字节。。（虽然我这个平台上的确少一半字节）

果断改成 `int`，就算性能没提升多少，遵守一下约定也是好的。

### DRY!

看到书上的示例代码中，判断农夫、狼、羊、菜定义了四个函数，还在判断农夫和可能移动的物体是否在河的同一岸又重复写，非常难受！

果断定义一个函数 `get(int location, int item)`，再定义四个常量解决。
