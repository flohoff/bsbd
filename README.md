
BSBd - Boiler Bus Daemon
========================

This project is basically trying to learn about the Boiler System Bus (BSB). It is inspired by BSB-LAN
which is a esp32/arduino projects for servicing the BSB.

As the number of messages, values and value types is a large n dimensional issue i think it has outgrown
the small embedded microntroller world.

The goal of this project is to listen on the BSB, dump messages to syslog and MQTT for graphing
via telegraf, influx and grafana.

Controlling the Heating is not currently in scope.

Config
======

Not much here. There is *config.json* which defines the Busses and some other details. Then there is *messageformat.json*
which defines possible messages and their value decoding.

Building
========

You need at least boost and nlohmann json to build this. 

	apt-get install build-essential libboost-all-dev cmake git
	
	git clone https://github.com/nlohmann/json.git nlohmann-json
	( cd nlohmann-json ; cmake . ; make ; make install )
	
	cmake .
	make
	
	bsbd --config config.json

