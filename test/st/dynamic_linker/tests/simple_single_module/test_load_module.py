import pytest
import pexpect

import configparser
import sys



def test_1():
    config = configparser.ConfigParser()
    with open("test.cfg", "w") as f:
        f.write("aaa")
    config.read_file(open("config.ini"))
    print(config['BINARIES']["simple_module"])

    #host = pexpect.spawnu("qemu-system-arm -machine stm32-f103c8 -kernel \
    #                      /home/mateusz/repos/msos/bin/test/st/dynamic_linker/host/host.bin\
    #                      -serial stdio -nographic")
    #host.expect("TEST DONE", timeout=1)
    #host.sendline("quit")

