import pytest

import utils

def test_1():
    sut = utils.BinaryUnderTest(utils.find_binary("simple_module"))
    sut.start()
    sut.expect("Hello from simple single module", 1)
    sut.stop(1)

