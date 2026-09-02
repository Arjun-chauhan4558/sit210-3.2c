Task 3.2C – MQTT (SIT210)

Arduino Nano 33 IoT project using MQTT publish/subscribe to control lighting via gesture detection.

What it does
Ultrasonic sensor detects a wave or a pat.
Publishes the event (with name) to ES/Wave or ES/Pat on the public EMQX broker (broker.emqx.io:1883).
Subscribes to both topics — a wave switches both LEDs ON, a pat switches both LEDs OFF.
Hardware
Arduino Nano 33 IoT
HC-SR04 Ultrasonic Sensor
2 × LEDs + resistors
Breadboard and jumper wires
Broker
broker.emqx.io, port 1883
Author

Arjun
