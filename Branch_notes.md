# 分支管理和版本说明
- 现在有以下两个分支
	- main
	- time
- 下面是分支介绍

## 基础版本
- main 
	- 简介和功能
		- alpaca和latics已实现 
		- 一致性测试已通过 #5
	- 备注(问题)
		- printf串口打印失效

## 时间测量
- time
	- 简介: main分支的扩展版本
	- 日志
		- [Nov 13, 2022](https://github.com/Yuano0o/alpaca/commit/9aaad2cbe0629667e9fd15a437287fca8db777db) 
			- 测试简介
				- 系统: alpaca+latics 
				- 粒度: benchmark
				- 项目: total time  
			- 具体说明
				- 测试了alpaca和latics的所有benchmark的**总运行时间** 以及**总任务数** 
			- 测试结果记录
				- [data](https://github.com/Yuano0o/alpaca/tree/time/data "data")
			- 备注 
				-  MCLK = 8MHz
				- 分别测量alpaca()和latics() 
					- 否则会有问题 具体描述见 #5
					- 类似问题见 #10
