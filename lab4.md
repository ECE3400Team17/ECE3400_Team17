[To Home Page](./index.md)

# Lab 4: FPGA Video Controller and Sound Generation

## Over all Objective: 
Lab 3 introduces the FPGA to our current setup.  The six of us were split into two teams.  One was incharge of setting up the VGA 

## Radio Team:
Yijia Chen, Thinesiya Krishnthasan, Stephanie Lin 

### Objective:
The goal of this lab is to implement the wireless communication between two Arduinos so that they may send and receive the robot’s mapping information. 

### Materials Used:
* 2 Nordic nRF24L01+ transceivers
* 2 Arduino Unos (one must be shared with the other sub-team)
* 2 USB A/B cables
* 2 radio breakout boards with headers

### Prelab:
First, we downloaded the RF34 Arduino library and added it to our Arduino library directory. After downloading the Getting Started code from the Lab 4 webpage and replacing the example in the RF 24 library, we modified it to test out our radios and become familiar with the library. We changed the identifier numbers by using the formula 2(3D+N)+X, where D = 4 (Friday lab), N = 17 (team number), and X = 0, 1 for our two radios. We changed the channel numbers to 3A and 3B, implemented in the following code:

```
const uint64_t pipes[2] = { 0x000000002ALL, 0x000000002BLL };
```

We then programmed both Arduino boards with the Getting Started example, plugged them into two different laptops, and selected one board to be the transmitter by entering “T” into the serial monitor. Below is a screenshot of what we saw on each monitor. The left side is from the transmitter and the right side is the response. We received regular outputs for successfully sent and received signals. Then, we examined the effect of moving the radios far from one another. We then noticed that the sending failed (see that the screen on the right does not show payload 67313 at all) and that the failures that didn’t time out would return with an exceptionally long round-trip delay.




[To Home Page](./index.md)
