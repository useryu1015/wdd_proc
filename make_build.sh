#!/bin/sh

if [ "$1" == "aarch" ];then
    export PATH=$PATH:/opt/arm_gcc/aarch64/aarch64-himix100-linux/bin/
    cmake -DCMAKE_TOOLCHAIN_FILE=$cross_dir/CrossAARCH64.cmake $dir
elif [ "$1" == "scm801" ];then
    export PATH=$PATH:/opt/arm_gcc/arm_linaro/bin
    cmake -DCMAKE_TOOLCHAIN_FILE=$cross_dir/CrossSCM801.cmake $dir
elif [ "$1" == "imx" ];then
    # 需要根据刘淼提供的开发吧文档， 配置CC和CXX   等
    make
elif [ "$1" == "clean" ];then
    make clean
else
    make -f makefile_x86
fi


