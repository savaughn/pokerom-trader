#!/bin/bash

export PATH=/Users/spencer/Projects/pokerom-trader/deps/pksav/testing/unit-tests:$PATH
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/Users/spencer/Projects/pokerom-trader/deps/pksav/lib
export PKSAV_TEST_SAVES=/Users/spencer/Projects/pokerom-trader/deps/pksav/testing/pksav-test-saves

/Users/spencer/Projects/pokerom-trader/deps/pksav/testing/unit-tests/gen2_save_test
