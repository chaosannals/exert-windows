# netsh

```bash
# 查看网络用户配置信息
netsh wlan show profiles

# 查看指定用户信息信息，包括密码。
# cfgname 就是上一步列举的配置名
netsh wlan show profile cfgname key=clear
```