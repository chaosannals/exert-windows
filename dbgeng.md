# dbgeng

调试引擎

## 命令

```bat
@rem 结束调试
q

@rem 结束并分离
qd

@rem 查看模块列表
lm

@rem 查询符号 x 模块名!*  *是通配符
x ntdll32!*

@rem 设置断点
bu calc!wWinMain

@rem 列举断点
bl

@rem 运行 （F5） 会到达断点或者事件触发而停止
g

@rem 执行当前函数直到结束（Shift+F11） go up
gu

@rem 单步进入（F11）
t
@rem 单步进入直到地址
ta 0x00781759
@rem 单步进入直到遇到 call
tc
@rem 单步进入直到遇到分支跳转
th
@rem 单步进入直到遇到 ret 命令
tt
@rem 单步进入直到遇到 call 或 ret 命令
tct

@rem 单步跳过（F10）
p
@rem 单步跳过直到地址
pa 0x00781759
@rem 单步跳过直到遇到 call
pc
@rem 单步跳过直到遇到分支跳转
ph
@rem 单步跳过直到遇到 ret 命令
pt
@rem 单步跳过直到遇到 call 或 ret 命令
pct

@rem 查看堆栈跟踪
k

@rem 查看线程列表
~
@rem 切到 线程1
~1s
@rem 切到 线程2
~2s
@rem 显示 线程2 寄存器
~2r
@rem 显示所有线程寄存器
~*r

@rem 查看数字基数
n
@rem 修改
n 10

@rem 符号文件重新加载
.reload

@rem 查看默认表达式求值器
.expr
@rem 修改为 c++ 或 masm
.expr /s c++
.expr /s masm
```


```bat
@rem gdtl 和 ldtl 需要读2次才能读完整
r gdtr
r gdtr
r ldtl
r ldtl

@rem 通过输入段寄存器的值来查看段信息
dg 1b

@rem 查看 PEB
!peb
@rem 显示PEB字段详细
dt ntdll!_PEB

@rem 查看 TEB
!teb
@rem 显示TEB字段详细
dt ntdll!_TEB

@rem Display Type 显示类型
dt -h
```
## 脚本与自动化

