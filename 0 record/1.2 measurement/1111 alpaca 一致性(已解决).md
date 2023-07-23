current testbench: 0.Cycles: 714304.the total number of tasks: 214.
current testbench: 1.Cycles: 767616.the total number of tasks: 137.
current testbench: 2.Cycles: 1320768.the total number of tasks: 57.
current testbench: 3.Cycles: 257024.the total number of tasks: 17.
current testbench: 4.Cycles: 643072.the total number of tasks: 1.
current testbench: 5.Cycles: 501760.the total number of tasks: 159.
current testbench: 6.Cycles: 1247936.the total number of tasks: 164.
current testbench: 7.Cycles: 710080.the total number of tasks: 157.
current testbench: 8.Cycles: 1916608.the total number of tasks: 186.

current testbench: 0.Cycles: 1916608.the total number of tasks: 0.
current testbench: 1.Cycles: 228800.the total number of tasks: 137.
current testbench: 2.Cycles: 1245184.the total number of tasks: 57.
current testbench: 3.Cycles: 453056.the total number of tasks: 94. 
current testbench: 4.Cycles: 199872.the total number of tasks: 1.
current testbench: 5.Cycles: 1544384.the total number of tasks: 159.
current testbench: 6.Cycles: 1213888.the total number of tasks: 164.
current testbench: 7.Cycles: 515008.the total number of tasks: 157.
current testbench: 8.Cycles: 377408.the total number of tasks: 186.

- 问题
pc 0 ar有问题  
3 cem不一致
一致: 1 2 4 5 6 7 8

解决了
- [x] 解决pa ar的war 新
- [x] alpaca pc cem war+  task 7
- [x] pc cem 修改task

task一致 total number应该全加上1 


- total number一致(实际值应为total number+1)
current testbench: 0.Cycles: 714304.the total number of tasks: 214.
current testbench: 1.Cycles: 767616.the total number of tasks: 137.
current testbench: 2.Cycles: 1320768.the total number of tasks: 57.
current testbench: 3.Cycles: 257280.the total number of tasks: 17. //172
current testbench: 4.Cycles: 643072.the total number of tasks: 1.
current testbench: 5.Cycles: 501760.the total number of tasks: 159.
current testbench: 6.Cycles: 1247936.the total number of tasks: 164.
current testbench: 7.Cycles: 710080.the total number of tasks: 157.
current testbench: 8.Cycles: 1916608.the total number of tasks: 186.

current testbench: 0.Cycles: 621632.the total number of tasks: 214. //如果连着测试,cycle会不一样. ((全局变量访问也是如此.顺序不同(关于cycle和test  )
current testbench: 1.Cycles: 228800.the total number of tasks: 137.
current testbench: 2.Cycles: 1245184.the total number of tasks: 57.
current testbench: 3.Cycles: 480704.the total number of tasks: 190.
current testbench: 4.Cycles: 199872.the total number of tasks: 1.
current testbench: 5.Cycles: 1544384.the total number of tasks: 159.
current testbench: 6.Cycles: 1213888.the total number of tasks: 164.
current testbench: 7.Cycles: 515008.the total number of tasks: 157.
current testbench: 8.Cycles: 377408.the total number of tasks: 186.

pc的0有问题 3不一致 还是有问题
时间测试应该在外部还是里面更好?

pc的i有问题

```C++
alpaca
current testbench: 0.Cycles: 717184.the total number of tasks: 17829.
current testbench: 1.Cycles: 771200.the total number of tasks: 905.
current testbench: 2.Cycles: 1325120.the total number of tasks: 1081.
current testbench: 3.Cycles: 384.the total number of tasks: 1.
current testbench: 4.Cycles: 647168.the total number of tasks: 1025.
current testbench: 5.Cycles: 505472.the total number of tasks: 927.
current testbench: 6.Cycles: 1256768.the total number of tasks: 2212.
current testbench: 7.Cycles: 712704.the total number of tasks: 669.
current testbench: 8.Cycles: 1936768.the total number of tasks: 5050.

current testbench: 0.Cycles: 624512.the total number of tasks: 17829.
current testbench: 1.Cycles: 232384.the total number of tasks: 905.
current testbench: 2.Cycles: 1249536.the total number of tasks: 1081.
current testbench: 3.Cycles: 491968.the total number of tasks: 2750.
current testbench: 4.Cycles: 203968.the total number of tasks: 1025.
current testbench: 5.Cycles: 1548096.the total number of tasks: 927.
current testbench: 6.Cycles: 1222720.the total number of tasks: 2212.
current testbench: 7.Cycles: 517696.the total number of tasks: 669.
current testbench: 8.Cycles: 397632.the total number of tasks: 5050.
```


alpaca的cem有问题


# 时间和一致性测试

分别测试了benchmark的时间(`Cycles`),及其执行的任务数(`total number of tasks`).

