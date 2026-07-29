#!/bin/sh

export QT_PLUGIN_PATH=./plugins
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./qtlib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./plugins/platforms/lib
sudo chmod a+x LidarAssistant
./LidarAssistant
