## 编程环境

|Key      |Value       |Version|
|:--------|:-----------|:------|
|System   |WSL Ubuntu  | 20.04 |
|Language |C           |       |
|Compiler |gcc         | 9.3.0 |
|Debugger |gdb         | 9.1   |
|Formatter|clang-format| 10.0.0|
|Editor   |VS Code     | 1.45.1|

#### identifier "NULL" is undefined

因为要构成多文件，感觉 `stdio.h` 对于排序函数并不需要，就没有添加。但是提示`identifier "NULL" is undefined`。

原来 `NULL` 不是内置的。根据 <https://stackoverflow.com/questions/924664>，`NULL` 在 `stddef.h` 里，但是一般会在 `stdio.h` “附赠”
