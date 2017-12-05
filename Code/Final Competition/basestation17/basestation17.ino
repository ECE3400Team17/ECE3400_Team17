/*
  Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

/**
   Example for Getting Started with nRF24L01+ radios.

   This is an example of how to use the RF24 class.  Write this sketch to two
   different nodes.  Put one of the nodes into 'transmit' mode by connecting
   with the serial monitor and sending a 'T'.  The ping node sends the current
   time to the pong node, which responds by sending the value back.  The ping
   node can then see how long the whole cycle took.
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9, 10);

int counter = 0;


//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000003ALL, 0x000000003BLL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r", role_friendly_name[role]);
  printf("* PRESS 'T' to begin transmitting to the other node\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //

  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();


    // Define data.
    unsigned char packet1;
    unsigned char packet2;
    // pack bits as follows:

    // packet 1
    // indicator = 0 | direction (N/S/E/W) | long side of maze (y-coord)
    // 1 bit | 2 bits | 5 bits

    // packet 2
    // indicator = 1| presence of wall (L/F/W) | short side of maze (x-coord)
    // 1 bit | 3 bits | 4 bits

    // assigning chars??

    // example packet 1: ind = 0,
    unsigned char ind_1 = 0;
    unsigned char dir = 3;
    unsigned char pos_y = 2;

    unsigned char ind_2 = 1;
    unsigned char wall = 5;
    unsigned char pos_x = 1;

    // shift bits in order to pack bits, then or them together
    packet1 = ind_1 << 7 | dir << 5 | pos_y;
    packet2 = ind_2 << 7 | wall << 4 | pos_x;

    // Send maze in single payload
    printf("Now sending packet 1!\n");
    bool ok1 = radio.write(&packet1, sizeof(unsigned char) );


    if (ok1)
      printf("ok\n");
    else
      printf("failed.\n\r");

    // delay(1000); //wait before sending packet 2 (we can shorten this)

    printf("Now sending packet 2!\n");
    bool ok2 = radio.write(&packet2, sizeof(unsigned char) );

    if (ok2)
      printf("ok\n");
    else
      printf("failed.\n\r");

    delay(1000);

    // Now, continue listening
    radio.startListening();
  }


  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  // ------------------------------- BASE STATION: RECEIVING & UPDATING DATA  ------------------------------- //

  // pack bits as follows:
  // packet 20
  // indicator(DONE) = 0 | Orientation (N/S/E/W) | Position (0-19, start is always at 19)
  // 1 bit | 2 bits | 5 bits
  // packet 19-0
  // treasure(7Hz,12Hz,17Hz)| color | wall(W/E/S/N)
  // 2 bit | 2 bits | 4 bits

  // Define data.
  unsigned char ind_done = 0;
  unsigned char rob_orien = 0;
  unsigned char rob_pos = 19;
  unsigned char treas;
  unsigned char tile_clr;
  unsigned char wall;

  //array of packets
  unsigned char packet20;
  unsigned char packet19;
  unsigned char packet18;
  unsigned char packet17;
  unsigned char packet16;
  unsigned char packet15;
  unsigned char packet14;
  unsigned char packet13;
  unsigned char packet12;
  unsigned char packet11;
  unsigned char packet10;
  unsigned char packet9;
  unsigned char packet8;
  unsigned char packet7;
  unsigned char packet6;
  unsigned char packet5;
  unsigned char packet4;
  unsigned char packet3;
  unsigned char packet2;
  unsigned char packet1;
  unsigned char packet0;

  unsigned char mapPacks[21] = {packet0, packet1, packet2, packet3, packet4, packet5, packet6, packet7, packet8, packet9, packet10,
                                packet11, packet12, packet13, packet14, packet15, packet16, packet17, packet18, packet19, packet20
                               };

  //unsigned char packIndex;

  ///// Radio Receiving //////

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      unsigned char got_data;
      radio.read(&got_data, sizeof(unsigned char) );

      //packet type 1: done/orientation/position
      if (counter % 2 == 0) {
        ind_done = (got_data & 128) >> 7;
        rob_orien = (got_data & 96) >> 5;
        rob_pos = got_data & 31;
        mapPacks[20] = got_data;
        printf("DONE = %d\nORIENTATION = %d\nPOSITION = %d\n\n", ind_done, rob_orien, rob_pos);   // print info
        //print done signal
        if (ind_done == 1) {
          printf("\nDONE!!!!!!! Congratulations!n\n");
        }
      }

      //packet type 2: treasure/color/walls
      else {
        mapPacks[rob_pos] = got_data;
        treas = (got_data & 192) >> 6;
        tile_clr = (got_data & 48) >> 4;
        wall = got_data & 15;
        // print info
        printf("TREASURE = %d\nTILE COLOR = %d\n", treas, tile_clr);
        Serial.print(wall, BIN);
        Serial.print("\n\n");
      }
      counter = counter + 1;



      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);

    }
  }


  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("* CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1, pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("* CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1, pipes[0]);
    }
  }
}
