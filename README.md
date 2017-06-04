# **Y86 CPU模拟器：**

## **新增特性：**

### 1.Y86内核(思路)：

* 多线程实现F D E M W五个阶段并发
  * 内核代码重构
  * 利用线程对虚拟存储器中变量的共享
    * Step1:  **waside-hcl.pdf** 
      * 变量类型
        * Y86 指令集变量
        * Y86 寄存器变量
        * Y86 ALU变量
        * 可能的状态变量
        * 指令寄存器内变量
        * 各级流水线寄存器变量
        * 各级流水线中间变量
      * 各级流水线中间变量赋值逻辑
      * Pipe控制逻辑信号赋值
    * 多进程实现多核Y86并行
      * 共享存储器
      * 合作运行一段代码
    * 多进程实现多个Y86并行
      * 各运行不同代码



## **开发信息：**

### **1.开发平台：**

* Ubuntu 16.04

### **2.开发语言：**

* C++ 98
* Qt 5.7

### **3.开发工具：**

* CLion 2016.2.2
* QtCreator 4.0.2
* Sublime 2.0
