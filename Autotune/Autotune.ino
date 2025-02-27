#include <Audio.h>
#include "Autotune.h"
#include "MyDsp.h"
#include <vector>
#include <cstdlib> 
#include <cmath>

Autotune pitchShifter;
MyDsp echo;
AudioAnalyzeNoteFrequency notefreq;
AudioInputI2S in;
AudioOutputI2S out;

AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(in,0,pitchShifter,0);
AudioConnection patchCord1(in,1,notefreq,0);
AudioConnection patchCord2(in,0,echo,0);
AudioConnection patchCord3(echo,0,pitchShifter,0);
AudioConnection patchCord4(pitchShifter,0,out,0);
AudioConnection patchCord5(pitchShifter,0,out,1);



std::vector<float> frequencies_C = {
      16.35, 18.35, 20.60, 21.83, 24.50, 27.50, 30.87,  // C0 - B0
      32.70, 36.71, 41.20, 43.65, 49.00, 55.00, 61.74,  // C1 - B1
      65.41, 73.42, 82.41, 87.31, 98.00, 110.00, 123.47,  // C2 - B2
      130.81, 146.83, 164.81, 174.61, 196.00, 220.00, 246.94,  // C3 - B3
      261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88,  // C4 - B4
      523.25, 587.33, 659.26, 698.46, 783.99, 880.00, 987.77,  // C5 - B5
      1046.50, 1174.66, 1318.51, 1396.91, 1567.98, 1760.00, 1975.53,  // C6 - B6
      2093.00, 2349.32, 2637.02, 2793.83, 3135.96, 3520.00, 3951.07,  // C7 - B7
      4186.01   // C8
  };
std::vector<float> frequencies_D = {
      18.35, 20.60, 23.12, 24.50, 27.50, 30.87, 34.65,  // D0 - B0
      36.71, 41.20, 46.25, 49.00, 55.00, 61.74, 69.30,  // D1 - B1
      73.42, 82.41, 92.50, 98.00, 110.00, 123.47, 138.59,  // D2 - B2
      146.83, 164.81, 185.00, 196.00, 220.00, 246.94, 277.18,  // D3 - B3
      293.66, 329.63, 369.99, 392.00, 440.00, 493.88, 554.37,  // D4 - B4
      587.33, 659.26, 739.99, 783.99, 880.00, 987.77, 1108.73,  // D5 - B5
      1174.66, 1318.51, 1479.98, 1567.98, 1760.00, 1975.53, 2217.46,  // D6 - B6
      2349.32, 2637.02, 2959.96, 3135.96, 3520.00, 3951.07, 4434.92,  // D7 - B7
      4698.64   // D8
};
std::vector<float> frequencies_E = {
      20.60, 23.12, 25.96, 27.50, 30.87, 34.65, 38.89,  // E0 - B0
      41.20, 46.25, 51.91, 55.00, 61.74, 69.30, 77.78,  // E1 - B1
      82.41, 92.50, 103.83, 110.00, 123.47, 138.59, 155.56,  // E2 - B2
      164.81, 185.00, 207.65, 220.00, 246.94, 277.18, 311.13,  // E3 - B3
      329.63, 369.99, 415.30, 440.00, 493.88, 554.37, 622.25,  // E4 - B4
      659.26, 739.99, 830.61, 880.00, 987.77, 1108.73, 1244.51,  // E5 - B5
      1318.51, 1479.98, 1661.22, 1760.00, 1975.53, 2217.46, 2489.02,  // E6 - B6
      2637.02, 2959.96, 3322.44, 3520.00, 3951.07, 4434.92, 4978.03,  // E7 - B7
      5274.04   // E8
};
std::vector<float> frequencies_F = {
      21.83, 24.50, 27.50, 30.87, 34.65, 38.89, 43.65,  // F0 - B0
      46.25, 51.91, 58.27, 61.74, 69.30, 77.78, 87.31,  // F1 - B1
      92.50, 103.83, 116.54, 123.47, 138.59, 155.56, 174.61,  // F2 - B2
      185.00, 207.65, 233.08, 246.94, 277.18, 311.13, 349.23,  // F3 - B3
      369.99, 415.30, 466.16, 493.88, 554.37, 622.25, 698.46,  // F4 - B4
      739.99, 830.61, 932.33, 987.77, 1108.73, 1244.51, 1396.91,  // F5 - B5
      1479.98, 1661.22, 1864.66, 1975.53, 2217.46, 2489.02, 2793.83,  // F6 - B6
      2959.96, 3322.44, 3729.31, 3951.07, 4434.92, 4978.03, 5587.65,  // F7 - B7
      5919.91   // F8
};
std::vector<float> frequencies_G = {
      24.50, 27.50, 30.87, 34.65, 38.89, 43.65, 49.00,  // G0 - B0
      51.91, 58.27, 65.41, 69.30, 77.78, 87.31, 98.00,  // G1 - B1
      103.83, 116.54, 130.81, 138.59, 155.56, 174.61, 196.00,  // G2 - B2
      207.65, 233.08, 261.63, 277.18, 311.13, 349.23, 392.00,  // G3 - B3
      415.30, 466.16, 523.25, 554.37, 622.25, 698.46, 783.99,  // G4 - B4
      830.61, 932.33, 1046.50, 1108.73, 1244.51, 1396.91, 1567.98,  // G5 - B5
      1661.22, 1864.66, 2093.00, 2217.46, 2489.02, 2793.83, 3135.96,  // G6 - B6
      3322.44, 3729.31, 4186.01, 4434.92, 4978.03, 5587.65, 6271.93,  // G7 - B7
      6644.88   // G8
};
std::vector<float> frequencies_A = {
      27.50, 30.87, 34.65, 38.89, 43.65, 49.00, 55.00,  // A0 - B0
      58.27, 65.41, 73.42, 77.78, 87.31, 98.00, 110.00,  // A1 - B1
      116.54, 130.81, 146.83, 155.56, 174.61, 196.00, 220.00,  // A2 - B2
      233.08, 261.63, 293.66, 311.13, 349.23, 392.00, 440.00,  // A3 - B3
      466.16, 523.25, 587.33, 622.25, 698.46, 783.99, 880.00,  // A4 - B4
      932.33, 1046.50, 1174.66, 1244.51, 1396.91, 1567.98, 1760.00,  // A5 - B5
      1864.66, 2093.00, 2349.32, 2489.02, 2793.83, 3135.96, 3520.00,  // A6 - B6
      3729.31, 4186.01, 4698.64, 4978.03, 5587.65, 6271.93, 7040.00,  // A7 - B7
      7458.62   // A8
};
std::vector<float> frequencies_B = {
      30.87, 34.65, 38.89, 43.65, 49.00, 55.00, 61.74,  // B0 - B0
      65.41, 73.42, 82.41, 87.31, 98.00, 110.00, 123.47,  // B1 - B1
      130.81, 146.83, 164.81, 174.61, 196.00, 220.00, 246.94,  // B2 - B2
      261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88,  // B3 - B3
      523.25, 587.33, 659.26, 698.46, 783.99, 880.00, 987.77,  // B4 - B4
      1046.50, 1174.66, 1318.51, 1396.91, 1567.98, 1760.00, 1975.53,  // B5 - B5
      2093.00, 2349.32, 2637.02, 2793.83, 3135.96, 3520.00, 3951.07,  // B6 - B6
      4186.01, 4698.64, 5274.04, 5587.65, 6271.93, 7040.00, 7902.13,  // B7 - B7
      8372.02   // B8
};

