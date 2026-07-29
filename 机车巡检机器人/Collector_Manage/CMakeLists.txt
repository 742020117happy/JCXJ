cmake_minimum_required(VERSION 3.16)
project(Collector_Manage_System VERSION 1.0.0 LANGUAGES C CXX CUDA)

# ============================================================================
# 全局配置
# ============================================================================
if(POLICY CMP0144)
    cmake_policy(SET CMP0144 OLD)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARM64 TRUE)
    message(STATUS "✅ Detected ARM64 Linux platform (Jetson)")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(NOT CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 120 CACHE STRING "CUDA Architectures" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(PCL_NO_MPI ON)

# ============================================================================
# 统一输出目录
# ============================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================================
# 添加所有子项目（8 个模块）
# ============================================================================
message(STATUS "===========================================")
message(STATUS "  Adding Subdirectories...")
message(STATUS "===========================================")

add_subdirectory(Collector_Manage)      # 主进程
add_subdirectory(RealSense)             # 子进程
add_subdirectory(RoboSense)             # 子进程
add_subdirectory(Zivid_Camera)          # 子进程
add_subdirectory(Hikvision)             # 子进程
add_subdirectory(Transmission_Server)   # 子进程
add_subdirectory(HIPNUC_CH10X)          # 子进程 (CH10X 惯导)

# ============================================================================
# 构建信息汇总
# ============================================================================
message(STATUS "===========================================")
message(STATUS "  ✅ Collector_Manage System Build Summary")
message(STATUS "===========================================")
message(STATUS "  Main Process:    collector_manage")
message(STATUS "  Sub Processes:   realsense_manage")
message(STATUS "                   robosense_manage")
message(STATUS "                   zivid_manage")
message(STATUS "                   hikvision_manage")
message(STATUS "                   transmission_server")
message(STATUS "                   hipnuc_ch10x_manage") 
message(STATUS "  Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "===========================================")