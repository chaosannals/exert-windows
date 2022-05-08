# exert-windows

## 去除 vc runtime dll 依赖

在 Visual Studio 属性 > 链接器 > 高级 > 入口点 输入 WinMain 来绕过对运行库的依赖。

## 目录配置

- 输出目录 $(SolutionDir)target\$(Platform)\$(Configuration)\
- 中间目录 $(SolutionDir)middle\$(ProjectName)\$(Platform)\$(Configuration)\

## dumpbin

```cmd
# 查看动态库依赖。
dumpbin /dependents <path_to_exe>
```

注：部分项目引用了外部二进制包，这些包没有Debug包，所以Debug不可构建。

## Windows 驱动

[官方示例](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/samples/)

## DebugView

微软官网可以下载到，必须以管理员身份打开。

注册表（这个导致 VBOX 装的系统坏掉，有待验证可行性）

HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter

DEFAULT 键 DWORD 类型

xp:[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter]"DEFAULT"=dword:00000008

vista:[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter]"DEFAULT"=dword:0000000f

win7:[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter]"DEFAULT"=dword:0000000f
