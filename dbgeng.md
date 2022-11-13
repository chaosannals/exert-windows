# dbgeng

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

@rem 运行 会到达断点或者事件触发而停止
g

@rem 查看堆栈跟踪
k

@rem 查看线程列表
~

@rem 切到 线程1
~1s
@rem 切到 线程2
~2s

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
