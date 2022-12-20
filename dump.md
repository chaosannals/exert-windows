# 转储文件

需要系统的转储文件，不然一半进程没有存这些信息。
C:\Windows\MEMORY.DMP 内存的转储，不一定有 bug 信息。

```bat
@rem 打印寄存器
r cr4

@rem 显示各种格式
.formats b50ef8

@rem 设置符号存储路径（需要联网）
.symfix C:\symbols

@rem 重新加载
.reload

@rem 打印所有进程
@rem DirBase 页目录基地址，与 cr3 匹配的话说明当前在执行这个程序。
!process 0 0 

@rem 显示详细信息，bug 的转储文件才有。
lmvm realbug

@rem 显示物理内存（指定物理内存地址）double word
!dd 01ae000
@rem 显示物理内存 byte
!db 01ae000

@rem 显示虚拟地址信息（比如查看PDE,PTE,等，用来算出物理地址）
!pte f8c2e04d
```