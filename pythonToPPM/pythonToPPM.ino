// @Harris Christiansen (HarrisChristiansen.com)
// 2014-10-03
// Quadcopter Control At HackMizzou


//////////////////////CONFIGURATION///////////////////////////////
#define chanel_number 8  //set the number of chanels
#define default_servo_value 1500  //set the default servo value
#define PPM_FrLen 22000  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 400  //set the pulse length
#define onState 0  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////

int ppm[chanel_number];
int pitch = 0;
int roll = 0;
int yaw = 0;
int y = 0;

void setup(){  
  for(int i=0; i<chanel_number; i++) {
    ppm[i]= default_servo_value;
  }
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);
  
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  
  OCR1A = 100;
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
  
  Serial.begin(115200);
  Serial.println("Started");
  Serial.setTimeout(1); // Min-Delay Between Numbers
}








// Receive Channels and Set
bool justSet = false;
void loop() {
  while (Serial.available() > 0) {
    setValues(Serial.parseInt());
    //setValues(Serial.read());
    justSet = true;
  }
  if(justSet) {
    ppm[0] = (y*10)+1000;
    ppm[1] = (yaw*5)+1500;
    ppm[2] = (pitch*5)+1500;
    ppm[3] = (roll*5)+1500;
    Serial.print(ppm[0]); Serial.print("  ");
    Serial.print(ppm[1]); Serial.print("  ");
    Serial.print(ppm[2]); Serial.print("  ");
    Serial.print(ppm[3]); Serial.println("  ");
    justSet = false;
  }
  delay(40);
  
  /*
  if (Serial.available() > 0) { // Input of form 1500 1500 1500 1500
    ppm[0] = Serial.parseInt();
    ppm[1] = Serial.parseInt();
    ppm[2] = Serial.parseInt();
    ppm[3] = Serial.parseInt();
    Serial.print(ppm[0]); Serial.print("  ");
    Serial.print(ppm[1]); Serial.print("  ");
    Serial.print(ppm[2]); Serial.print("  ");
    Serial.print(ppm[3]); Serial.println("  ");
  }
  */
  
  //ppm[0] = 1500;
}










//////////// PPM Output ////////////

ISR(TIMER1_COMPA_vect){
  // PPM Generator From http://www.rcgroups.com/forums/showthread.php?t=1808432
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= chanel_number){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PulseLen;
      OCR1A = (PPM_FrLen - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}









////////////// Serial Input //////////////

void setValues(int x) {
  if (x >= 0 && x <= 200) { // Forward/Backward
    pitch = x-100;
  } else if (x >= 300 && x <= 500) { // Left/Right
    yaw = x-400;
  } else if (x >= 600 && x <= 800) { // Rotate
    roll = x-700;
  } else if (x >= 900 && x <= 1000) { // Throttle
    y = x-900;
  }
}

