/******************************************************************************* 
 * Copyright (c) 2022 John Choo <john.choo.plano@gmail.com>
 * 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v2.0 which accompany this distribution.
 *
 * Subscribe, decode and save jpeg image using Golang OpenCV and Paho MQTT.
 *******************************************************************************/
package main

import (
	"fmt"
	MQTT "github.com/eclipse/paho.mqtt.golang"
	"gocv.io/x/gocv"
)

var messagePubHandler MQTT.MessageHandler = func(client MQTT.Client, msg MQTT.Message) {
    fmt.Printf("Received message: %d from topic: %s\n", len(msg.Payload()), msg.Topic())
    mat, err := gocv.IMDecode(msg.Payload(), gocv.IMReadUnchanged)
	if err != nil {
		fmt.Println("Error decoding image")
		return
	}
	defer mat.Close()
	gocv.IMWrite("remoteImg.jpg", mat)
}

var connectHandler MQTT.OnConnectHandler = func(client MQTT.Client) {
    fmt.Println("Connected")
}

var connectLostHandler MQTT.ConnectionLostHandler = func(client MQTT.Client, err error) {
    fmt.Printf("Connect lost: %v\n", err)
}

func main() {
    var broker = "localhost"
    var port = 1883
    opts := MQTT.NewClientOptions()
    opts.AddBroker(fmt.Sprintf("tcp://%s:%d", broker, port))
    opts.SetClientID("go_mqtt_client")
    opts.SetDefaultPublishHandler(messagePubHandler)
    opts.OnConnect = connectHandler
    opts.OnConnectionLost = connectLostHandler
    client := MQTT.NewClient(opts)
    if token := client.Connect(); token.Wait() && token.Error() != nil {
        panic(token.Error())
	}

	topic := "camera/kitchen/image"
	token := client.Subscribe(topic, 1, nil)
	token.Wait()
	fmt.Printf("Subscribed to topic %s\n", topic)

	fmt.Println("Press any key to exit")
	fmt.Scanln();

	if token := client.Unsubscribe(topic); token.Wait() && token.Error() != nil {
		panic(token.Error())
	}
	client.Disconnect(250);
}