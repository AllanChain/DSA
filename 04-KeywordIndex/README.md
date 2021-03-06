# 词索引表建立上机报告

## 问题理解

词索引表是按照顺序存储关键词和书目，需要根据书名提取关键词。之所以使用顺序存储，主要是便于后期的二分查找。

## 设计与实现

数据结构上，基本与 PPT 的指引一致，建立索引结构 `IndexList`，关键词结构 `KeywordItem`，以及书号链表结构 `BookList`。每次输入后改为小写，与忽略词表进行比对。如果并非忽略词，则进行添加操作。

添加时，首先要调用 `binarySearch` 在已有关键词中进行二分查找，如果存在则返回位置 `p`，不存在则返回 `-p - 1`，其中 `p` 为应该插入的位置。然后创建节点。

以负数书号作为输入结束标志，并打印索引表。

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

### `short` 类型在 `format` 里怎么表示？

```c
"%hd"
```

### 如何从 `stdin` 读取一行带有空格的？

据说 `fflush` 会产生未定义的行为，而 `fgets` 可以很好的解决缓冲区的问题。

```c
fgets(line, 256, stdin);
sscanf(line, "%hd", &bookNo);
```

### 新输一行为什么之前的关键词变了？

那就是 `strtok` 回来的是指针。。`strcpy` 一下就好。

### 为什么变量在打印回车后发生了变化？

因为是局部变量，在未打印时，还暂时存在于内存中，打印后，该部分内存被 `stdout` 占用，导致问题。

这也其实我们，变量在一波毫不相干的操作中突然变化，很有可能是误使用局部变量导致的。

解决方法很简单，使用 `malloc` 即可。

### `sizeof struct ABC` 不对?

```c
sizeof (struct ABC)
```
