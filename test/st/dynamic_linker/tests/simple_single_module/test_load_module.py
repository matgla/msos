import pytest
import pexpect


def test_1():
    host = pexpect.spawnu("qemu-system-arm -machine stm32-f103c8 -kernel \
                          /home/mateusz/repos/msos/bin/test/st/dynamic_linker/host/host.bin\
                          -serial stdio -nographic")
    host.expect("TEST DONE", timeout=1)
    host.sendline("quit")

