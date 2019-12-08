import pytest
import pexpect

import configparser
import sys

def find_binary(binary_name):
    config = configparser.ConfigParser()
    config.read_file(open("config.ini"))

    binary_path = config["BINARIES"][binary_name]
    return binary_path

class BinaryUnderTest:
    def __init__(self, binary_path):
        self.binary_path = binary_path

    def start(self, timeout):
        self.executable = pexpect.spawnu("qemu-system-arm -machine " \
                                         "stm32-f103c8 -kernel " + \
                                         self.binary_path + " -serial stdio " \
                                         " -nographic")
    def stop(self, timeout):
        if (self.executable):
            self.executable.expect("TEST DONE", timeout)
            self.executable.sendline("quit")
        else:
            raise "Executable not started"

    def expect(self, expectation, timeout):
        if (self.executable):
            self.executable.expect(expectation, timeout)
        else:
            raise "Executable not started"
