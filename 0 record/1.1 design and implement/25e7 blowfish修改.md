blowfish 修改

task_down原本是输出时间-->return

需要调试 把alpaca包含的东西移植

//LOG 

VERBOSE 日志

1031更新 看看

在原有工程上
修改了blowfish 看看代码 
修改了宏定义

static还是和PC保持一致比较好

## 问题
global
	函数内使用 
		扩大/改变作用域
		引入外面已声明的全局变量 / 声明为全局变量

static
	全局 和 局部 只有作用域不同 生存周期都是整个程序运行周期 
		而函数内的局部变量 在栈中分配空间 调用结束就释放
		static局部变量 函数返回后不释放 只是作用域没了


## benchmark
blowfish
两部提交  函数模板 其他调试问题再说

sort
能跑通
commit 有问题 
考虑用空数组代替data_src初值 让基址指针指向它


## 链接
[(40条消息) C语言__attribute__的使用_【ql君】qlexcel的博客-CSDN博客_c __attribute__](https://blog.csdn.net/qlexcel/article/details/92656797)
[(40条消息) Go语言的LABLE标签与goto语句_ScarletMeCarzy的博客-CSDN博客](https://blog.csdn.net/ScarletMeCarzy/article/details/106520823)

[(40条消息) C和C++中泛型编程 - 适应不同类型参数的函数_Sean_SJTU的博客-CSDN博客](https://blog.csdn.net/seanwang_25/article/details/26283015)
[C++函数模板（模板函数）详解 (biancheng.net)](http://c.biancheng.net/view/315.html)