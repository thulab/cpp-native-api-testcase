#ifndef PLM_TEST_CONFIG_H
#define PLM_TEST_CONFIG_H

#include <cstdio>
#include <cstdlib>
#include <string>

/**
 * PLM / 本地执行连接参数。
 *
 * PLM 执行时由平台注入 IOTDB_HOST / IOTDB_PORT（K8s 实例地址）；
 * 账号口令默认 root/TimechoDB@2021，可用 IOTDB_USER / IOTDB_PASSWORD 覆盖。
 * 缺少 K8s 地址时直接退出，避免测试悄悄连接执行机本地默认库。
 */
inline std::string plmEnv(const char* name, const char* fallback) {
    const char* value = std::getenv(name);
    if (value == nullptr || value[0] == '\0') {
        return std::string(fallback);
    }
    return std::string(value);
}

inline bool plmExecutionMode() {
    const char* value = std::getenv("PLM_AUTOMATION_TYPE");
    return value != nullptr && value[0] != '\0';
}

inline void plmRequireEndpoint() {
    if (!plmExecutionMode()) {
        return;
    }
    const char* host = std::getenv("IOTDB_HOST");
    const char* port = std::getenv("IOTDB_PORT");
    if (host == nullptr || host[0] == '\0' || port == nullptr || port[0] == '\0') {
        std::fprintf(stderr,
                     "[plm] PLM 执行必须提供 K8s 实例连接地址 IOTDB_HOST/IOTDB_PORT\n");
        std::exit(2);
    }
}

inline std::string plmHost() {
    plmRequireEndpoint();
    return plmEnv("IOTDB_HOST", "127.0.0.1");
}

inline int plmPort() {
    plmRequireEndpoint();
    return std::stoi(plmEnv("IOTDB_PORT", "6667"));
}

inline std::string plmUser() {
    return plmEnv("IOTDB_USER", "root");
}

inline std::string plmPassword() {
    return plmEnv("IOTDB_PASSWORD", "TimechoDB@2021");
}

#endif  // PLM_TEST_CONFIG_H
