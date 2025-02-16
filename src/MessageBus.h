#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>

class MessageBus {
    public:
        template<typename... Args, typename Callable>
        void subscribe(const char* topic, Callable&& callback) {
            subscriptions[std::string(topic)].push_back(
                std::make_shared<Subscription<Args...>>(
                    std::function<void(Args...)>(std::forward<Callable>(callback))
                )
            );
        }
    
        template<typename... Args>
        void publish(const char* topic, Args... args) {
            auto it = subscriptions.find(std::string(topic));
            if (it == subscriptions.end()) return;

            for (const auto& callback : it->second) {
                static_cast<Subscription<Args...>*>(callback.get())->func(args...);
            }
        }
    
    private:
        template<typename... Args>
        struct Subscription {
            std::function<void(Args...)> func;
            explicit Subscription(std::function<void(Args...)> f) : func(std::move(f)) {}
        };
    
        std::unordered_map<std::string, std::vector<std::shared_ptr<void>>> subscriptions;
};

#endif
