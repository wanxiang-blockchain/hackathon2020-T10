一、目录说明：
├── app                         ----- 考试场景源码
├── bin                         ----- 目标文件目录
├── env.sh                    	----- 项目编译环境变量脚本  
├── include                     ----- 项目头文件目录
├── lib                         ----- 项目库文件目录
├── sdk                         ----- 区块链存证功能实现源码
│   ├── apis
│   ├── cjson
│   ├── CMakeLists.txt
│   ├── ecdsa
├── start.sh					----- 项目编译脚本  
└── vendor						----- 第三方源码目录
    ├── curl-7.72.0.tar.gz
    └── mqtt

二、编译说明：
1.配置环境变量：
source env.sh
2.编译说明
./start.sh 			：编译所有
./start.sh clean 	：clean所有
./start.sh sdk		：只编译sdk
./start.sh demo		：只编译demo
./start.sh curl		：只编译curl
./start.sh mqtt		：只编译mqtt
./start.sh vendor	：编译所有第三方软件
