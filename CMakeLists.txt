# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(qing LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# 设置编译选项
set(CMAKE_C_FLAGS "-Wall -g")

# 指定项目的头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# 设置源代码文件目录
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# 设置测试目录
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)


# 添加核心源文件
set(SOURCE_FILES
    ${SRC_DIR}/qing_tensor.c
    ${SRC_DIR}/qing_ops.c
    ${SRC_DIR}/qing_model.c
    ${SRC_DIR}/qing_backend.c
    ${SRC_DIR}/qing_utils.c
    ${SRC_DIR}/qing_graph.c
)

# 添加测试源文件
set(TEST_FILES
    ${TEST_DIR}/test_qing_tensor.c
    ${TEST_DIR}/test_qing_ops.c
    ${TEST_DIR}/test_qing_model.c
    ${TEST_DIR}/test_qing_backend.c
    ${TEST_DIR}/test_qing_utils.c
    ${TEST_DIR}/test_qing_graph.c
)

# 设置可执行文件
# add_executable(qing ${SOURCE_FILES})
add_library(qing STATIC ${SOURCE_FILES})

# 添加测试可执行文件
add_executable(test_qing_graph ${TEST_DIR}/test_qing_graph.c)
add_executable(test_qing_ops ${TEST_DIR}/test_qing_ops.c)  # 新增测试目标
add_executable(test_qing_tensor ${TEST_DIR}/test_qing_tensor.c)

# 统一设置包含目录和链接库
foreach(test_name test_qing_graph test_qing_ops test_qing_tensor)
    target_include_directories(${test_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${test_name} qing)
endforeach()

# 添加 CTest 支持
enable_testing()

# 添加到 CTest
add_test(NAME test_qing_graph COMMAND test_qing_graph)
add_test(NAME test_qing_ops COMMAND test_qing_ops)  # 新增测试条目
add_test(NAME test_qing_tensor COMMAND test_qing_tensor)

# 目标链接库
# 如果有依赖库，可以在这里链接
# target_link_libraries(qing <LIBRARIES>)

# 为 CMake 提供一些输出信息
message(STATUS "Source files: ${SOURCE_FILES}")
message(STATUS "Test files: ${TEST_FILES}")
