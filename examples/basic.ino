#include <MessageBus.h>

MessageBus bus;

void callback(int sensor_value) {
    // Print the received sensor value
    Serial.printf("Received: %d\n", sensor_value);
}

void setup() {
    Serial.begin(115200);

    // Subscribe to the "sensor_value" topic
    bus.subscribe<int>("sensor_value", callback);
}

void loop() {
    // Publish a random value to the "sensor_value" topic
    bus.publish("sensor_value", random(0, 100));

    // Wait 3 seconds
    delay(3000);
}