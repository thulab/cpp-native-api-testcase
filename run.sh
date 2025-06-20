#!/bin/bash

function example() {
    # 检查文件是否存在并且是可执行的
    if [ -x "./build/example/$filename" ]; then
        echo "/******* Start test ********/"
        cd ./build/example
        # 执行文件
        ./"$filename"
    else
        # 文件不存在或不可执行，打印错误信息
        echo "!!!错误:build/test/tree 目录下 $filename 文件不存在或不是可执行文件。"
    fi
}

function test() {
    # 检查文件是否存在并且是可执行的
    if [ -x "./build/test/$filename" ]; then
        echo "/******* Start test ********/"
        cd ./build/test
        # 执行文件
        ./"$filename" --gtest_output="json:cpp_session_test_report.json"
    else
        # 文件不存在或不可执行，打印错误信息
        echo "!!!错误:build/test 目录下 $filename 文件不存在或不是可执行文件。"
    fi
}

# 默认测试模式
mode="test"
filename="main"

# 根据输入的模式执行对应的测试
while getopts ":m:" opt ; do
    # 根据选项执行不同的操作
    case $opt in
        m)
            mode=$OPTARG
            ;;
        *)
            echo "无效的选项: -$OPTARG" >&2
            exit 1
            ;;
    esac
done

# 根据模式执行对应的函数
case $mode in
    example)
        example
        exit 0
        ;;
    test)
        test
        exit 0
        ;;
    *)
        echo "输入的模型无效，请输入 example 或 test (区分大小写)"
        exit 1
        ;;
esac

