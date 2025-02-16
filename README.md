
# MessageBus

MessageBus is a simple, type-safe, and lightweight publish-subscribe (pub/sub) library for handling function invocations based on topic subscriptions in Arduino projects. It enables decoupled communication between different parts of your application without direct dependencies.

## Features
- **Type-Safe Subscriptions** – Supports parameterized callbacks with strict type checking.
- **Multiple Subscribers per Topic** – Allow multiple functions to receive messages on the same topic.
- **Efficient & Lightweight** – Uses `std::unordered_map` and `std::shared_ptr` for memory efficiency.
- **Minimalistic API** – Simple `subscribe` and `publish` functions for easy integration.
- **Asynchronous Event Handling** – Enables event-driven programming in embedded systems.

## Installation
### Arduino IDE
1. Download the latest release from [GitHub](https://github.com/yourname/MessageBus).
2. Extract and place the `MessageBus` folder in your Arduino `libraries/` directory.
3. Restart the Arduino IDE.

### PlatformIO
```sh
pio lib install "MessageBus"
```

## Getting Started
### **Basic Example**
This example demonstrates how to subscribe to a topic and publish messages.

```cpp
#include <MessageBus.h>

MessageBus bus;

void callback(int value) {
    Serial.print("Received: ");
    Serial.println(value);
}

void setup() {
    Serial.begin(115200);
    
    // Subscribe to the "test" topic with an integer parameter
    bus.subscribe<int>("test", callback);

    // Publish an integer message to "test"
    bus.publish("test", 42);
}

void loop() {
}
```

### **Multiple Subscribers**
You can attach multiple subscribers to the same topic:

```cpp
#include <MessageBus.h>

MessageBus bus;

void callback1(int value) {
    Serial.print("Callback1 received: ");
    Serial.println(value);
}

void callback2(int value) {
    Serial.print("Callback2 received: ");
    Serial.println(value);
}

void setup() {
    Serial.begin(115200);
    
    bus.subscribe<int>("event", callback1);
    bus.subscribe<int>("event", callback2);

    bus.publish("event", 99);
}

void loop() {
}
```
**Expected Output:**
```
Callback1 received: 99
Callback2 received: 99
```

### **Lambda Callbacks**
You can use lambda functions to keep code more concise:

```cpp
#include <MessageBus.h>

MessageBus bus;

void setup() {
    Serial.begin(115200);
    
    bus.subscribe<int>("number", [](int num) {
        Serial.printf("Received number: %d
", num);
    });

    bus.publish("number", 7);
}

void loop() {
}
```
**Expected Output:**
```
Received number: 7
```

### **Publishing Multiple Parameters**
MessageBus allows publishing multiple parameters to a topic:

```cpp
#include <MessageBus.h>

MessageBus bus;

void sensorHandler(int id, float value) {
    Serial.printf("Sensor %d reports %.2f
", id, value);
}

void setup() {
    Serial.begin(115200);
    
    bus.subscribe<int, float>("sensor", sensorHandler);
    bus.publish("sensor", 1, 23.5f);
}

void loop() {
}
```
**Expected Output:**
```
Sensor 1 reports 23.50
```

## API Reference
### **`subscribe<Args...>(const char* topic, Callable&& callback)`**
Subscribes a function to a topic. When a message is published on this topic, the function will be called with the provided arguments.

#### Parameters:
- `topic` – The topic name (const char*).
- `callback` – A function that takes the same parameter types as those published to the topic.

#### Example Usage:
```cpp
bus.subscribe<int, float>("sensor", [](int id, float value) {
    Serial.printf("Sensor %d reports %.2f
", id, value);
});
```

### **`publish<Args...>(const char* topic, Args... args)`**
Publishes a message to all subscribers of the given topic. Each subscriber's function will be executed with the provided arguments.

#### Parameters:
- `topic` – The topic name (const char*).
- `args...` – The values to pass to the subscriber callbacks.

#### Example Usage:
```cpp
bus.publish("sensor", 1, 23.5f);
```

### **Performance Considerations**
- The library uses `std::unordered_map` for fast topic lookup.
- Callback functions are stored as `std::shared_ptr`, ensuring memory safety.
- Suitable for real-time applications where message processing time is critical.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributing
Contributions are welcome! Feel free to submit pull requests or report issues on [GitHub](https://github.com/yourname/MessageBus).
