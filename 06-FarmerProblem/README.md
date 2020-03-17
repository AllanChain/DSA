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

说这是一个特定的用法。。不跟括号是计算表达式，跟括号是计算类型。。
