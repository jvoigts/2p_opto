//
//  LED pulse code for use in simultaneous 2p-imaging and visible light stimulation, for instance for optogenetics
//  see blog post on http://jvoigts.scripts.mit.edu/blog/?p=853 for details on the protocol.
//
//   Mar 2015, jvoigts@mit.edu

// set pin numbers:
const int ledPin =  4;      // LED pin
const int trigPin = 2; // look for trigger input here, not needed if output is used to gate an existing LED driver signal (with an analog max circuit or similar method)

const int controlPin = 12; // look for frame trigger input here

int armed = LOW;                    // ledState used to set the LED
unsigned long endMicros = 0;        // will store last time LED was updated
int prevtrig = LOW;

int line = 0;                      // keep track of what image scan line we're on
int skiplines = 8;


unsigned long wait = 0;           // target interval between end of scanlne and light pulse (actual nu,bers will be ~50-100us higher - measure with oscilloscope)
unsigned long duration = 100;    // approx pulse duration - won't be precise for all values but should be consistent across pulses

void setup() {

  pinMode(ledPin, OUTPUT);

}

void loop()
{

  unsigned long currentMicros = micros();

  int trig = digitalRead(trigPin);
  int control = digitalRead(controlPin);  // set to HIGH if no control is needed (LED is gated/modulated from somewhere else)

  if ((trig == LOW) && (prevtrig == HIGH)) { // on high->low transitions
    endMicros = currentMicros;
    armed = HIGH;
    line = line + 1;
  }

  if (armed == HIGH) {
    if (line % skiplines == 0 ) {
      if ((currentMicros - endMicros > wait) ) {
        if (control == HIGH) {
          digitalWrite(ledPin, HIGH);
          delayMicroseconds(duration);
          digitalWrite(ledPin, LOW);
        }
        armed = LOW;
      }
    }
  }
  prevtrig = trig;

}
