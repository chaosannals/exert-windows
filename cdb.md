# cdb

一般 cdb 在:

- C:\Program Files (x86)\Windows Kits\10\Debuggers\x64
- C:\Program Files (x86)\Windows Kits\10\Debuggers\x86

```bat
cdb C:\Windows\Syswow64\calc.exe
```

```bat
@rem 调试计算器
cdb C:\Windows\Syswow64\calc.exe

@rem -z 用于分析 *.dmp *.cab *.exe *.dll
cdb -z C:\Windows\Syswow64\calc.exe
```
