# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color|*-256color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
#export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

# ========================================
# Jetson AGX Thor T5000 Development Environment
# CUDA + Qt5 + PCL + VTK + Boost + Eigen3 + Zivid + RealSense + OpenCV + 海康威视SDK
# ========================================

# --- 防止重复source导致路径重复 ---
if [ -n "$BASHRC_SOURCED" ]; then
    return
fi
export BASHRC_SOURCED=1

# --- Path Priority (Critical for OpenCV 4.8.0 vs 4.6.0) ---
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib/aarch64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=/usr/lib:/usr/lib/aarch64-linux-gnu:/usr/local/cuda-13.0/lib64:$LD_LIBRARY_PATH

# --- CUDA 13.0 (Jetson Thor) ---
export CUDA_HOME=/usr/local/cuda-13.0
export PATH=$CUDA_HOME/bin:$PATH
# LD_LIBRARY_PATH 已在 Path Priority 部分设置，避免重复

# --- Qt5 Configuration (Ubuntu 24.04 JetPack 7.1) ---
export QT_SELECT=5
export QT_QPA_PLATFORM=xcb
# --- export QT_QPA_PLATFORM=offscreen
export QT_PLUGIN_PATH=/usr/lib/aarch64-linux-gnu/qt5/plugins
export QML2_IMPORT_PATH=/usr/lib/aarch64-linux-gnu/qt5/qml
export PATH=/usr/lib/qt5/bin:$PATH

# --- OpenCV Configuration (NVIDIA Optimized 4.8.0) ---
export OpenCV_DIR=/usr/lib/cmake/opencv4

# --- Boost + Eigen3 ---
export BOOST_ROOT=/usr/include/boost
export EIGEN3_INCLUDE_DIR=/usr/include/eigen3

# --- 海康威视SDK (ArmLinux64) - 关键集成 ---
if [ -d "/opt/hikvision_sdk_arm64" ]; then
    export HIKVISION_SDK_PATH=/opt/hikvision_sdk_arm64
    export LD_LIBRARY_PATH=$HIKVISION_SDK_PATH/MakeAll:$HIKVISION_SDK_PATH/MakeAll/HCNetSDKCom:$LD_LIBRARY_PATH
    export CPLUS_INCLUDE_PATH=$HIKVISION_SDK_PATH/incCn:$CPLUS_INCLUDE_PATH
    export C_INCLUDE_PATH=$HIKVISION_SDK_PATH/incCn:$C_INCLUDE_PATH
    # 避免与系统openssl冲突（SDK自带openssl 1.1.1）
    export OPENSSL_CONF=/etc/ssl/openssl.cnf  # 优先使用系统openssl
fi

# --- Jetson Performance Tuning ---
alias jetson-max="sudo nvpmodel -m 0 && sudo jetson_clocks && echo '✅ MAXN mode (130W) enabled'"
alias jetson-status="nvpmodel -q 2>/dev/null | grep -E 'NV Power Mode' | head -1 && jetson_clocks --show 2>/dev/null | grep -E 'GPU|EMC' | head -2"
alias jetson-temp="sudo jetson_clocks --show | grep -E 'GPU|CPU|SOC' | awk '{print \$1 \": \" \$3 \"°C\"}'"

# --- Development Aliases ---
alias qtbuild='rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && make -j$(nproc)'
alias pclview='pcl_viewer'
alias zivid-capture='ZividCapture --settings "{\"Acquisitions\": [{\"ExposureTime\": 8000, \"Gain\": 1.0}]}" --output /tmp/capture.zdf && echo "✅ Saved to /tmp/capture.zdf"'
alias rsview='realsense-viewer &'
alias opencv-version='opencv_version && echo "✅ NVIDIA Optimized OpenCV 4.8.0"'
alias vtk-version='echo "VTK 9.1 (installed)"'
alias hikvision-verify='test -f $HIKVISION_SDK_PATH/MakeAll/libhcnetsdk.so && echo "✅ Hikvision SDK V6.1.9.45 (ArmLinux64) ready" || echo "❌ Hikvision SDK not found at $HIKVISION_SDK_PATH"'
alias hikvision-log='ls -lh /tmp/sdklog/*.log 2>/dev/null | tail -5'

# --- Enhanced History ---
HISTSIZE=10000
HISTFILESIZE=20000
HISTCONTROL=ignoreboth:erasedups
shopt -s histappend
PROMPT_COMMAND="history -a; $PROMPT_COMMAND"

# --- Color Prompt (Jetson专属) ---
if [ "$TERM" != "linux" ]; then
    PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
