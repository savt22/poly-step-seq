#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include <MIDI.h>
#include <uClock.h>

//screen setup
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define I2C_ADDRESS 0x3C

#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

unsigned long screenref = 0;
unsigned long screentime = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

//mux setup
#define muxchannels 16
const int notetimemuxin(A7);
const int controlmuxin(A6);
static byte noteinput = 0;
static int notemuxvalues[muxchannels] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};
static byte controlinput = 0;
static int controlmuxvalues[muxchannels] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};

//step variables
int bpm = 120;
int clockstep;
int startstep;
int steptick;
int steptick1b;
int steptick1;
int steptick2;
int steptick3;
int steptick4;
int steptick5;
int steptick6;
int steptick7;
int steptick8;
int steptick8b;

int timearray[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

//control variables

int octave;
int octavenote = 0;
int octavearray[6] = {-24,-12,0,12,24,24};

int rootnote = 0;
int rootprevious = 0;
int rootcurrent = 0;

int scale = 0;

int modemap = 0;

int currentnote = 48;
int current3rd = 52;
int current5th = 55;
int note8 = 48;
int note83rd = 52;
int note85th = 55;
int note7 = 48;
int note73rd = 52;
int note75th = 55;
int note6 = 48;
int note63rd = 52;
int note65th = 55;
int note5 = 48;
int note53rd = 52;
int note55th = 55;
int note4 = 48;
int note43rd = 52;
int note45th = 55;
int note3 = 48;
int note33rd = 52;
int note35th = 55;
int note2 = 48;
int note23rd = 52;
int note25th = 55;
int note1 = 48;
int note13rd = 52;
int note15th = 55;
int note1map;
int note2map;
int note3map;
int note4map;
int note5map;
int note6map;
int note7map;
int note8map;

int notemap = 0;
int timemap = 5;
int time1 = 16;
int time2 = 16;
int time3 = 16;
int time4 = 16;
int time5 = 16;
int time6 = 16;
int time7 = 16;
int time8 = 16;
int t1;
int t2;
int t3; 
int t4;
int t5;
int t6;
int t7;
int t8;
int time1map;
int time2map;
int time3map;
int time4map;
int time5map;
int time6map;
int time7map;
int time8map;

const int *notearray = NULL;

static const int majorscale[21] = {48,50,52,53,55,57,59,60,62,64,65,67,69,71,72,74,76,77,79,81,83};
static const int lydianscale[21] = {48,50,52,54,55,57,59,60,62,64,66,67,69,71,72,74,76,78,79,81,83};
static const int mixolydianscale[21] = {48,50,52,53,55,57,58,60,62,64,65,67,69,70,72,74,76,77,79,81,82};
static const int minorscale[21] = {48,50,51,53,55,56,58,60,62,63,65,67,68,70,72,74,75,77,79,80,82};
static const int dorianscale[21] = {48,50,51,53,55,57,58,60,62,63,65,67,69,70,72,74,75,77,79,81,82};
static const int phrygianscale[21] = {48,49,51,53,55,56,58,60,61,63,65,67,68,70,72,73,75,77,79,80,82};
static const int chromaticscale[21] = {48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68};

char modes[3] [7] = {"mono", "poly", "poly"};
String mode = "mono";

int harmony;
int harmonymap;
int harmonynote;
int harmonyoctave;
int harmonyoff;
int randomswitch;
int randomswitchrun = 1;
int currentharm = 0;
int currentharmmap;

static const int harmonyarray [9] = {1, 3, 5, 6, 1, 3, 5, 6, 6};
static const int harmonyoctavearray [9] = {0, 0, 0, 0, 12, 12, 12, 12, 12};
char harmonyname [9] [7] = {"2nd", "4th", "6th", "7th", "2nd", "4th", "6th", "7th", "7th"};
char keyname[12] [5] = {"C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
char notename[128] [5] = {"C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G" };
char scalename[8] [6] = {"maj", "lyd", "mix", "min", "dor", "phr", "chr", "chr"};

//clock
const int clock_led = 12;

//steps
const int n1led = 2;
const int n2led = 3;
const int n3led = 4;
const int n4led = 5;
const int n5led = 6;
const int n6led = 7;
const int n7led = 8;
const int n8led = 9;
//pingpong
int pingpong = 0;
int pingpongswitch = 0;
const int pingpongpin = 11;
//on/off
int onoff = 10;
int offswitch = 1;

//states
enum notes {
  N1,
  N1b,
  N2,
  N3,
  N4,
  N5,
  N6,
  N7,
  N8,
  N8b,
  OFF
};

notes note, prior_note;

enum clock {
  ON,
  ON2,
  CLOCKOFF
};

clock clockswitch, priorclockswitch;

//functions
void off() {
  if(digitalRead(onoff) == HIGH){
    if (steptick - startstep == 1) {
      pingpong = 0;
      prior_note = OFF;
      note = N1;
      priorclockswitch = CLOCKOFF;
      clockswitch = ON;
      Serial.write(250);
      offswitch = 1;
    }
  } else {
    if (offswitch == 1) {
      digitalWrite(n1led, LOW);
      digitalWrite(n2led, LOW);
      digitalWrite(n3led, LOW);
      digitalWrite(n4led, LOW);
      digitalWrite(n5led, LOW);
      digitalWrite(n6led, LOW);
      digitalWrite(n7led, LOW);
      digitalWrite(n8led, LOW);
      midinoteoff();
      steptick = 0;
      Serial.write(252);
      offswitch = 0;
    }
    startstep = steptick;
  }
}

void onSync24Callback(uint32_t tick) {
  Serial.write(248);
}

void onStepCallback(uint32_t step) {
  ++steptick;
}

void n1b() {
  if (note != prior_note) {
    steptick1 = steptick;
    t1 = time1;
    prior_note = note; 
    digitalWrite(n1led, HIGH);
    currentnote = (note1 + octavenote + rootnote);
    current3rd = (note13rd + octavenote + rootnote);
    current5th = (note15th + octavenote + rootnote);
    currentharmmap = notearray[(note1map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }
  
  if (steptick - steptick1 == t1) {
    note = N2;
  }

  if (note != prior_note){
    digitalWrite(n1led, LOW);
    pingpong = 0;
    midinoteoff();
  }
}

void n1() {
  if (note != prior_note) {
    steptick1b = steptick;
    t1 = time1;
    prior_note = note; 
    digitalWrite(n1led, HIGH);
    currentnote = (note1 + octavenote + rootnote);
    current3rd = (note13rd + octavenote + rootnote);
    current5th = (note15th + octavenote + rootnote);
    currentharmmap = notearray[(note1map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }
  
  if (steptick - steptick1b == t1) {
    if(pingpong == 1){
      note = N1b;
    } else {
      note = N2;
    }
  }

  if (note != prior_note){
    digitalWrite(n1led, LOW);
    midinoteoff();
  }
}

void n2() {
  if (note != prior_note) {
    steptick2 = steptick;
    t2 = time2;
    prior_note = note; 
    digitalWrite(n2led, HIGH);
    currentnote = (note2 + octavenote + rootnote);
    current3rd = (note23rd + octavenote + rootnote);
    current5th = (note25th + octavenote + rootnote);
    currentharmmap = notearray[(note2map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }

  if (steptick - steptick2 == t2) {
    if(pingpong == 1){
      note = N1;
    } else {
      note = N3;
    }
  }

  if (note != prior_note){
    digitalWrite(n2led, LOW);
    midinoteoff();
  }
}

void n3() {
  if (note != prior_note) {
    steptick3 = steptick;
    t3 = time3;
    prior_note = note; 
    digitalWrite(n3led, HIGH);
    currentnote = (note3 + octavenote + rootnote);
    current3rd = (note33rd + octavenote + rootnote);
    current5th = (note35th + octavenote + rootnote);
    currentharmmap = notearray[(note3map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }
  
  if (steptick - steptick3 == t3) {
    if(pingpong == 1){
      note = N2;
    } else {
      note = N4;
    }
  }

  if (note != prior_note){
    digitalWrite(n3led, LOW);
    midinoteoff();
  }
}

void n4() {
  if (note != prior_note) {
    steptick4 = steptick;
    t4 = time4;
    prior_note = note; 
    digitalWrite(n4led, HIGH);
    currentnote = (note4 + octavenote + rootnote);
    current3rd = (note43rd + octavenote + rootnote);
    current5th = (note45th + octavenote + rootnote);
    currentharmmap = notearray[(note4map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }

  if (steptick - steptick4 == t4){
    if(pingpong == 1){
      note = N3;
    } else {
      note = N5;
    }
  }

  if (note != prior_note){
    digitalWrite(n4led, LOW);
    midinoteoff();
  }
}

void n5() {
  if (note != prior_note) {
    steptick5 = steptick;
    t5 = time5;
    prior_note = note; 
    digitalWrite(n5led, HIGH);
    currentnote = (note5 + octavenote + rootnote);
    current3rd = (note53rd + octavenote + rootnote);
    current5th = (note55th + octavenote + rootnote);
    currentharmmap = notearray[(note5map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }
  
  if (steptick - steptick5 == t5){
    if(pingpong == 1){
      note = N4;
    } else {
      note = N6;
    }
  }

  if (note != prior_note){
    digitalWrite(n5led, LOW);
    midinoteoff();
  }
}

void n6() {
  if (note != prior_note) {
    steptick6 = steptick;
    t6 = time6;
    prior_note = note; 
    digitalWrite(n6led, HIGH);
    currentnote = (note6 + octavenote + rootnote);
    current3rd = (note63rd + octavenote + rootnote);
    current5th = (note65th + octavenote + rootnote);
    currentharmmap = notearray[(note7map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }

  if (steptick - steptick6 == t6){
    if(pingpong == 1){
      note = N5;
    } else {
      note = N7;
    }
  }

  if (note != prior_note){
    digitalWrite(n6led, LOW);
    midinoteoff();
  }
}

void n7() {
  if (note != prior_note) {
    steptick7 = steptick;
    t7 = time7;
    prior_note = note; 
    digitalWrite(n7led, HIGH);
    currentnote = (note7 + octavenote + rootnote);
    current3rd = (note73rd + octavenote + rootnote);
    current5th = (note75th + octavenote + rootnote);
    currentharmmap = notearray[(note1map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }
  
  if (steptick - steptick7 == t7){
    if(pingpong == 1){
      note = N6;
    } else {
      note = N8;
    }
  }

  if (note != prior_note){
    digitalWrite(n7led, LOW);
    midinoteoff();
  }
}

void n8() {
  if (note != prior_note) {
    steptick8 = steptick;
    t8 = time8;
    prior_note = note; 
    digitalWrite(n8led, HIGH);
    currentnote = (note8 + octavenote + rootnote);
    current3rd = (note83rd + octavenote + rootnote);
    current5th = (note85th + octavenote + rootnote);
    currentharmmap = notearray[(note8map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }

  if (steptick - steptick8 == t8){
    if (pingpongswitch == 1){
      pingpong = 1;
    } else {
      pingpong = 0;
    }
    if(pingpong == 1){
      note = N8b;
    } else {
      note = N1;
    }
  }

  if (note != prior_note){
    digitalWrite(n8led, LOW);
    digitalWrite(n8led, LOW);
    midinoteoff();
  }  
}

void n8b() {
  if (note != prior_note) {
    steptick8b = steptick;
    t8 = time8;
    prior_note = note; 
    digitalWrite(n8led, HIGH);
    currentnote = (note8 + octavenote + rootnote);
    current3rd = (note83rd + octavenote + rootnote);
    current5th = (note85th + octavenote + rootnote);
    currentharmmap = notearray[(note8map + harmony)];
    currentharm = (currentharmmap + octavenote + harmonyoctave + rootnote);
    midinoteon();
  }

  if (steptick - steptick8b == t8){
    if(pingpong = 1){
      note = N7;
    } else {
      note = N1;
    }
  }

  if (note != prior_note){
    digitalWrite(n8led, LOW);
    midinoteoff();
  }

}

void clockledon() {

  if (clockswitch != priorclockswitch) {
    clockstep = steptick;
    priorclockswitch = clockswitch;
    digitalWrite(clock_led, HIGH);
  }
  
  if (steptick - clockstep == 1) {
    clockswitch = ON2;
  }

  if (clockswitch != priorclockswitch){
    digitalWrite(clock_led, LOW);
  }
}

void clockledon2() {

  if (clockswitch != priorclockswitch) {
    clockstep = steptick;
    priorclockswitch = clockswitch;
    digitalWrite(clock_led, LOW);
  }
  
  if (steptick - clockstep == 1) {
    clockswitch = ON;
  }

  if (clockswitch != priorclockswitch){
    digitalWrite(clock_led, HIGH);
  }
}

void midinoteon() {
  if (mode == "mono") {  
  MIDI.sendNoteOn(currentnote, 127, 1);
  } else {
    MIDI.sendNoteOn(currentnote, 127, 1);
    MIDI.sendNoteOn((currentnote - 12), 127, 1);
    MIDI.sendNoteOn(current3rd, 127, 1);
    MIDI.sendNoteOn(current5th, 127, 1);
    if (harmonyoff == 0) {
    } else {
      MIDI.sendNoteOn(currentharm, 127, 1);
    }
  }

}

void midinoteoff () {
  MIDI.sendNoteOff(currentnote, 0, 1);
  MIDI.sendNoteOff((currentnote - 12), 0, 2);
  MIDI.sendNoteOff((currentnote - 12), 0, 1);
  MIDI.sendNoteOff(current3rd, 0, 1);
  MIDI.sendNoteOff(current5th, 0, 1);
  MIDI.sendNoteOff(currentharm, 0, 1); 
}

void randomgen () {
  if ((randomswitch == 1) || (randomswitch == 2))  {
    if (randomswitchrun == 1) {
        note8map = random(0, 14);
        note8 = notearray[note8map];
        note83rd = notearray[(note8map + 2)];
        note85th = notearray[(note8map +4)];
        note7map = random(0, 14);
        note7 = notearray[note7map];
        note73rd = notearray[(note7map + 2)];
        note75th = notearray[(note7map +4)];
        note6map = random(0, 14);
        note6 = notearray[note6map];
        note63rd = notearray[(note6map + 2)];
        note65th = notearray[(note6map +4)];
        note5map = random(0, 14);
        note5 = notearray[note5map];
        note53rd = notearray[(note5map + 2)];
        note55th = notearray[(note5map +4)];
        note4map = random(0, 14);
        note4 = notearray[note4map];
        note43rd = notearray[(note4map + 2)];
        note45th = notearray[(note4map +4)];
        note3map = random(0, 14);
        note3 = notearray[note3map];
        note33rd = notearray[(note3map + 2)];
        note35th = notearray[(note3map +4)];
        note2map = random(0, 14);
        note2 = notearray[note2map];
        note23rd = notearray[(note2map + 2)];
        note25th = notearray[(note2map +4)];
        note1map = random(0, 14);
        note1 = notearray[note1map];
        note13rd = notearray[(note1map + 2)];
        note15th = notearray[(note1map +4)];
        time8map = random(0, 3);
        time8 = timearray[time8map];
        time7map = random(0, 3);
        time7 = timearray[time7map];
        time6map = random(0, 3);
        time6 = timearray[time6map];
        time5map = random(0, 3);
        time5 = timearray[time5map];
        time4map = random(0, 3);
        time4 = timearray[time4map];
        time3map = random(0, 3);
        time3 = timearray[time3map];
        time2map = random(0, 3);
        time2 = timearray[time2map];
        time1map = random(0, 3);
        time1 = timearray[time1map];
        randomswitchrun = 0;
      }
  } else {
    if (randomswitchrun == 0) {
      for (int i = 0; i < 16; i++) {
        int notereset = notemuxvalues[i];
        notemuxvalues[i] = (notereset + 12);
        randomswitchrun = 1;
      }
    }
  }
}

void checkmux () {

  unsigned long notereadstart = micros();
  static unsigned long notereadprevious = 0;

  if (notereadstart - notereadprevious >= 100) {
    notereadprevious = notereadstart;

    int notemuxread = analogRead(notetimemuxin);
    notemap = map(notemuxread, 0, 1023, 0, 14);
    timemap = map(notemuxread, 0, 1023, 0, 8);

    if (notemuxread > (notemuxvalues[noteinput] + 10) || notemuxread < (notemuxvalues[noteinput] - 10)) {
      notemuxvalues[noteinput] = notemuxread;

      switch (noteinput) {
        default:
        Serial.print("NOTHING");
        case 0:
          note8map = notemap;
          note8 = notearray[notemap];
          note83rd = notearray[(notemap + 2)];
          note85th = notearray[(notemap +4)];
          break;
        case 1:
          note7map = notemap;
          note7 = notearray[notemap];   
          note73rd = notearray[(notemap + 2)];
          note75th = notearray[(notemap +4)];                    
          break;
        case 2:
          note6map = notemap;
          note6 = notearray[notemap]; 
          note63rd = notearray[(notemap + 2)];
          note65th = notearray[(notemap +4)];        
          break;
        case 3:
          note5map = notemap;
          note5 = notearray[notemap]; 
          note53rd = notearray[(notemap + 2)];
          note55th = notearray[(notemap +4)];        
          break;
        case 4:
          note4map = notemap;
          note4 = notearray[notemap]; 
          note43rd = notearray[(notemap + 2)];
          note45th = notearray[(notemap +4)];      
          break;
        case 5:
          note3map = notemap;
          note3 = notearray[notemap];
          note33rd = notearray[(notemap + 2)];
          note35th = notearray[(notemap +4)];          
          break;
        case 6:
          note2map = notemap;
          note2 = notearray[notemap];
          note23rd = notearray[(notemap + 2)];
          note25th = notearray[(notemap +4)];        
          break;
        case 7:
          note1map = notemap;
          note1 = notearray[notemap];
          note13rd = notearray[(notemap + 2)];
          note15th = notearray[(notemap +4)];        
          break;
        case 8:
          time8 = timearray[timemap];
          break;
        case 9:
          time7 = timearray[timemap];
          break;
        case 10:
          time6 = timearray[timemap];
          break;
        case 11:
          time5 = timearray[timemap];
          break;
        case 12:
          time4 = timearray[timemap];
          break;
        case 13:
          time3 = timearray[timemap];
          break;
        case 14:
          time2 = timearray[timemap];
          break;
        case 15:
          time1 = timearray[timemap];
          break;
      }
    
    }
    
    ++noteinput;
    if (noteinput >= muxchannels) noteinput = 0;

    int controlmuxread = analogRead(controlmuxin);

    if (controlmuxread > (controlmuxvalues[controlinput] + 10) || controlmuxread < (controlmuxvalues[controlinput] - 10)) {
      controlmuxvalues[controlinput] = controlmuxread;

      switch (controlinput) {
        case 0:
          bpm = map(controlmuxread, 0, 1023, 40, 200);
          uClock.setTempo(bpm);
          updatescreen();
          break;
        case 1:
          rootnote = map(controlmuxread, 0, 1023, 0, 11);
          updatescreen();
          break;
        case 2:
          scale = map(controlmuxread, 0, 1023, 0, 7);
          switch (scale) {
            case 0: //major
              notearray = majorscale;
              break;
            case 1: //lydian
              notearray = lydianscale;
              break;
            case 2: // mixolydian
              notearray = mixolydianscale;
              break;
            case 3: //minor
              notearray = minorscale;
            case 4: //dorian
              notearray = dorianscale;
              break;
            case 5: //phrygian
              notearray = phrygianscale;
              break;
            case 6: //chromatic (poly mode won't sound nice...)
              notearray = chromaticscale;
              break;
            case 7:
              notearray = chromaticscale;
              break;
          }
          for (int i = 0; i < 8; i++) {
            int notereset = notemuxvalues[i];
            notemuxvalues[i] = (notereset + 12);
          }
          updatescreen();
          break;
        case 3:
          octave = map(controlmuxread, 0, 1023, 0, 5);
          octavenote = octavearray[octave];
          updatescreen();
          break;
        case 4:
          modemap = analogRead(controlmuxin);
          modemap = map(modemap, 0, 1023, 0, 2);
          mode = modes[modemap];
          updatescreen();
          break;
        case 5:
          harmonymap = map(controlmuxread, 0, 1023, 0, 8);
          harmony = harmonyarray[harmonymap];
          harmonyoctave = harmonyoctavearray[harmonymap];
          updatescreen();
          break;
        case 6:
          harmonyoff = map(controlmuxread, 0 ,1023, 0, 2);
          updatescreen();
          break;
        case 7:
          randomswitch = map(controlmuxread, 0, 1023, 0, 2);
          updatescreen();
          break;
      }

    }

    ++controlinput;
    if (controlinput >= muxchannels) controlinput = 0;
    digitalWrite(A0, controlinput & B0001);
    digitalWrite(A1, controlinput & B0010);
    digitalWrite(A2, controlinput & B0100);
    digitalWrite(A3, controlinput & B1000);
    
  }


}

void updatescreen() {
 
  oled.clear();
  oled.set2X();
  oled.print(modes[modemap]);
  oled.println(" ");
  oled.print(bpm);
  oled.print(" ");
  oled.println(octavenote);
  oled.print(keyname[rootnote]);
  oled.print(" ");
  oled.println(scalename[scale]);
  if (harmonyoff == 0) {
    oled.println("harm off");
  } else {
      oled.println(harmonyname[harmonymap]);
  }
}

void setup() {

  MIDI.begin(MIDI_CHANNEL_OFF);
  delay(100);

  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);

  delay(2000);

  oled.println("HIGHFIELD SYNTHESIS");
  oled.println("POLYPHONIC MIDI");
  oled.println("STEP SEQUENCER");

  pinMode(clock_led, OUTPUT);
  digitalWrite(clock_led, LOW);
  pinMode(n1led, OUTPUT);
  digitalWrite(n1led, LOW);
  pinMode(n2led, OUTPUT);
  digitalWrite(n2led, LOW);
  pinMode(n3led, OUTPUT);
  digitalWrite(n3led, LOW);
  pinMode(n4led, OUTPUT);
  digitalWrite(n4led, LOW);
  pinMode(n5led, OUTPUT);
  digitalWrite(n5led, LOW);
  pinMode(n6led, OUTPUT);
  digitalWrite(n6led, LOW);
  pinMode(n7led, OUTPUT);
  digitalWrite(n7led, LOW);
  pinMode(n8led, OUTPUT);
  digitalWrite(n8led, LOW);

  randomSeed(analogRead(0));

  pinMode(pingpongpin, INPUT);

  pinMode(onoff, INPUT);
  
  pinMode(controlmuxin, INPUT);
  pinMode(notetimemuxin, INPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  delay(2000);

  pingpong = 0;
  prior_note = OFF;
  note = N1;
  priorclockswitch = CLOCKOFF;
  clockswitch = ON;
  notearray = majorscale;
  
  uClock.init();
  uClock.setOnSync24(onSync24Callback);
  uClock.setOnStep(onStepCallback);
  uClock.setTempo(120);
  uClock.start();

}

void loop() {

  //on/off
  if(digitalRead(onoff) == LOW){
    note = OFF;
    clockswitch = CLOCKOFF;
  } 
  //state switches
  switch (note) {
    case N1b:
      n1b();
      break;
    case N1:
      n1();
      break;
    case N2:
      n2();
      break;
    case N3:
      n3();
      break;
    case N4:
      n4();
      break;
    case N5:
      n5();
      break;
    case N6:
      n6();
      break;
    case N7:
      n7();
      break;    
    case N8:
      n8();
      break;
    case N8b:
      n8b();
      break;
    case OFF:
      off();
      break;

  }

  switch (clockswitch) {
    case ON:
      clockledon();
      break;
    case ON2:
      clockledon2();
      break;
    case CLOCKOFF:
      digitalWrite(clock_led, LOW);
      break;
  }
  
  //pingpong switch
  if(digitalRead(pingpongpin) == HIGH){
    pingpongswitch = 1;
  } else {
    pingpongswitch = 0;
  }
  
  checkmux();
  randomgen();

}
