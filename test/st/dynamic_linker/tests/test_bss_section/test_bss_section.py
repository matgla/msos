import pytest

import utils

def test_bss_section():
    sut = utils.BinaryUnderTest(utils.find_binary("test_bss_section"))
    sut.start()
    sut.expect("Hello from bss test", 1)
    sut.expect("Array 1: 1, 2, 3, 4,", 1)
    sut.expect("Integer 1: 0", 1)
    sut.expect("Integer 2: 15", 1)
    sut.expect("Integer 3: 5", 1)
    sut.expect("Array 2: 5, 6, 19, 8,", 1)
    sut.stop(1)

