import pytest

import utils

def test_1():
    sut = utils.BinaryUnderTest(utils.find_binary("simple_module"))
    timeout_for_test = 2
    sut.start(timeout_for_test)
    sut.expect("Hello from simple single module", 1)
    sut.stop(1)

