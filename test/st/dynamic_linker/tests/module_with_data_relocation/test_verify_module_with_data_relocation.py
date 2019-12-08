import pytest

import utils

def test_use_module_with_data_relocation():
    sut = utils.BinaryUnderTest(utils.find_binary("module_with_data"))
    sut.start()
    sut.expect("Module started", 1)
    sut.stop(1)

