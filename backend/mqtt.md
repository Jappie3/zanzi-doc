# MQTT

## Usage

MQTT was used during this project to send the data from the device to the database. Since the code that handles sending that data is written in JavaScript, the mqtt npm package was used (<https://www.npmjs.com/package/mqtt>). The code itself is relatively simple, it just listens to a specified channel and sends the data to InfluxDB (using [this](./influxdb.md) code).

```js

const mqtt = require("mqtt");

// MQTT options
options = {
  clientId: "mqttjs01", //this must be unique
  username: process.env.MOSQUITTO_USER,
  password: process.env.MOSQUITTO_PASSWD,
  clean: true,
};

const client = mqtt.connect(process.env.MOSQUITTO_URL_EXAMPLE, options);

// Connect to MQTT
// Logging result to the console to verify a connection or error
client.on("connect", function () {
  console.log("connected");
});

client.on("error", function (error) {
  console.log("Can't connect" + error);
});

// Subscribing to the correct toppic
const topic_s = "exampleTopic";
client.subscribe(topic_s, { qos: 0 });

client.on("message", function (topic, message, packet) {
  //Logging message and topic to console for debugging
  console.log("message is " + message);
  console.log("topic is " + topic);

  // Here you can format the message and handle it accordingly
});
```

This code connects to an MQTT server, subscribes to a specified channel and then logs every message it receives to the console.
There are a couple of things that should be kept in mind:

    - The topic can contain wildcards, symbols that represent an unknown amount of characters. For example: if the topic is "testTopic*", messages from both "testTopic1" and "testTopic1234567890" will be logged to the console.
    - The clientId in the options MUST be unique (within the scope of the server). If two applications are listening to the same server and with the same clientId then the two applications will constantly disconnect and reconnect (this causes a loop). All options can be the same EXCEPT the clientId.