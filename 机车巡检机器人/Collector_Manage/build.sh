#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}=========================================${NC}"
echo -e "${BLUE}  Collector_Manage System Build Script${NC}"
echo -e "${BLUE}=========================================${NC}"

cd "$(dirname "$0")"

# 清理构建目录
if [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf build
fi

# 创建构建目录
mkdir -p build
cd build

# CMake 配置
echo -e "${YELLOW}Running CMake configuration...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ CMake configuration failed!${NC}"
    exit 1
fi

# 编译
echo -e "${YELLOW}Building all modules (using $(nproc) jobs)...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}  ✅ Build completed successfully!${NC}"
echo -e "${GREEN}=========================================${NC}"

# 列出编译产物
echo -e "${YELLOW}Build artifacts:${NC}"
ls -lh bin/

echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}  Executables ready to run:${NC}"
echo -e "${GREEN}    Main:    ./bin/collector_manage${NC}"
echo -e "${GREEN}    Subs:    ./bin/realsense_manage${NC}"
echo -e "${GREEN}             ./bin/robosense_manage${NC}"
echo -e "${GREEN}             ./bin/zivid_manage${NC}"
echo -e "${GREEN}             ./bin/hikvision_manage${NC}"
echo -e "${GREEN}             ./bin/transmission_client${NC}"
echo -e "${GREEN}             ./bin/transmission_server${NC}"
echo -e "${GREEN}             ./bin/hipnuc_ch10x_manage${NC}"
echo -e "${GREEN}=========================================${NC}"