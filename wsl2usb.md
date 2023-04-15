# WSL2 下使用 USB 

#### 1. Windows 下 安装 [usbipd-win](https://github.com/dorssel/usbipd-win)

下载 msi 安装，会安装 usbipd 服务。

#### 2. WSL2 的 Linux 下安装 usbip 客户端

```bash
# 安装
sudo apt install linux-tools-generic hwdata

# 启动自动更新设备
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
```

#### 3. 查看设备

Windows 下可以查看 wsl 的设备信息

```bat
@rem 查看 wsl 可用设备列表
usbipd wsl list

@rem 挂在设备到 wsl2 （需要管理员权限，而且可能需要 wsl --update 来启动 wsl 内核对 usbip 的支持）
@rem usbipd wsl attach --busid <busid>
usbipd wsl attach --busid 1-3

@rem 不使用是可以手动卸载
@rem usbipd wsl detach --busid <busid>
@rem usbipd wsl detach --busid 1-3
```

Linux 下查看被附加的设备

```bash
# 查看设备
lsusb
```
