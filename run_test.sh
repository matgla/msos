#!/bin/bash

function generate {
    if [ -d "build/test" ] && [ -d "build/test_st" ] && [ -f "build/test/generation_done.stamp" ] && [ -f "build/test_st/generation_done.stamp" ]
    then
        return 0
    fi
    mkdir -p build/test
    mkdir -p build/test_st
    cd build/test && cmake ../.. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -GNinja
    if [ $? -ne 0 ]; then
        cd ..
        rm -rf build/test
        return 1
    fi
    touch generation_done.stamp
    cd ..
    cd test_st && cmake ../.. -DCMAKE_BUILD_TYPE=Release -DBOARD=Stm32_Black_Pill -GNinja
    if [ $? -ne 0 ]; then
        cd ..
        rm -rf build/test_st
        return 1
    fi
    touch generation_done.stamp
    return $?
}

function run_tests {
    local return_code=0
    failures=()
    cd build/test
    ninja && ninja test
    if [ $? -ne 0 ]; then
        return_code=1
        failures=("UT tests failed" "${failures[@]}")
    fi
    cd ..
    cd test_st
    ninja run_st -d explain -v
    if [ $? -ne 0 ]; then
        return_code=1
        failures=("ST tests failed" "${failures[@]}")
    fi
    printf 'FAILURE\n=================================\n'
    printf '%s\n' "${failures[@]}"
    return $return_code
}

if [[ "$*" == *--generate* ]]
then
    generate
else
    run_tests
fi
