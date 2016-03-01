# 湖北E信拨号器

## 特点

  * 在武汉地区测试通过
  * 无广告，无驻留
  * 支持Linux (Archlinux x64/armv7下测试通过)
  * 支持Windows (Win10 x64下测试通过)
  * 未实现心跳功能。因此部分用户可能出现拨号成功后，在十分钟内被断网的情况


## 项目状态

  * 本人即将离开湖北，以后没有测试和使用环境了，也不会再维护这个项目了。

  * 加密算法完成度低。仅部分的加密算法得到了破解和实现，在本人的环境下测试通过，不保证所有湖北电信E信的用户都可以得到满意的效果。

  * 不支持ipv6拨号。


## 支持的Linux发行版

  作者只在glibc下使用过本软件。在其它libc的系统中效果未知。下面列出常见的

  * Archlinux(x86/x64/arm)。提供完整的`PKGBUILD`。执行`makepkg`即可。因为这个软件本人不维护，所以不往AUR上放了。

  * Debian系。项目内包含debian文件夹。但未测试，请根据提供的debian文件夹适度修改。
 
  * RPM系。本人没提供相应的spec。不过好在本项目使用cmake编译，而且不复杂，可以参考上面两个linux的自己搞一个。


## Linux下使用方法

  1. 因为E信的加密算法涉及时间戳，请确保系统的系统时间设置正确！尤其是Windows双启动的用户，请确保Windows和Linux交替启动之后不会有8小时的时间差。
    将以下内容保存为`*.reg`的文件，并双击导入注册表，可以使Windows把硬件时间设置为UTC。

    ```
    Windows Registry Editor Version 5.00
    
    [HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\TimeZoneInformation]
         "RealTimeIsUniversal"=dword:00000001
    ```

  2. 设置账号。输入以下命令，并按提示设置。
  
  ```bash
  sudo hubei_exin_setup
  ```
  
  3. 拨号与挂断。

  以systemd发行版为例。
  ```bash
  # 马上拨号
  sudo systemctl start ppp@hubeiexin

  # 开机自动拨号（下次启动生效）
  sudo systemctl enable ppp@hubeiexin

  # 如果你的拨号不成功，可以再试一次。
  sudo systemctl restart ppp@hubeiexin

  # 马上下线
  sudo systemctl stop ppp@hubeiexin

  # 取消开机自动拨号（下次启动生效）
  sudo systemctl disable ppp@hubeiexin
  ```

  非systemd发行版，如OpenWRT，请参考发行版对应的ppp/pppd的文档使用编译出的拨号模块。


## 编译Windows下的拨号器

  本项目支持在Linux使用mingw交叉编译，也支持在Windows上使用msys2编译。但未在VS系列下测试过。


## Windows下使用说明

  1. 将下载到的，或编译好的可执行文件，复制到桌面上。
  2. 首次运行。按照步骤提示，填入账户名、密码。勾选保存密码。请注意，拨号名必须为`HubeiExin`。如果你在此前有一个同名的网络链接，请删除。
  3. 首次运行配置完毕，会自动拨号。拨号成功或失败后，程序退出。
  4. 下线，请使用Windows自带的界面断开链接。
  5. 此后运行，不会弹出配置向导，直接拨号完成后退出。本软件不会设置开机自启。
  6. 卸载本软件，只要到控制面板的网络部分，删除`HubeiExin`这个链接即可。无其它残留。


## 其它

  本软件不负责共享Wifi，请查找你使用平台的资料自行设置。


## 目录结构

```
├── arch                           Archlinux打包用
│   ├── PKGBUILD
│   └── ppp-hubeiexin.install
├── CMakeLists.txt
├── debian                         Debian系打包用
│   ├── compat
│   ├── control
│   └── source
│       └── format
├── README.md                      本Readme
└── src
    ├── asm.c                      从E信中提取的加密算法的汇编实现
    ├── asm.h
    ├── CMakeLists.txt
    ├── crossexam.c                用于检验C实现等效于汇编实现的测试代码
    ├── crossexam.h
    ├── exam.c                     用于检验C实现等效于观测结果的测试代码
    ├── exam.h
    ├── hubeiexin.c                E信加密算法的C实现
    ├── hubeiexin.h
    ├── platform.h
    ├── platform_linux.c           Linux平台相关
    ├── platform_win32.c           Win32平台相关
    ├── ppp_hubeiexin.ip-up        Linux下ppp插件支持文件
    ├── ppp_plugin.c               Linux下ppp插件
    ├── ppp_plugin.h
    ├── ppp_setup.py               Linux下的首次配置程序（Python3版）
    ├── ppp_setup.rs               Linux下的首次配置程序（Rust版）
    ├── ppp_version.c              为Linux下安装ppp插件确定安装位置的辅助程序
    ├── ras_dialer.c               Windows下的拨号器的实现文件
    ├── ras_dialer.h
    ├── utils.c                    杂项
    └── utils.h
```