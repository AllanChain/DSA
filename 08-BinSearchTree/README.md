# 二叉排序树上机报告

## 问题理解

二叉排序树是一种既能动态添加删除，有较方便查找的字典数据结构。二叉排序树的特点之一在于增、删、查以查为核心。

## 设计和实现

通过观察删除的前半部分代码可以发现，基本和查找雷同，就是记录了一个父节点。而恰巧查找部分就有变量 `q`，记录 `p` 的前驱，只需增加一个指针参数（返回值）`parentPos` 即可。但是有时候并不需要这个返回值，故增加条件判断，允许传入 `NULL`。这样查找部分的代码就被很好的重用了。

其余部分和书本代码雷同，不加赘述。

## 编程环境

|Key      |Value       |Version|
|:--------|:-----------|:------|
|System   |WSL Ubuntu  | 20.04 |
|Language |C           |       |
|Compiler |g++         | 9.2.1 |
|Debugger |gdb         | 9.1   |
|Editor   |VS Code     | 1.45.0|

## 上机问题与解决

### VSCode 编译错误

> The task provider for "C/C++" tasks unexpectedly provided a task of type "shell".

Bug 即将被修好 <https://github.com/microsoft/vscode-cpptools/issues/5388#issuecomment-626082004>

### Segmentation Fault

给 `search` 的 `parentPos` 传了一个 `NULL`，又打算通过赋值的方式返回。。

加个条件判断不是 `NULL` 再赋值就好。

### 又一个 Segmentation Fault

忘记给 `ptree` 赋初值 `NULL`了

### 没输入 `search` 怎么就进了 `search`？

事实证明是 `strcmp(..., ...)` 后面忘打了 `== 0`
