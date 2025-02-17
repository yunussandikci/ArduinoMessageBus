#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <Arduino.h>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include <tuple>

class MessageBus {
    public:
        template<typename... T, typename Callable> void subscribe(const char* topic, Callable&& callback) {
            subscriptions[std::string(topic)].push_back(
                std::make_shared<Subscription<T...>>(
                    std::function<void(T...)>(std::forward<Callable>(callback))
                )
            );
        }
    
        template<typename... T> unsigned long publishMessage(const char* topic, T... args) {
            return _publishMessage(topic, false, args...);
        }

        template<typename... T> unsigned long publishAndStoreMessage(const char* topic, T... args) {
            return _publishMessage(topic, true, args...);
        }
        
        template<typename... T> std::shared_ptr<std::tuple<T...>> getLatestMessage(const char* topic) {
            auto it = lastMessages.find(std::string(topic));
            if (it == lastMessages.end()) return nullptr;
        
            return std::static_pointer_cast<std::tuple<T...>>(it->second);
        }
    
    private:
        template<typename... T> struct Subscription {
            std::function<void(T...)> func;
            explicit Subscription(std::function<void(T...)> f) : func(std::move(f)) {}
        };

        template<typename... T> unsigned long _publishMessage(const char* topic, bool save, T... args) {
            unsigned long times = 0;

            auto message = std::make_shared<std::tuple<T...>>(args...);

            if (save) {
                lastMessages[std::string(topic)] = message;
            }

            auto it = subscriptions.find(std::string(topic));
            if (it == subscriptions.end()) return 0;

            for (const auto& callback : it->second) {
                static_cast<Subscription<T...>*>(callback.get())->func(args...);
                times++;
            }

            return times;
        }
    
        std::unordered_map<std::string, std::vector<std::shared_ptr<void>>> subscriptions;
        std::unordered_map<std::string, std::shared_ptr<void>> lastMessages;
};

#endif
