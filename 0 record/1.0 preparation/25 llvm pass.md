- # llvm pass
	[LLVM Pass入门导引](https://zhuanlan.zhihu.com/p/122522485)
- ## llvm
	 - 作者参考的intro [The Architecture of Open Source Applications: LLVM (aosabook.org)](http://www.aosabook.org/en/llvm.html)
	- git [llvm-project/llvm/lib/IR at main · llvm/llvm-project (github.com)](https://github.com/llvm/llvm-project/tree/main/llvm/lib/IR)
	- intro+llvm编译参考 [Gettintroing Started with the LLVM System — LLVM 16.0.0git documentation](https://llvm.org/docs/GettingStarted.html)
	- cmake编译pass(好像不用,用clang)
		- 参考 https://link.zhihu.com/?target=http%3A//llvm.org/docs/CMake.html%23cmake-out-of-source-pass
		- 入门项目 [ttroy50/cmake-examples: Useful CMake Examples (github.com)](https://github.com/ttroy50/cmake-examples)

# 笔记
- ## alpaca-pass 
	- build run
		compile、build分别对应编译和连接 run就是运行刚刚产生的exe
- ## llvm pass
	- [LLVM Pass入门](https://zhuanlan.zhihu.com/p/122522485)
	- ### 准备工作
		- 4 --安装llvm和clang
	- ### 构建 pass
		- [Writing an LLVM Pass](https://llvm.org/docs/WritingAnLLVMPass.html) 
			- [Introduction](https://link.zhihu.com/?target=https%3A//llvm.org/docs/WritingAnLLVMPass.html%23introduction-what-is-a-pass) 和[Quick Start](https://link.zhihu.com/?target=https%3A//llvm.org/docs/WritingAnLLVMPass.html%23quick-start-writing-hello-world) 
			- 略读 [Pass classes and requirements](https://link.zhihu.com/?target=https%3A//llvm.org/docs/WritingAnLLVMPass.html%23pass-classes-and-requirements) 与[Pass Statistics](https://link.zhihu.com/?target=https%3A//llvm.org/docs/WritingAnLLVMPass.html%23pass-statistics) 这两部分
	- ### ==编译 --Clang run Pass
		- $ clang -Xclang -load -Xclang path/to/LLVMHello.so main.c -o main```
		- 参考 [Run an LLVM Pass Automatically with Clang (cornell.edu)](https://www.cs.cornell.edu/~asampson/blog/clangpass.html)
		- 不用cmake?--源代码编译成IR代码，opt运行Pass
			Pass集成到clang的参数中调用


- llvm可以继续看的
	- 作者参考的intro [The Architecture of Open Source Applications: LLVM ](http://www.aosabook.org/en/llvm.html)
	- llvm编译参考+intro [Getting Started with the LLVM System — LLVM 16.0.0git documentation](https://llvm.org/docs/GettingStarted.html)
	- [Run an LLVM Pass Automatically with Clang](https://www.cs.cornell.edu/~asampson/blog/clangpass.html)


[[08072142 陈钦霖 学习 记笔记]]
- [作者 陈钦霖 可以继续看的](https://www.zhihu.com/people/QinlinChen/answers)
	- [x] 学习最重要的过程是理解知识 
	- [ ] 计算机本科的知识框架