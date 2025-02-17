# ArduinoMessageBus

ArduinoMessageBus is a simple, type-safe, and lightweight publish-subscribe (pub/sub) library for handling function invocations based on topic subscriptions in Arduino projects. It enables decoupled communication between different parts of your application without direct dependencies.

## Features
- **Type-Safe Subscriptions** – Supports parameterized callbacks with strict type checking.
- **Multiple Subscribers per Topic** – Allows multiple functions to receive messages on the same topic.
- **Efficient & Lightweight** – Uses `std::unordered_map` and `std::shared_ptr` for memory efficiency.
- **Minimalistic API** – Simple `subscribe` and `publish` functions for easy integration.
- **Asynchronous Event Handling** – Enables event-driven programming in embedded systems.
- **Message Storage** – Optionally store the latest message for a topic for later retrieval.

## Installation
### Arduino IDE
1. Download the latest release from GitHub.
2. Extract and place the `MessageBus` folder in your Arduino `libraries/` directory.
3. Restart the Arduino IDE.

### PlatformIO
Add the following to your `platformio.ini` file:
```ini
lib_deps =
    yunussandikci/MessageBus
```

## Getting Started
### Basic Example
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
    bus.publishMessage("test", 42);
}

void loop() {
}
```

### Multiple Subscribers
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

    bus.publishMessage("event", 99);
}

void loop() {
}
```

**Expected Output:**
```
Callback1 received: 99
Callback2 received: 99
```

### Lambda Callbacks
You can use lambda functions to keep code more concise:

```cpp
#include <MessageBus.h>

MessageBus bus;

void setup() {
    Serial.begin(115200);
    
    bus.subscribe<int>("number", [](int num) {
        Serial.printf("Received number: %d\n", num);
    });

    bus.publishMessage("number", 7);
}

void loop() {
}
```

**Expected Output:**
```
Received number: 7
```

### Publishing Multiple Parameters
MessageBus allows publishing multiple parameters to a topic:

```cpp
#include <MessageBus.h>

MessageBus bus;

void sensorHandler(int id, float value) {
    Serial.printf("Sensor %d reports %.2f\n", id, value);
}

void setup() {
    Serial.begin(115200);
    
    bus.subscribe<int, float>("sensor", sensorHandler);
    bus.publishMessage("sensor", 1, 23.5f);
}

void loop() {
}
```

**Expected Output:**
```
Sensor 1 reports 23.50
```

## API Reference
### `subscribe<Args...>(const char* topic, Callable&& callback)`
Subscribes a function to a topic. When a message is published on this topic, the function will be called with the provided arguments.

#### Parameters:
- `topic` – The topic name (`const char*`).
- `callback` – A function that takes the same parameter types as those published to the topic.

#### Example Usage:
```cpp
bus.subscribe<int, float>("sensor", [](int id, float value) {
    Serial.printf("Sensor %d reports %.2f\n", id, value);
});
```

### `publishMessage<Args...>(const char* topic, Args... args)`
Publishes a message to all subscribers of the given topic. Each subscriber's function will be executed with the provided arguments.

#### Parameters:
- `topic` – The topic name (`const char*`).
- `args...` – The values to pass to the subscriber callbacks.

#### Example Usage:
```cpp
bus.publishMessage("sensor", 1, 23.5f);
```

### `publishAndStoreMessage<Args...>(const char* topic, Args... args)`
Publishes a message to all subscribers and stores the latest message for the topic.

#### Parameters:
- `topic` – The topic name (`const char*`).
- `args...` – The values to pass to the subscriber callbacks.

#### Example Usage:
```cpp
bus.publishAndStoreMessage("sensor", 1, 23.5f);
```

### `getLatestMessage<Args...>(const char* topic)`
Retrieves the latest message stored for a topic.

#### Parameters:
- `topic` – The topic name (`const char*`).

#### Example Usage:
```cpp
auto message = bus.getLatestMessage<int, float>("sensor");
if (message) {
    int id = std::get<0>(*message);
    float value = std::get<1>(*message);
    Serial.printf("Latest sensor data: %d, %.2f\n", id, value);
}
```

## Performance Considerations
- The library uses `std::unordered_map` for fast topic lookup.
- Callback functions are stored as `std::shared_ptr`, ensuring memory safety.
- Suitable for real-time applications where message processing time is critical.

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing
Contributions are welcome! Feel free to submit pull requests or report issues on GitHub.
