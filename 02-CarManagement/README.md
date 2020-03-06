# 车辆管理问题上机报告

## 问题理解

车辆管理问题是一个让人看一眼就像建数据库去做的题。一张表放所有的车，一张表放所有的违章记录，Foreign key 搞定 one-to-many。

但是由于是线性表一章，还要用 C++ 写，就要求用简单的文件存储形式，以及合适的数据结构，来表达数据关系。

## 设计与实现

### 文件结构

使用一个文件存储车辆和违章信息，并每行使用前缀来标志是车还是违章记录。每行中不同字段使用逗号分隔。利用文件里位置的联系来确定违章记录的从属关系。

如：

```
CAR:1,面包车,2016-02-17,130000,李建东
REC:1,闯红灯,2017-05-26,海淀区颐和园路5号,罚款1000元
CAR:23,小轿车,2019-12-30,235000,王光法
```

### 数据结构

由于一辆车和一条违章记录占用空间较大，这里使用链表的数据结构不会造成很大的相对空间开销。即车之间、违章记录之间分别构成链表，在每条车的记录中存储违章记录链表的头结点。

并对车 (`Car`)、车链表 (`CarList`)、违章记录 (`Rec`)、违章记录链表 (`RecList`) 分别用类（拙劣地）封装。

访问各成员使用简单粗暴的全部遍历。

## 编程环境

|Key      |Value       |Version|
|:--------|:-----------|:------|
|System   |WSL Ubuntu  | 19.10 |
|Language |C++         |       |
|Compiler |g++         | 9.2.1 |
|Debugger |gdb         | 8.3   |
|Formatter|clang-format| 9.0.0 |
|Editor   |VS Code     | 1.42.1|

## 上机问题与解决

### 如何从 `stdin` 读取一行带有空格的？

```cpp
std::getline(std::cin, line);
```

### `str.startswith()` Equivalent?

```cpp
line.rfind("add car ", 0) == 0
```

### 数次忘记 `car = car->next` 导致死循环

~~没救了~~ `while` 完了赶紧写

### 读取逗号分隔

```cpp
#include <sstream>

std::istringstream lineStream(line);
std::getline(lineStream, id, ',')
```

### Not Found 怎么办？

为了 DRY，需要写一个`getCarById`的函数，然后调用它。那么如果没有这个 id 怎么办？难道要我每个地方都判断一下吗？不可能的。直接上 `exception`：

```cpp
#include <stdexcept>

throw std::out_of_range("Car not found");

try {
  // code
} catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
}
```

备忘 C++ 内置错误：
- logic_error
- domain_error
- invalid_argument
- length_error
- out_of_range
- runtime_error
- range_error
- overflow_error
- underflow_error

## 收获

~~不写不知道，`Python` + `SQLite` 真香！~~

- 基于类实现的链表
- C++ 常用操作

## 疑惑

看到按照某个某个字段显示的时候，每个字段都要新写一个函数，一点也不 DRY。没有 generator怎么搞定写了一遍又一遍的链表循环？除了直接上 `map` 之类不“数据结构与算法”的东西？
