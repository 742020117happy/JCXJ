cmake_minimum_required(VERSION 3.16)
project(Transmission_Server VERSION 1.0.0 LANGUAGES C CXX CUDA)

# ============================================================================
# 策略与平台检测
# ============================================================================
if(POLICY CMP0144)
    cmake_policy(SET CMP0144 OLD)
endif()

# 检测ARM64平台
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARM64 TRUE)
    message(STATUS "✅ Detected ARM64 Linux platform (Jetson)")
endif()

# ============================================================================
# 构建配置
# ============================================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(NOT CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 120 CACHE STRING "CUDA Architectures" FORCE)
endif()

if(DEFINED ENV{CUDA_HOME})
    set(CUDA_HOME $ENV{CUDA_HOME})
    set(CMAKE_CUDA_COMPILER ${CUDA_HOME}/bin/nvcc)
endif()
if(DEFINED ENV{OpenCV_DIR})
    set(OpenCV_DIR $ENV{OpenCV_DIR})
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
# 查找依赖包（含完善版librealsense2配置）
# ============================================================================
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network PrintSupport Charts WebSockets Concurrent OpenGL)
find_package(OpenCV REQUIRED)
find_package(Zivid REQUIRED COMPONENTS Core Visualization)
find_package(PCL 1.14 REQUIRED COMPONENTS common io visualization filters kdtree search sample_consensus)
find_package(VTK 9.1 REQUIRED COMPONENTS CommonCore CommonDataModel FiltersCore RenderingCore RenderingOpenGL2 GUISupportQt)
find_package(Boost REQUIRED COMPONENTS system thread)

# ============================================================================
# librealsense2 配置（完善版）
# ============================================================================
# 优先使用 find_package（现代CMake方式）
find_package(realsense2 QUIET)
if(realsense2_FOUND)
    message(STATUS "✅ librealsense2 found via find_package: ${realsense2_VERSION}")
    set(REALSENSE2_FOUND TRUE)
    set(REALSENSE2_INCLUDE_DIRS ${realsense2_INCLUDE_DIRS})
    set(REALSENSE2_LIBRARIES realsense2::realsense2)  # 使用导入目标
else()
    # 回退到 pkg-config（传统方式）
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(REALSENSE2 QUIET realsense2)
        if(REALSENSE2_FOUND)
            message(STATUS "✅ librealsense2 found via pkg-config: ${REALSENSE2_VERSION}")
            # 修复pkg-config路径（Jetson常见问题）
            if(ARM64 AND NOT REALSENSE2_INCLUDE_DIRS)
                set(REALSENSE2_INCLUDE_DIRS "/usr/include/librealsense2")
            endif()
        else()
            message(WARNING "⚠️  librealsense2 not found (optional feature disabled)")
        endif()
    else()
        message(WARNING "⚠️  pkg-config not found, cannot search for librealsense2")
    endif()
endif()

# 验证librealsense2路径（Jetson关键修复）
if(REALSENSE2_FOUND AND ARM64)
    if(NOT EXISTS "${REALSENSE2_INCLUDE_DIRS}/librealsense2/rs.hpp")
        # 尝试备用路径
        if(EXISTS "/usr/include/librealsense2/rs.hpp")
            set(REALSENSE2_INCLUDE_DIRS "/usr/include/librealsense2")
            message(STATUS "🔧 Fixed librealsense2 include path: ${REALSENSE2_INCLUDE_DIRS}")
        endif()
    endif()
endif()

# ============================================================================
# 海康SDK路径配置
# ============================================================================
set(HIKVISION_SDK_ROOT "/opt/hikvision_sdk_arm64" CACHE PATH "Path to Hikvision SDK for ARM64")
set(HIKVISION_INC_PATH "${HIKVISION_SDK_ROOT}/incCn")
set(HIKVISION_LIB_PATH "${HIKVISION_SDK_ROOT}/MakeAll")
set(HIKVISION_COMP_PATH "${HIKVISION_SDK_ROOT}/MakeAll/HCNetSDKCom")

if(NOT EXISTS "${HIKVISION_INC_PATH}")
    message(FATAL_ERROR "❌ Hikvision SDK include directory not found at ${HIKVISION_INC_PATH}")
endif()
if(NOT EXISTS "${HIKVISION_LIB_PATH}/libhcnetsdk.so")
    message(FATAL_ERROR "❌ Hikvision SDK core library not found at ${HIKVISION_LIB_PATH}/libhcnetsdk.so")
endif()