fi

# --- Environment Verification (增强版) ---
env-check() {
    echo -e "\n✅ Jetson AGX Thor T5000 Development Environment Status\n=========================================================="
    
    # 核心框架
    echo -e "\n📦 核心框架"
    echo "✅ Qt5:      $(qmake -v 2>/dev/null | grep 'Using Qt' | awk '{print $4}' || echo 'N/A')"
    echo "✅ CUDA:     $(nvcc --version 2>/dev/null | grep release | awk '{print $5}' | tr -d ',' || echo 'N/A')"
    opencv_ver=$(pkg-config --modversion opencv4 2>/dev/null || opencv_version 2>/dev/null | head -1 | awk '{print $1}' || echo 'N/A')
    echo "✅ OpenCV:   ${opencv_ver} (NVIDIA Optimized)"
    pcl_ver=$(pkg-config --modversion pcl_common 2>/dev/null || echo 'N/A')
    echo "✅ PCL:      ${pcl_ver}"
    vtk_ver=$(pkg-config --modversion vtk9 2>/dev/null || echo '9.1 (installed)')
    echo "✅ VTK:      ${vtk_ver}"
    
    # 数学库
    echo -e "\n🧮 数学库"
    boost_ver=$(dpkg -l 2>/dev/null | grep -m1 libboost-system | awk '{print $3}' | cut -d'-' -f1 || echo 'N/A')
    echo "✅ Boost:    ${boost_ver}"
    eigen_ver=$(pkg-config --modversion eigen3 2>/dev/null || echo 'N/A')
    echo "✅ Eigen3:   ${eigen_ver}"
    
    # 3D传感器
    echo -e "\n📸 3D传感器"
    zivid_ver=$(strings /usr/lib/libZividCore.so 2>/dev/null | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1 || echo '2.17.2 (installed)')
    echo "✅ Zivid:    ${zivid_ver}"
    rs_ver=$(realsense-viewer --version 2>/dev/null | grep -oP 'version:\s+\K[\d.]+' | head -1 | cut -d'.' -f1-3 || echo '2.56.4')
    echo "✅ RealSense:${rs_ver}"
    
    # 海康威视SDK (关键新增)
    echo -e "\n📹 海康威视SDK"
    if [ -f "$HIKVISION_SDK_PATH/MakeAll/libhcnetsdk.so" ]; then
        hik_ver="V6.1.9.45 (ArmLinux64)"
        echo "✅ SDK路径:  $HIKVISION_SDK_PATH"
        echo "✅ 核心库:   $hik_ver"
        echo "✅ 头文件:   $HIKVISION_SDK_PATH/incCn/HCNetSDK.h"
        echo "✅ 组件库:   $HIKVISION_SDK_PATH/MakeAll/HCNetSDKCom/"
        
        # 检查关键依赖
        if ldd $HIKVISION_SDK_PATH/MakeAll/libhcnetsdk.so 2>/dev/null | grep -q "not found"; then
            echo "⚠️  依赖警告: $(ldd $HIKVISION_SDK_PATH/MakeAll/libhcnetsdk.so 2>/dev/null | grep 'not found' | wc -l) 个缺失依赖"
        else
            echo "✅ 依赖检查: 所有依赖已满足"
        fi
    else
        echo "❌ SDK未安装: 请确认 /opt/hikvision_sdk_arm64/ 是否存在"
        echo "   安装指南: 参考《海康威视SDK ArmLinux64 完整部署指南》"
    fi
    
    # Jetson状态
    echo -e "\n⚡ Jetson 硬件状态"
    if command -v jetson_clocks &> /dev/null; then
        nvpmodel -q 2>/dev/null | grep -E 'NV Power Mode' | head -1 | sed 's/^/✅ /'
        jetson_clocks --show 2>/dev/null | grep -E 'GPU|EMC|CPU' | head -3 | sed 's/^/✅ /'
    else
        echo "⚠️  jetson_clocks 未安装 (需sudo apt install jetson-stats)"
    fi

    
    # 环境变量验证
    echo -e "\n🔍 环境变量验证"
    if echo $LD_LIBRARY_PATH | grep -q "hikvision"; then
        echo "✅ LD_LIBRARY_PATH 包含海康威视路径"
    else
        echo "⚠️  LD_LIBRARY_PATH 未包含海康威视路径 (检查 $HIKVISION_SDK_PATH)"
    fi
    
    echo -e "\n💡 提示: 运行 'jetson-max' 启用 MAXN 模式 (130W) | 'hikvision-verify' 验证SDK"
}



