# exert windows

## VS 注意项

1. OutputDebugString 在 VS 调试时会被截获，不会在 DebugView 看到。从 VS2010 开始有个 BUG ，就是如果 OutputDebugString 输出的字符串不换行 "\n" 就不会被输出到 VS 的调试输出窗口。

## VS 远程调试 Vbox

1. Vbox 把网卡改成桥接，然后固定共享 VS的远程调试工具目录（例如：C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\Remote Debugger）
2. 然后 Vbox 里系统框架（例如：x64）进入目录，运行 msvsmon.exe 并把权限设置为无。
3. 通过 CMakeLists.txt 右键添加调试配置，选 Windows 远程调试 Windows 的配置模板。
4. 配置的 cwd 执行挂在的路径（例如我挂在在 R:\）匹配的机器架构（x64)目录，如下。projectTarget 是程序文件 remoteMachineName 是 vbox 里的名字和端口，type 必须是 remoteWindows。

```json
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "args": [],
      "authenticationType": "none",
      "comment": "了解如何配置远程 Windows 调试。有关详细信息，请参阅 https://docs.microsoft.com/cpp/build/cmake-remote-debugging",
      "cwd": "R:\\x64",
      "deploy": [],
      "env": {},
      "name": "CMakeLists.txt",
      "program": "${debugInfo.fullTargetPath}",
      "project": "CMakeLists.txt",
      "projectTarget": "tanker.exe",
      "remoteMachineName": "WIN7-PC:4026",
      "type": "remoteWindows"
    }
  ]
}
```


## mklink

注：PowerShell 没有该命令，要 CMD 命令行下才有

```bat
@rem 迁移 .nuget 目录到D盘
@rem /D 目录 符号链接
@rem /H 硬链接 只能是文件
@rem /J 目录联接（Junction Point）
mklink /D C:\Users\yourname\.nuget D:\.nuget
```

## ipconfig

```bat
@rem 刷新 DNS 缓存
ipconfig /flushdns
```