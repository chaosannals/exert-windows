# exert windows

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