# ============================================================================
# 包含目录（严格顺序 + librealsense2修复）
# ============================================================================
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${HIKVISION_INC_PATH}
    ${PCL_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
    ${ZIVID_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${REALSENSE2_INCLUDE_DIRS} 
)

# ============================================================================
# 源文件
# ============================================================================
set(SOURCES
     main.cpp    
     TCP_Server.cpp  
     Transmission_Server.cpp  
     Transmission_Server.qrc  
     Variable.cpp
     Fr_Light.cpp             
     Transmission_Server.ui  
)

set(HEADERS
    Fr_Light.h
    TCP_Server.h   
    Transmission_Server.h
    Variable.h
)

# ============================================================================
# 创建可执行文件
# ============================================================================
add_executable(transmission_server ${SOURCES} ${HEADERS})

# ============================================================================
# 链接库（含librealsense2完善配置）
# ============================================================================
target_link_libraries(transmission_server PRIVATE
    # Qt5
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::PrintSupport
    Qt5::Charts Qt5::WebSockets Qt5::Concurrent Qt5::OpenGL
    
    # Zivid
    Zivid::Core Zivid::Visualization
    
    # OpenCV
    ${OpenCV_LIBS}
    
    # PCL + VTK
    ${PCL_LIBRARIES}
    ${VTK_LIBRARIES}
    
    # Boost
    ${Boost_LIBRARIES}
    
    # ============================================================================
    # librealsense2（完善配置）
    # ============================================================================
    ${REALSENSE2_LIBRARIES}  # 自动处理导入目标或库列表
    
    # ============================================================================
    # 海康SDK（严格按文档顺序）
    # ============================================================================
    ${HIKVISION_LIB_PATH}/libhcnetsdk.so
    ${HIKVISION_LIB_PATH}/libHCCore.so
    ${HIKVISION_LIB_PATH}/libhpr.so
    ${HIKVISION_LIB_PATH}/libPlayCtrl.so
    ${HIKVISION_LIB_PATH}/libSuperRender.so
    ${HIKVISION_LIB_PATH}/libAudioRender.so
    ${HIKVISION_COMP_PATH}/libHCCoreDevCfg.so
    ${HIKVISION_COMP_PATH}/libHCPreview.so
    ${HIKVISION_COMP_PATH}/libHCPlayBack.so
    ${HIKVISION_COMP_PATH}/libHCVoiceTalk.so
    ${HIKVISION_COMP_PATH}/libHCAlarm.so
    ${HIKVISION_COMP_PATH}/libHCDisplay.so
    ${HIKVISION_COMP_PATH}/libHCIndustry.so
    ${HIKVISION_COMP_PATH}/libHCGeneralCfgMgr.so
    ${HIKVISION_COMP_PATH}/libStreamTransClient.so
    ${HIKVISION_COMP_PATH}/libSystemTransform.so
    ${HIKVISION_COMP_PATH}/libanalyzedata.so
    ${HIKVISION_COMP_PATH}/libAudioIntercom.so
    ${HIKVISION_COMP_PATH}/libiconv.so.2
)

# ============================================================================
# RPATH与编译优化
# ============================================================================
set_target_properties(transmission_server PROPERTIES
    BUILD_RPATH "${HIKVISION_LIB_PATH}:${HIKVISION_COMP_PATH}"
    INSTALL_RPATH "${HIKVISION_LIB_PATH}:${HIKVISION_COMP_PATH}"
    BUILD_RPATH_USE_ORIGIN ON
    INSTALL_RPATH_USE_LINK_PATH TRUE
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
)

target_compile_options(transmission_server PRIVATE
    -O3 -mcpu=native
    -Wall -Wextra
    -Wno-unused-parameter -Wno-unused-variable -Wno-deprecated-declarations
)

# ============================================================================
# 资源文件复制
# ============================================================================
add_custom_command(TARGET transmission_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Communicate_DB.json
        ${CMAKE_CURRENT_SOURCE_DIR}/stuqss.css
        $<TARGET_FILE_DIR:transmission_server>/
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Images
        $<TARGET_FILE_DIR:transmission_server>/Images
    COMMENT "Copying resources to build directory"
)

# ============================================================================
# 构建信息输出
# ============================================================================
message(STATUS "========================================")
message(STATUS "✅ Project Configuration Summary")
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CUDA Architecture: ${CMAKE_CUDA_ARCHITECTURES}")
message(STATUS "OpenCV: ${OpenCV_VERSION}")
message(STATUS "PCL: ${PCL_VERSION}")
message(STATUS "VTK: ${VTK_VERSION}")
message(STATUS "Zivid SDK: ${ZIVID_VERSION}")
message(STATUS "librealsense2: ${REALSENSE2_VERSION} (${REALSENSE2_FOUND})")
message(STATUS "Hikvision SDK: ${HIKVISION_SDK_ROOT}")
message(STATUS "ARM64 Platform: ${ARM64}")
message(STATUS "========================================")
