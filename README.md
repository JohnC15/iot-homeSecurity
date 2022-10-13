# iot-homeSecurity
* IOT home security project based on Raspberry PI 4 and an old Linux laptop.
  - Raspberry PI - using C++ Paho MQTT and OpenCV libraries and external USB camera. 
  - Linux laptop - using Golang Paho MQTT and OpenCV libraries, and EMQX Broker docker container


## mqttPub
* C++ mqtt client that publishes captured video frame using OpenCV, encoding as JPEG before sending them to a MQTT Broker

## mqttSub 
* golang mqtt client that subscribes for JPEG images topic with a MQTT Broker, decoding and saving them using OpenCV

## videoCapture
* OpenCV C++ client that capture video from external USB camera and saving them as MP4 movie and JPEG images.
