# easy-2048
## 介绍
Linux平台使用C语言编写的2048小游戏。
## 安装教程
1. 需先安装 libncurses5-dev，各大发行版均可使用包管理器安装
2. 编译并运行
```bash
cd easy-2048
gcc 2048.c -o 2048 -lcurses #编译
./2048 #运行
```
## 使用说明
与传统2048规则相同，出现2048即为胜利。
## 跨平台
在Windows平台编译运行需先修改源码，将引用的头文件<curses.h>改为<conio.h>
## 许可证
采用GPL-3.0开源许可协议