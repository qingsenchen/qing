#!/bin/bash
set -e # 遇到错误立即退出

# 进入项目根目录
cd /Users/king/gitdir/qing

# 清理并创建构建目录
rm -rf build
mkdir build
cd build

# 生成构建系统
cmake ..

# 编译项目
make -j$(sysctl -n hw.logicalcpu) # 使用所有CPU核心编译

# 运行所有测试
ctest --output-on-failure

echo "✅ 构建和测试完成"