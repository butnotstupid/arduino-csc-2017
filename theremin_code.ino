#include <TimerOne.h>

// boundaries
const int min_pitch = 55;
const int max_pitch = 900;
const int min_delay = 50;
const int max_delay = 200;

int min_in = 1023;
int max_in = 0;


// pins
const byte speaker = 12;
const byte interruptPin = 2;

// note sets
const int tones[89] = {31,33,35,37,39,41,44,46,49,52,55,58,62,65,69,73,78,82,87,93,98,104,110,117,123,131,139,147,156,165,175,185,196,208,220,233,247,262,277,294,311,330,349,370,392,415,440,466,494,523,554,587,622,659,698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976,2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,4435,4699,4978};
const int tones4[13] = {31,262,277,294,311,330,349,370,392,415,440,466,494};
const int tonesNS[54] = {31,33,37,41,44,49,55,62,65,73,82,87,98,110,123,131,147,165,175,196,220,247,262,294,330,349,392,440,494,523,587,659,698,784,880,988,1047,1175,1319,1397,1568,1760,1976,2093,2349,2637,2794,3136,3520,3951,4186,4699,0};

// 0 -- tonal mode, 1 -- pitch mode
volatile byte mode = 0;

void setup() {
    // put your setup code here, to run once:

    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), change_mode, FALLING);

    Serial.begin(9600);
    Serial.println("Initialized");

//    Timer1.initialize(1000000);
//    Timer1.attachInterrupt(trace_potentiometr);

    say_hello();
}

bool calibrated = false;

void loop() {
    // put your main code here, to run repeatedly:

    if (millis() < 5000) {
      collect_range();
    }

    if (millis() > 5000 && !calibrated) {
        say_hello();
        show_range();
        calibrated = true;
    }

    if (calibrated) {
        int ldr_in = analogRead(0);
        
        int pot_in = analogRead(1);
        int wait;
        if (pot_in == 0) {
            wait = 0;
        }
        else {
            wait = map(pot_in, 0, 1023, min_delay, max_delay);  
        }

        if (mode == 0) {
//            Serial.println("..............in tonal mode");
            int note = floor(map(ldr_in, min_in, max_in, 0, 53));
            tone(speaker, tonesNS[note], 50);  
            
        }
        else {
//            Serial.println("..............in pitch mode");
            int pitch = map(ldr_in, min_in, max_in, min_pitch, max_pitch);
            if (pitch > 800) pitch = 0;
            tone(speaker, pitch, 50);    
        }
        
        delay(wait);

        if (wait > 0) {
          tone(speaker, 55, 25);
          delay(25);

          tone(speaker, 55, 25);
          delay(25);
            
        }
    }
}

void change_mode() {
    mode = !mode;
    Serial.print("mode: ");
    Serial.println((mode == 0) ? "tonal" : "pitch");
}

void say_hello() {
    int sum = 0;
    for (int pitch = 150; pitch < 200; pitch++){
       tone(speaker, pitch, 5);  
       delay(5);
    }
    delay(2);
    for (int pitch = 100; pitch < 200; pitch++){
       tone(speaker, pitch, 5);  
       delay(5);
    }
} 

void collect_range() {
    int in = analogRead(0);
    trace_input();
    if (in > max_in) {
        max_in = in;
    }
    if (in < min_in) {
        min_in = in;
    }
}

void trace_input() {
  Serial.print("LDR: ");
  Serial.println(analogRead(0));  
}

void trace_potentiometr() {
  Serial.print("Potentiometr: ");
  Serial.println(analogRead(1));  
}

void show_range() {
    Serial.print("Range: ");
    Serial.print(min_in);
    Serial.print(" -- ");
    Serial.println(max_in);
}
