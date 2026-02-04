#pragma once
#include <functional>
#include <vector>

template <typename... Args>
class GameEvent
{
private:
    using Handler = std::function<void(Args...)>;
    std::vector<Handler> handlers;
public:
    // 连接事件处理器（返回ID用于取消）
    size_t connect(Handler handler) {
        handlers.push_back(handler);
        return handlers.size() - 1;
    }
    
    // 重载 += 操作符，更直观
    size_t operator+=(Handler handler) {
        return connect(handler);
    }

    void disconnect(size_t id) {
        if (id < handlers.size()) {
            handlers[id] = nullptr;  // 标记为null，不删除保持索引稳定
        }
    }

    void emit(Args... args) {
        for (auto& handler : handlers) {
            handler(args...);
        }
    }
};