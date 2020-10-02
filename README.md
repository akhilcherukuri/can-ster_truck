# CAN-STER TRUCK

<strong> [SocialLedge Wiki Page](http://socialledge.com/sjsu/index.php/S20:_Canster_Truck) <strong>

The Canster Truck Project is an autonomous RC car with CAN Bus interfaced controllers. The development of the RC car's subsystem modules (Interfacing of Ultrasonic Sensor, LIDAR, Bluetooth, GPS, Compass and CAN modules) was divided among six team members. The aim of the project is to develop an autonomous RC Car which can navigate from one source location to the selected destination on the app by avoiding obstacles in its path using sensors.

## Logic Setup.
<pre>
├───can_module
│   ├───can_bus_initializer
│   ├───can_constants
│   ├───can_driver_node
│   ├───can_geo_node
│   ├───can_handler
│   ├───can_motor_node
│   ├───can_sensor_node
│   └───test
├───hardware
│   ├───bt
│   ├───compass
│   ├───gps
│   ├───lcd
│   ├───lidar
│   ├───motor
│   ├───test
│   └───ultrasonic
├───logic
│   ├───driver_obstacle
│   ├───driver_state
│   ├───geo_logic
│   └───test
└───utility
    ├───line_buffer
    └───test
</pre>