std::vector<float> *currentFrequencies = &frequencies_C; 

void setup() {
  Serial.begin(6900);
  Serial.println("entersetup");
  AudioMemory(50);
  notefreq.begin(.15);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC); 
  audioShield.micGain(1);
  audioShield.volume(1);
  
  patchCord2.disconnect();   //Disconnect echo cord
  patchCord3.disconnect();
  Serial.println("Choisissez la cle: C, D, E, F, G, A, B");
  
}
float volume = 0.5;
int offset = 0;
float frequencyToMIDI(double frequency) {
    return (69 + 12 * log2(frequency / 440.0));
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(13);
    command.trim();
    if (command == "C") {
      currentFrequencies = &frequencies_C;}
    else if (command == "D") {
      currentFrequencies = &frequencies_D;}
    else if (command == "E") {
      currentFrequencies = &frequencies_E;}
    else if (command == "F") {
      currentFrequencies = &frequencies_F;}
    else if (command == "G") {
      currentFrequencies = &frequencies_G;}
    else if (command == "A") {
      currentFrequencies = &frequencies_A;}
    else if (command == "B") {
      currentFrequencies = &frequencies_B;}  
    else if (command.indexOf("offset") >= 0) {
      int index = command.indexOf(" ");
      if (index != -1){
        index ++;
        String val = command.substring(index);
        offset = val.toInt();
        Serial.printf("offset set = %d",offset);
      }
    }
    else if (command.indexOf("echo") >= 0){
      patchCord0.disconnect();   //Connect echo cord
      patchCord2.connect();
      patchCord3.connect();
    }
    else if (command.indexOf("oche") >= 0){
      patchCord2.disconnect();   //Disconnect echo
      patchCord3.disconnect();
      patchCord0.connect();
    }
     else if (command.indexOf("volume") >= 0) {
      int index = command.indexOf(" ");
      if (index != -1){
        index ++;
        String val = command.substring(index);
        volume = val.toFloat();
        audioShield.volume(volume);
        Serial.printf("volume set = %.2f",volume);
      }
    }
    Serial.println("Change completed");
  }
  pitchShifter.setParamValue("thresh", -50 );
  if (notefreq.available()) {
        float note= notefreq.read();
        // if ((note < 100)){
        //   return;
        // }
        float prob = notefreq.probability();  
        if (prob > 0.8){
          float min = 200;
          float nearest_note;
          for (size_t i = 0; i < currentFrequencies->size(); ++i) {
            if (std::abs(note - (*currentFrequencies)[i]) < min){
              nearest_note = (*currentFrequencies)[i];
              min = std::abs(note - (*currentFrequencies)[i]);
            }
          }
          float freq = frequencyToMIDI(nearest_note) - frequencyToMIDI(note);          
          pitchShifter.setParamValue("freq", freq + offset );
          delay(100);
          Serial.print(frequencyToMIDI(note));
          Serial.print(" ");
          Serial.println(frequencyToMIDI(nearest_note));
        }
        
    }
}
