#!/bin/bash
echo "hi"

lib_path="/1.RunEnv/lib/"
fil_path=`pwd`

echo "Add lib Path: ${fil_path}${lib_path}"

export LD_LIBRARY_PATH="${fil_path}${lib_path}:$LD_LIBRARY_PATH"



