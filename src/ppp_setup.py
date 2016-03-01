#!/usr/bin/python3

import sys
import subprocess

def getNetworkInterfaces():
    lines = subprocess.check_output("ls -1 /sys/class/net".split(" "), universal_newlines = True)
    interfaces = list(filter(bool, lines.split("\n")))
    return interfaces

def filterEthernetInterfaces(interfaces):
    return list(filter(lambda interface: interface[0] == "e", interfaces))

def getContent():
    print("湖北E信设置程序")

    print("> 请输入用户名: ", end = "")
    sys.stdout.flush()
    account = sys.stdin.readline().rstrip()

    print("> 请输入密码: ", end = "")
    sys.stdout.flush()
    password = sys.stdin.readline().rstrip()

    interfaces = filterEthernetInterfaces(getNetworkInterfaces())
    if not interfaces:
        print("未发现网卡。", file = sys.err)
        sys.exit(-1)

    for i, interface in enumerate(interfaces):
        print("{i} - {interface}".format(**locals()))

    print("> 请填写用于E信拨号的有线网卡的序号: ", end = "")
    sys.stdout.flush()
    i = sys.stdin.readline().rstrip()
    interface = interfaces[int(i)]
    content = """plugin rp-pppoe.so
plugin libHubeiExin4PPP.so

# Network Interface
{interface}

debug
persist
defaultroute
hide-password
noauth
usepeerdns
nodetach
name {account}
pwd {password}
""".format(**locals())
    return content

def main():
    try:
        with open("/etc/ppp/peers/hubeiexin", "w", encoding = "utf-8") as f:
            content = getContent()
            f.write(content)
            f.close()
        with open("/etc/ppp/ip-up.d/01-hubeiexin", "w", encoding = "utf-8") as f:
            f.write("""#!/bin/sh
ip route add default dev $1""")
            f.close()
    except PermissionError:
        print("权限不足。请以root执行。", file = sys.stderr)
        sys.exit(-1)
    print("设置完成。")


if __name__ == "__main__":
    main()
