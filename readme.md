# exert-windows

## 去除 vc runtime dll 依赖

在 Visual Studio 属性 > 链接器 > 高级 > 入口点 输入 WinMain 来绕过对运行库的依赖。

## dumpbin

```cmd
# 查看动态库依赖。
dumpbin /dependents <path_to_exe>
```