## alpaca
```C++
current testbench: 0.Cycles: 717184.the total number of tasks: 17829.
current testbench: 1.Cycles: 771200.the total number of tasks: 905.
current testbench: 2.Cycles: 1325120.the total number of tasks: 1081.
current testbench: 3.Cycles: 384.the total number of tasks: 1.
current testbench: 4.Cycles: 647168.the total number of tasks: 1025.
current testbench: 5.Cycles: 505472.the total number of tasks: 927.
current testbench: 6.Cycles: 1256768.the total number of tasks: 2212.
current testbench: 7.Cycles: 712704.the total number of tasks: 669.
current testbench: 8.Cycles: 1936768.the total number of tasks: 5050.
```
## latics
```C++
current testbench: 0.Cycles: 624512.the total number of tasks: 17829.
current testbench: 1.Cycles: 232384.the total number of tasks: 905.
current testbench: 2.Cycles: 1249536.the total number of tasks: 1081.
current testbench: 3.Cycles: 491968.the total number of tasks: 2750.
current testbench: 4.Cycles: 203968.the total number of tasks: 1025.
current testbench: 5.Cycles: 1548096.the total number of tasks: 927.
current testbench: 6.Cycles: 1222720.the total number of tasks: 2212.
current testbench: 7.Cycles: 517696.the total number of tasks: 669.
current testbench: 8.Cycles: 397632.the total number of tasks: 5050.
```

## 测试结果和问题

- 除`cem.c`外,alpaca和latics的benchmark的执行流程一致.
- alpaca的`cem.c`时间和任务数都产生溢出.

- [ ] 下一步将调试修复alpaca的`cem.c`


# 时间测试
1. 时间测试放在哪里更合适? 图1还是图2?

2. 问题
如果连续调用`latics()`和`alpaca()`并测试时间.尽管这里采用了不同的局部变量:

```    C++
main()
    Alpaca_only();  
	     uint16_t s, f; 
	    timer_start_count(&s);
	    timer_end_count(&f);
	    timer_get_cycles(s, f) 
    Periodic_only();
	    uint16_t a, b;
	    timer_start_count(&a);
	    timer_end_count(&b);
	    timer_get_cycles(a, b) 
```

还是会出现下面的结果:

```C++
current testbench: 8.Cycles: 1916608. //alpaca的最后一个benchmark
current testbench: 0.Cycles: 1916608. //latics的第一个benchmark, 读取的cycle是上一个cycle值.
current testbench: 1.Cycles: 228800. //后续正常
```

对于任务的计数也有同样的问题,为了方便,对`task_count`使用了全局变量,但把`task_count`换成`volatile task_count` 后,任务计数恢复正常.是编译器优化的问题吗?


1112
MCLK = DCO = 8MHz

alpaca
```C++
current testbench: 0.Cycles: 89664.the total number of tasks: 726.
current testbench: 1.Cycles: 96384.the total number of tasks: 905.
current testbench: 2.Cycles: 165632.the total number of tasks: 1081.
current testbench: 3.Cycles: 109184.the total number of tasks: 2750.
current testbench: 4.Cycles: 80832.the total number of tasks: 1025.
current testbench: 5.Cycles: 63168.the total number of tasks: 927.
current testbench: 6.Cycles: 157056.the total number of tasks: 2212.
current testbench: 7.Cycles: 89024.the total number of tasks: 669.
current testbench: 8.Cycles: 242048.the total number of tasks: 5050.
```

Latics
```C++
current testbench: 0.Cycles: 78080.the total number of tasks: 726.
current testbench: 1.Cycles: 28992.the total number of tasks: 905.
current testbench: 2.Cycles: 156160.the total number of tasks: 1081.
current testbench: 3.Cycles: 61440.the total number of tasks: 2750.
current testbench: 4.Cycles: 25472.the total number of tasks: 1025.
current testbench: 5.Cycles: 193472.the total number of tasks: 927.
current testbench: 6.Cycles: 152832.the total number of tasks: 2212.
current testbench: 7.Cycles: 64704.the total number of tasks: 669.
current testbench: 8.Cycles: 573952.the total number of tasks: 5050.
```

注: 需要把alpaca()和latics()分别测量.
如果连续测量,会出现以下问题,原因不明:
```C++
//alpaca
current testbench: 0.Cycles: 89664.the total number of tasks: 726.
current testbench: 1.Cycles: 96384.the total number of tasks: 905.
current testbench: 2.Cycles: 165632.the total number of tasks: 1081.
current testbench: 3.Cycles: 109184.the total number of tasks: 2750.
current testbench: 4.Cycles: 80832.the total number of tasks: 1025.
current testbench: 5.Cycles: 63168.the total number of tasks: 927.
current testbench: 6.Cycles: 157056.the total number of tasks: 2212.
current testbench: 7.Cycles: 89024.the total number of tasks: 669.
current testbench: 8.Cycles: 242048.the total number of tasks: 5050.
//latics 
//第一个testbench的计数都出现了问题 
current testbench: 0.Cycles: 242048.the total number of tasks: 0.
current testbench: 1.Cycles: 28992.the total number of tasks: 905.
current testbench: 2.Cycles: 156160.the total number of tasks: 1081.
current testbench: 3.Cycles: 61440.the total number of tasks: 2750.
current testbench: 4.Cycles: 25472.the total number of tasks: 1025.
current testbench: 5.Cycles: 193472.the total number of tasks: 927.
current testbench: 6.Cycles: 152832.the total number of tasks: 2212.
current testbench: 7.Cycles: 64704.the total number of tasks: 669.
current testbench: 8.Cycles: 573952.the total number of tasks: 5050.
```

[Git忽略文件.gitignore的使用 - 简书 (jianshu.com)](https://www.jianshu.com/p/a09a9b40ad20)