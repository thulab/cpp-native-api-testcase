#ifndef PLM_TEST_CONFIG_H
#define PLM_TEST_CONFIG_H

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include "SessionBuilder.h"
#include "TableSessionBuilder.h"

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

/**
 * 重定向开关：K8s 单入口实例（客户端拉到 *.svc.cluster.local 无法解析）默认关闭；
 * PVE 多节点拓扑（1C2D/1C4D）下客户端连首个 DataNode，服务端重定向到其它
 * DataNode 的可达 IP，通过 PLM_ENABLE_REDIRECTIONS=true 打开以验证重定向。
 */
inline bool plmRedirectEnabled() {
    const char* value = std::getenv("PLM_ENABLE_REDIRECTIONS");
    return value != nullptr && (std::string(value) == "true" || std::string(value) == "1");
}

/**
 * 表模型 Builder：关闭自动获取节点与重定向。
 *
 * K8s 实例的外部地址是唯一的可访问入口；客户端自动拉取节点列表会拿到
 * DataNode 的集群内部 DNS（*.svc.cluster.local），执行机上无法解析。
 * 只连接平台注入的外部地址，避免被导向内部 DNS。
 */
inline TableSessionBuilder* plmTableBuilder() {
    auto* builder = new TableSessionBuilder();
    builder->enableAutoFetch = false;
    builder->enableRedirections = plmRedirectEnabled();
    return builder;
}

/** 树模型会话：关闭自动获取节点与重定向，直接使用平台注入的外部连接地址。 */
inline std::shared_ptr<Session> plmTreeSession() {
    auto builder = SessionBuilder();
    builder.host(plmHost())
        ->rpcPort(plmPort())
        ->username(plmUser())
        ->password(plmPassword())
        ->enableAutoFetch(false)
        ->enableRedirections(plmRedirectEnabled());
    return builder.build();
}

#endif  // PLM_TEST_CONFIG_H
