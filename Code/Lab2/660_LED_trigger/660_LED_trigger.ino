#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft
#include <FFT.h> // include the library

int binNum = 20;
int soundThres = 60;

int D7 = 7;

void setup() {
  Serial.begin(115200); // use the serial port
  pinMode(D7, OUTPUT);
}


void loop() {
  while(1) {
    cli();
    for (int i = 0 ; i < 512 ; i += 2) {
      fft_input[i] = analogRead(A0); // <-- NOTE THIS LINE
      fft_input[i+1] = 0;
    }
    fft_window();
    fft_reorder();
    fft_run();
    fft_mag_log();
    sei();
//    Serial.println("start");



    for (byte i = 0 ; i < FFT_N/2 ; i++) {
//      Serial.println(fft_log_out[i]);
      if (fft_log_out[binNum]> soundThres){
        Serial.println("Yay");
        digitalWrite(D7, HIGH); 
      }
      else{
        digitalWrite(D7, LOW);
      }
    }
  }
}
