# DuiLib

注：vcpkg 需要指定的系统框架
例：设置 Triplet: x64-windows-static

```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install duilib
./vcpkg install duilib:x64-windows
./vcpkg install duilib:x64-windows-static
./vcpkg install duilib:x64-windows-static-md
```

目前静态 DuiLib 会报错。

注：在用 vcpkg 时候被墙用代理，一般 HTTPS 代理。HTTP 已经很少了。

```pwsh
$env:HTTP_PROXY="127.0.0.1:1088" 
$env:HTTPS_PROXY="127.0.0.1:1088"
```

[第三方XML编辑器DuiEditor](https://github.com/xfcanyue/DuiLib_DuiEditor)

DuiEditor 插入 ImageBox 有编辑器直接崩溃关闭的问题。

注：xml 的路径，相对路径是相对于生成的 exe 文件。
