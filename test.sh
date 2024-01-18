#!/bin/bash

function check_return_value() {
    if [ $? -ne 0 ]; then
      echo -n "Test $1: "
      echo -e "\033[31mKO\033[0m"
      exit 1
    fi
  if [ $? -eq 0 ]; then
      echo -n "Test $1: "
      echo -e "\033[32mOK\033[0m"
  fi
}

cd ./samples && ./build.sh > /dev/null 2>&1 && cd ..
./woody_woodpacker ./samples/hello > /dev/null
./woody > /dev/null
check_return_value 1
./woody_woodpacker ./samples/hello-32 > /dev/null
./woody > /dev/null
check_return_value 2
./woody_woodpacker ./samples/hello-pie > /dev/null
./woody > /dev/null
check_return_value 3
./woody_woodpacker ./samples/hello-relocation > /dev/null 2>&1
./woody > /dev/null
check_return_value 4
./woody_woodpacker ./samples/no-std > /dev/null
./woody > /dev/null
check_return_value 5
./woody_woodpacker ./samples/nodata > /dev/null
./woody > /dev/null
check_return_value 6
./woody_woodpacker ./samples/tiny_c > /dev/null
./woody > /dev/null
check_return_value 7