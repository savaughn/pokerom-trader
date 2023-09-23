#!/bin/bash

export PATH=/Users/spencer/Projects/pokerom-trader/libpksav/testing/unit-tests:$PATH
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/Users/spencer/Projects/pokerom-trader/libpksav/lib
export PKSAV_TEST_SAVES=/Users/spencer/Projects/pokerom-trader/libpksav/testing/pksav-test-saves

/Users/spencer/Projects/pokerom-trader/libpksav/testing/unit-tests/byteswap_test
