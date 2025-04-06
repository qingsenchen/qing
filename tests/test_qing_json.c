#include "utils/qing_json.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// 测试基础对象解析
void test_object_parse() {
    const char* json_str = "{ \
        \"name\": \"QingLite\", \
        \"version\": 1.2, \
        \"count\": 10, \
        \"active\": true, \
        \"modules\": [\"core\", \"math\", \"io\"] \
    }";
    
    //const char* json_str = "{\"name\": \"QingLite\"     , \"version\": 1.2, \"active\": true, \"modules\": [\"core\", \"math\", \"io\"]}";
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    
    assert(json != NULL);
    assert(json->type == QING_JSON_OBJECT);
    assert(json->count == 5);
    
    // 验证字段值
    for (size_t i = 0; i < json->count; i++) {
        const char* key = json->object.keys[i];
        qing_json_value_t* val = json->object.values[i];
        
        if (strcmp(key, "name") == 0) {
            assert(val->type == QING_JSON_STRING);
            assert(strcmp(val->string, "QingLite") == 0);
        } else if (strcmp(key, "version") == 0) {
            assert(val->type == QING_JSON_FLOAT);
            assert(val->number == 1.2);
        } else if (strcmp(key, "count") == 0) {
            assert(val->type == QING_JSON_INT);
            assert(val->integer == 10);
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
    qing_json_print_pretty(json);  // 调用打印函数

    qing_json_free(json);
    printf("[PASS] test_print_function\n");
}



void test_print_from_file(const char* path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Failed to open file: %s\n", path);
        return;
    }
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *json_str = malloc(file_size + 1);
    if (json_str == NULL) {
        printf("Failed to allocate memory for JSON string.\n");
        fclose(fp);
        return;
    }

    // 读取文件内容到json_str
    size_t read_size = fread(json_str, 1, file_size, fp);
    json_str[read_size] = '\0';  // 添加字符串结束符
    fclose(fp);
    
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    
    assert(json != NULL);
    printf("/Users/king/gitdir/Qwen-1_8B-Chat/config.json -> Print:\n");
    qing_json_print(json);  // 调用打印函数
    printf("\n");

    qing_json_free(json);
    printf("[PASS] test_print_function\n");
}

// 测试通过键获取值的功能
void test_get_value_by_key() {
    const char* json_str = "{ \
        \"name\": \"QingLite\", \
        \"version\": 1.2, \
        \"count\": 10, \
        \"active\": true, \
        \"nested\": { \
            \"key1\": \"value1\", \
            \"key2\": 42 \
        }, \
        \"modules\": [\"core\", \"math\", \"io\"] \
    }";
    
    const char* ptr = json_str;
    qing_json_value_t* json = qing_json_parse(&ptr);
    
    assert(json != NULL);
    
    // 测试获取基本类型值
    qing_json_value_t* name = qing_json_get_value(json, "name");
    assert(name != NULL);
    assert(name->type == QING_JSON_STRING);
    assert(strcmp(name->string, "QingLite") == 0);
    
    qing_json_value_t* version = qing_json_get_value(json, "version");
    assert(version != NULL);
    assert(version->type == QING_JSON_FLOAT);
    assert(version->number == 1.2);
    
    qing_json_value_t* count = qing_json_get_value(json, "count");
    assert(count != NULL);
    assert(count->type == QING_JSON_INT);
    assert(count->integer == 10);
    
    qing_json_value_t* active = qing_json_get_value(json, "active");
    assert(active != NULL);
    assert(active->type == QING_JSON_BOOL);
    assert(active->boolean == 1);
    
    // 测试获取嵌套对象
    qing_json_value_t* nested = qing_json_get_value(json, "nested");
    assert(nested != NULL);
    assert(nested->type == QING_JSON_OBJECT);
    
    // 测试从嵌套对象中获取值
    qing_json_value_t* key1 = qing_json_get_value(nested, "key1");
    assert(key1 != NULL);
    assert(key1->type == QING_JSON_STRING);
    assert(strcmp(key1->string, "value1") == 0);
    
    // 测试获取数组
    qing_json_value_t* modules = qing_json_get_value(json, "modules");
    assert(modules != NULL);
    assert(modules->type == QING_JSON_ARRAY);
    assert(modules->count == 3);
    
    // 测试获取不存在的键
    qing_json_value_t* not_exist = qing_json_get_value(json, "not_exist");
    assert(not_exist == NULL);
    
    qing_json_free(json);
    printf("[PASS] test_get_value_by_key\n");
}

int main() {
    test_object_parse();
    test_nested_structure();
    test_error_handling();
    test_print_function();
    test_get_value_by_key(); 
    test_print_from_file("/Users/king/gitdir/Qwen-1_8B-Chat/config.json");

    printf("\nAll JSON tests passed!\n");
    return 0;
}