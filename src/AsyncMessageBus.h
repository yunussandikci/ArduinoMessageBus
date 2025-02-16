#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <Arduino.h>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <tuple>

class AsyncMessageBus {
private:
    template<int...> struct IndexSeq {};
    template<int N, int... S> struct MakeIndexSeq : MakeIndexSeq<N-1, N-1, S...> {};
    template<int... S> struct MakeIndexSeq<0, S...> { typedef IndexSeq<S...> type; };
    template<typename F, typename Tuple, int... Is>
    static void call_with_tuple(F&& f, Tuple&& t, IndexSeq<Is...>) {
        std::forward<F>(f)(std::get<Is>(std::forward<Tuple>(t))...);
    }

public:
    template<typename... T, typename Callable>
    void subscribe(const char* topic, Callable&& callback) {
        std::string t(topic);
        subscriptions[t].push_back(
            std::static_pointer_cast<void>(
                std::make_shared<Subscription<T...>>(
                    std::function<void(T...)>(std::forward<Callable>(callback))
            )
        ));
    }

    template<typename... T>
    void publish(const char* topic, T&&... args) {
        auto message = std::make_shared<Message<T...>>(
            std::forward<T>(args)...,
            std::string(topic)
        );
        
        queue.emplace([this, message]() {
            auto it = subscriptions.find(message->topic);
            if (it == subscriptions.end()) return;
            
            for (auto& sub : it->second) {
                auto* s = static_cast<Subscription<T...>*>(sub.get());
                using Seq = typename MakeIndexSeq<sizeof...(T)>::type;
                
                call_with_tuple(s->func, message->args, Seq());
            }
        });
    }

    void loop() {
        while (!queue.empty()) {
            queue.front()();
            queue.pop();
        }
    }

    size_t getQueueSize() const {
        return queue.size();
    }

private:
    template<typename... T>
    struct Subscription {
        std::function<void(T...)> func;
        explicit Subscription(std::function<void(T...)> f) : func(std::move(f)) {}
    };

    template<typename... T>
    struct Message {
        std::tuple<T...> args;
        std::string topic;
        Message(T&&... a, std::string t) : 
            args(std::forward<T>(a)...), topic(std::move(t)) {}
    };

    std::queue<std::function<void()>> queue;
    std::unordered_map<std::string, std::vector<std::shared_ptr<void>>> subscriptions;
};

#endif
