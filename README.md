# Self Driving Car TopGun
Self driving car with 5 CAN bus nodes.
This project contains 5 different controller codes for different functionality.
1) Master controller
2) Sensor Controller
3) Motor/IO Controller
4) Geo Controller
5) Bridge/Bluetooth Controller

Each controller is communicating with each other over CAN bus. To get specific messages on specific controller CAN filter is implemented.

The GPS-controlled automated RC car will consistes of 5 different LPC 1758 controllers. Each controller will have a specific major tasks required to drive the car. The naming convention goes as:-
Motor & I/O controller - this will control the motors of the car and will also connected with a LCD display to show the car's status, Sensor controller - It will be connected to the obstacle detecting sensors on the car, Communication Bridge - It will be connected to an Android mobile phone so as to provide co-ordinates, GEO controller - This will give the exact orientation of the car e.g., heading & bearing, etc. and finally the Master controller - This will collect the data from other controllers and will guide the motor controller. These controllers are connected using CAN bus. After the final implementation, this car will be capable of driving by itself using the destination co-ordinates set by us avoiding every obstacles, overcoming slopes thereby reaching the destination safely!

![Alt text](https://cloud.githubusercontent.com/assets/8917396/11769973/85bf390a-a1a8-11e5-9867-e8eee2157e49.jpg "Project Block Diagram")
