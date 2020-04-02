#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'


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
    passes=()
    cd build/test
    ninja && ninja test
    if [ $? -ne 0 ]; then
        return_code=1
        failures=("UT tests" "${failures[@]}")
    else
        passes=("UT tests" "${passes[@]}")
    fi
    cd ..
    cd test_st
    ninja run_st
    if [ $? -ne 0 ]; then
        return_code=1
        failures=("ST tests" "${failures[@]}")
    else
        passes=("ST tests" "${passes[@]}")
    fi

    if [ ${#failures[@]} -eq 0 ]; then
        printf "${GREEN}***************************************\n"
        printf "${GREEN}*         ALL TESTS PASSED            *\n"
        printf "${GREEN}***************************************\n${NC}"
    else
        printf "${GREEN}***************************************\n"
        printf "${GREEN}*            TESTS PASSED             *\n"
        printf "${GREEN}***************************************\n${NC}"
        printf "${BLUE}->${NC} %s\n" "${passes[@]}"
        printf "${RED}***************************************\n"
        printf "${RED}*            TESTS FAILED             *\n"
        printf "${RED}***************************************\n${NC}"
        printf "${BLUE}->${NC} %s\n" "${failures[@]}"
        printf "${NC}"

    fi

    return $return_code
}

if [[ "$*" == *--generate* ]]
then
    generate
else
    run_tests
fi
