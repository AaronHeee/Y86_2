# **Y86 v2.0 CPU模拟器：**

Y86 v2.0项目旨在实现对上学期ICS课程项目**“基于Y86指令集的流水线CPU——Y86 v1.0”**的技术升级。

运用网络编程与并发编程知识，强化Y86内核，将其分拆为**客户端**与**服务端**，并使其多任务处理能力。

# **新增特性：**

### 1.Y86 v2.0内核：

* 内核代码重构
* 流水线内部线程级并发
  * W M E三阶段并发
  * 流水线寄存器值集中更新


### 2.Y86 v2.0服务器：

* 进程级并发多任务处理
* 服务端命令行界面
* 命令行参数设置周期睡眠参数

### 3.Y86 v2.0客户端：

* UI内部逻辑代码重构
* 回退功能

### 4.数据传输机制：

* socket网络编程
* 传输和显示异步


## **开发信息：**

### **1.开发平台：**

* Ubuntu 16.04

### **2.开发语言：**

* C++ 98
* Qt 5.7

### **3.开发工具：**

* CLion 2016.2.2
* QtCreator 4.0.2

