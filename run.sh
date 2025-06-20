#!/bin/bash

function tree_test() {
    # 检查文件是否存在并且是可执行的
    if [ -x "./build/test/test/tree/$filename" ]; then
        echo "/******* Start test ********/"
        cd ./build/test/tree
        # 执行文件
        ./main
    else
        # 文件不存在或不可执行，打印错误信息
        echo "!!!错误:build/test/tree 目录下 $filename 文件不存在或不是可执行文件。"
    fi
}

function table_test() {
    # 检查文件是否存在并且是可执行的
    if [ -x "./build/test/table/$filename" ]; then
        echo "/******* Start test ********/"
        cd ./build/test/table
        # 执行文件
        ./main
    else
        # 文件不存在或不可执行，打印错误信息
        echo "!!!错误:build/test/table 目录下 $filename 文件不存在或不是可执行文件。"
    fi
}

# 根据输入的模式执行对应的测试
while getopts ":m:" opt ; do
    # 根据选项执行不同的操作
    case $opt in
        m)
            case $OPTARG in
                tree)
                    tree_test
                    exit 0
                    ;;
                table)
                    table_test
                    exit 0
                    ;;
                all)
                    tree_test
                    table_test
                    exit 0
                    ;;
                *)
                    echo "输入的模型无效，请输入 tree 或 table (区分大小写)"
                    exit 1
                    ;;
            esac
            ;;
        *)
            echo "无效的选项: -$OPTARG" >&2
            exit 1
            ;;
    esac
done

# 提示用户输入测试的模型
echo "请输入测试的模型(tree/table):"
read mode
filename="main"
case $mode in
    tree)
        tree_test
        ;;
    table)
        table_test
        ;;
    *)
        echo "输入的模型无效，请输入 tree 或 table(区分大小写)"
        exit 1
        ;;
esac

