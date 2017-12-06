[To Home Page](../index.md)

# Final Report

## Introduction 

The goal of this project was to build an intelligent physical system that could traverse a maze independently by perceiving and acting upon its environment. Our final design intended to:

* Begin on a 660 Hz tone
* Follow a grid of black lines through the maze a grid consisting of black lines
* Detect walls and IR frequency beacons (treasures)
* Display and map the maze as it progresses 
* Send a “done” signal to indicate that the robot has finished traversing the maze

Our robot, unfortunately, did not meet all the requirements of the final competition, but he sure tried really hard. He also looks the part. We are proud of it, our robo son. While he stubbornly refused to use SPI to map the maze, he was adequate in wall detection, treasure detection, and maze traversal. 

## Competition

Here is a video of our pride and joy during the competition, featuring Thinesiya’s hair at some point:

<iframe width="560" height="315" src="https://www.youtube.com/embed/aayaG6dnSNM" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

Our boy never stops working. Literally. He doesn’t stop. But nonetheless, it was a wholesome learning experience in which we learned about hardware, software, & mental wear and tear due to our superb, marvelous communication skills. We like to think we grew as a team and as individuals thanks to this semester-long team project.

### Design

The objective of the challenge was to build a system that was capable of solving and mapping a maze once promoted by a 660 Hz signal. While the robot solves the maze, it is to report using radio to radio communication the current position of the robot, its previous occurrences, and the location and specific frequency of the treasures. To indicate completion, the robot is to play a sound. 

In order to succeed these objectives, our group chose to maximize stability and reduce drag in order to develop a robot with high accuracy and speed. 

The Bill of Materials utilized includes: 
* Arduino Mega
* 2 continuous rotation servo motors
* 2 Wheels
* 4 IR treasure sensors
* 4 wall detection sensors
* 4 line detection sensors
* 1 microphone
* 1 push button
* 1 radio
* 1 5V External Battery
* 1 9V External Battery

*Note: In our prelimary design, we were utilizing the Arduino Uno, which do not provide enough analog pins needed to integrate all sensors. Instead of utilizing a MUX to select between various signals, we decided to switch our microcontroller to the Arduino Mega which provided significantly more analog pins.* 

In order to avoid a messy electronics cage, we minimized amounts of wires needed by designing and milling a shield for the Arduino Mega. This eliminated the need for a breakout board. 

![Shield](./img/shield.png)

We placed the line sensors underneath the body of the robot. This decision to w
make them as close to the ground as possible was due to correlation between closer distances and accuracy. The code to prompt the robot to follow the black lines detected can be found later in this tutorial. 

We then mounted the wall detectors on each axis of the robot. In competition, we found that because one of these sensors was tilted, our robot was unable to detect a wall and therefore incorrectly continued to turn into a wall. The algorithm and code for this is also provided later in this tutorial. It is important to know, that using these sensors, we figured out which threshold was needed to achieve wall detection. Through designing the system, we were sure to clear the path needed for the IR sensors

Additionally, we included the 4 IR treasure sensors. These sensors also showed to have higher accuracy when placed very close to the the treasures. Therefore, we created 4 arms on each side of the robot to extend these circuits to be as close to the treasures without interference.

In order to communicate the sensor detection, we mounted a radio on the top of the robot in order to communicate with the base station. 

A few of the technical hardware solutions we needed to account for was the uneven distribution of weight. Because it was not an option to build a wider base due to space constraints, we built our system vertically. From this, we noticed that the robot had a tendency to tip forward and backward. We added ballpoint stabilizers in order to increase stability. As seen in competition videos, our robot not only made very controlled movements indicating stability.


### Robot
The two features that we added were: 

* [Start Tone](./StartTone.md)
* [Treasure Detection](./TreasureDetection.md)

#### Improvements to Navigation Algorithm
We improved upon the algorithm by adding the capability for the robot to keep track of the position it has visited.  Which we were able to do by keeping track of the robot’s orientation.  We also added four wall sensors to the robot so that it can transmit its environment from all directions back to the base station. We used the crosssections as indicators where the robot would decide on where to go as well as the transmit it packets. Here is a vedio of the test run where the robot stops when it has traversed the maze.:

<iframe width="560" height="315" src="https://www.youtube.com/embed/BwsDFrqkYBE" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

Here is the link to the full length [code](./NavigationAlgo.md)

### Communication and Base Station

* [Radio Communication](./RadioCommunication.md)
* [SPI](./Spi.md)
* [Map Drawing](./DrawingtheMaze.md)

### Final Product
We all learned a lot about integrating all parts of ECE to great this final product and we greatly thank all the wonerful TA's that helped is every step the way.  Thank you Professor Petersen for also encouraging us to work as a team to accomplish our goals.  Although we didn't prefrom as well during competition and there are many improvements we can make to optimize our robot, we are very proud the way our little guy turned out. Thank You for an Awesome Semester!

![Robot](./img/robot.png)

### Work Distribution For the Report/Final Product
We all helped each other, especially when integrating the components. But we split the work as follows amongst ourselves based on people coming to lab for component wise testing:
* Introduction: Stephanie
* Design: Mira
* Improvements to Navigation Algorithm & Start Tone: Thinesiya 
* Treasure Detection: Peter
* Radio Communication: Stephanie
* SPI: Alan & Yijia
* Map Drawing: Peter & Alan & Yijia
* Formating Website: Thinesiya  



[To Home Page](../index.md)
