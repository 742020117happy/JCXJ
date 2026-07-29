#!/bin/bash

OUTPUT_FILE="/home/nvidia/Collector_Manage/Zivid_Camera.txt"

echo "正在扫描项目文件..."

# 查找文件并统计
CPP_FILES=$(find . -type f -name "*.cpp" -not -path "./build/*" -not -path "./.git/*" | wc -l)
H_FILES=$(find . -type f -name "*.h" -not -path "./build/*" -not -path "./.git/*" | wc -l)
UI_FILES=$(find . -type f -name "*.ui" -not -path "./build/*" -not -path "./.git/*" | wc -l)
CMAKE_FILES=$(find . -type f -name "*.txt" -not -path "./build/*" -not -path "./.git/*" | wc -l)

echo "找到 $CPP_FILES 个 .cpp 文件"
echo "找到 $H_FILES 个 .h 文件"
echo "找到 $UI_FILES 个 .ui 文件"
echo "找到 $CMAKE_FILES 个 .txt 文件"

echo "开始合并到 $OUTPUT_FILE..."

{
    echo "========================================================================"
    echo "Project: Collector_Manage"
    echo "Export Time: $(date)"
    echo "Total Files: $((CPP_FILES + H_FILES))"
    echo "========================================================================"
    echo ""
    
    # 头文件
    echo "==================== HEADER FILES (.h) ===================="
    find . -type f -name "*.h" \
        -not -path "./build/*" \
        -not -path "./.git/*" \
        -not -path "./CMakeFiles/*" \
        | sort \
        | while read file; do
            echo ""
            echo "########################################################################"
            echo "FILE: $file"
            echo "########################################################################"
            echo ""
            cat "$file"
            echo ""
        done
    
    # 源文件
    echo ""
    echo "==================== SOURCE FILES (.cpp) ===================="
    find . -type f -name "*.cpp" \
        -not -path "./build/*" \
        -not -path "./.git/*" \
        -not -path "./CMakeFiles/*" \
        | sort \
        | while read file; do
            echo ""
            echo "########################################################################"
            echo "FILE: $file"
            echo "########################################################################"
            echo ""
            cat "$file"
            echo ""
        done
    # Ui
    echo "==================== QDesigner FILES (.ui) ===================="
    find . -type f -name "*.ui" \
        -not -path "./build/*" \
        -not -path "./.git/*" \
        -not -path "./CMakeFiles/*" \
        | sort \
        | while read file; do
            echo ""
            echo "########################################################################"
            echo "FILE: $file"
            echo "########################################################################"
            echo ""
            cat "$file"
            echo ""
        done  
    # Cmake
    echo "==================== Cmake FILES (.txt) ===================="
    find . -type f -name "*.txt" \
        -not -path "./build/*" \
        -not -path "./.git/*" \
        -not -path "./CMakeFiles/*" \
        | sort \
        | while read file; do
            echo ""
            echo "########################################################################"
            echo "FILE: $file"
            echo "########################################################################"
            echo ""
            cat "$file"
            echo ""
        done
    
} > "$OUTPUT_FILE"

echo "✅ 完成！输出文件: $OUTPUT_FILE"
echo "📦 文件大小: $(du -h "$OUTPUT_FILE" | cut -f1)"
