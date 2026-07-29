#!/bin/bash

cd /home/nvidia/Collector_Manage/Collector_Manage/
sh export_code.sh

cd /home/nvidia/Collector_Manage/Hikvision/
sh export_code.sh

cd /home/nvidia/Collector_Manage/RealSense/
sh export_code.sh

cd /home/nvidia/Collector_Manage/RoboSense/
sh export_code.sh

cd /home/nvidia/Collector_Manage/Transmission_Server/
sh export_code.sh

cd /home/nvidia/Collector_Manage/Zivid_Camera/
sh export_code.sh

cd /home/nvidia/Collector_Manage/HIPNUC_CH10X/
sh export_code.sh

tree -I 'build|.git|CMakeFiles|.vscode|*.o' -L 4 > directory_tree.txt

