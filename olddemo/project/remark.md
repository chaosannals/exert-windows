# remark

## idtd

1. 获取 钉钉 进程 PID。
2. 通过 PID 获取得到句柄，因为 钉钉 多进程所以必须能处理多个。
3. 扫描是得到 HWND。
4. 通过 HWND 得到 HDC 截屏。
