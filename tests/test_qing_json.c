#include "utils/qing_json.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// 测试基础对象解析
void test_object_parse() {
    const char* json_str = "{ \
        \"name\": \"QingLite\", \
        \"version\": 1.2, \
        \"active\": true, \
        \"modules\": [\"core\", \"math\", \"io\"] \
    }";
    
    //const char* json_str = "{\"name\": \"QingLite\"     , \"version\": 1.2, \"active\": true, \"modules\": [\"core\", \"math\", \"io\"]}";
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    
    assert(json != NULL);
    assert(json->type == QING_JSON_OBJECT);
    assert(json->count == 4);
    
    // 验证字段值
    for (size_t i = 0; i < json->count; i++) {
        const char* key = json->object.keys[i];
        qing_json_value_t* val = json->object.values[i];
        
        if (strcmp(key, "name") == 0) {
            assert(val->type == QING_JSON_STRING);
            assert(strcmp(val->string, "QingLite") == 0);
        } else if (strcmp(key, "version") == 0) {
            assert(val->type == QING_JSON_NUMBER);
            assert(val->number == 1.2);
        } else if (strcmp(key, "active") == 0) {
            assert(val->type == QING_JSON_BOOL);
            assert(val->boolean == 1);
        } else if (strcmp(key, "modules") == 0) {
            assert(val->type == QING_JSON_ARRAY);
            assert(val->count == 3);
        }
    }
    
    qing_json_free(json);
    printf("[PASS] test_object_parse\n");
}

// 测试嵌套结构解析
void test_nested_structure() {
    const char* json_str = "{ \
        \"matrix\": [[1.1, 2.2], [3.3, 4.4]], \
        \"metadata\": { \
            \"author\": \"Qing Team\", \
            \"license\": \"Apache-2.0\" \
        } \
    }";
    
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    
    assert(json != NULL);
    assert(json->type == QING_JSON_OBJECT);
    
    // 验证矩阵数据
    qing_json_value_t* matrix = json->object.values[0];
    assert(matrix->type == QING_JSON_ARRAY);
    assert(matrix->count == 2);
    
    for (size_t i = 0; i < matrix->count; i++) {
        qing_json_value_t* row = matrix->array[i];
        assert(row->type == QING_JSON_ARRAY);
        assert(row->count == 2);
    }
    
    // 验证元数据
    qing_json_value_t* metadata = json->object.values[1];
    assert(metadata->type == QING_JSON_OBJECT);
    assert(metadata->count == 2);
    
    qing_json_free(json);
    printf("[PASS] test_nested_structure\n");
}

// 测试错误格式处理
void test_error_handling() {
    const char* invalid_jsons[] = {
        "{ \"key\": undefined }",  // 无效值
        "[1, 2, 3,]",             // 尾部逗号
        "{ missing_quotes: 42 }",  // 缺少引号
        NULL
    };
    
    for (int i = 0; invalid_jsons[i]; i++) {
        const char* ptr = invalid_jsons[i];
        qing_json_value_t* json = qing_json_parse(&ptr);
        printf("ptr = %s\n", ptr);
        assert(json == NULL);
    }
    printf("[PASS] test_error_handling\n");
}

// 测试打印功能
void test_print_function() {
    const char* json_str = "{ \
        \"name\": \"QingLite\", \
        \"version\": 1.2, \
        \"active\": true, \
        \"modules\": [\"core\", \"math\", \"io\"] \
    }";
    
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    
    assert(json != NULL);
    printf("JSON Print Output:\n");
    qing_json_print(json);  // 调用打印函数
    printf("\n");

    qing_json_free(json);
    printf("[PASS] test_print_function\n");
}

int main() {
    test_object_parse();
    test_nested_structure();
    test_error_handling();
    test_print_function();  // 新增：调用打印测试
    printf("\nAll JSON tests passed!\n");
    return 0;
}