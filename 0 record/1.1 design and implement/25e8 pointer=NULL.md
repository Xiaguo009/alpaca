```C
int** x = NULL
x --> (int*)NULL //NULL为0地址 不允许写值
*x 得到 NULL 
*x = data //由于NULL 不允许写值 所以赋值失败

int** x = NULL
*x = data 意味着在0地址写入data 不允许

int* x = NULL
*x 还是得到NULL 
除非让x = data 否则不可行
```