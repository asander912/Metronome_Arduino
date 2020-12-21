/*
 * Metronome code for an Arduino Uno microcontroller.
 * Andrew Sander
 * 
  */
  #include <LiquidCrystal.h>

  #define MAX_TEMPO_READ 975 //any analog in readings on tempoPin above this will be treated as 975
  #define MAX_TEMPO_BPM 180 //maximum tempo in Beats Per Minute
  #define MIN_TEMPO_BPM 40 //minimum tempo in Beats Per Minute

  LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

  //Assign pins
  int speakerPin = 4; //piezospeaker
  int tempoPin = A0;  //adjust tempo
  int switchPin = 2; //used to turn metronome on and off
  int subdivPin = 3; //used to switch between different subdivisions

  int tempo; //in beats per minute

  boolean volatile met_run = false; //true if metronome is running
  int volatile subdiv = 1; //subdivision of beat played by the metronome. 1 = quarter notes

  unsigned long met_run_time = millis();
  unsigned long subdiv_time = millis();

/*
 * Initialize lcd display and pin modes (INPUT/OUTPUT as well as attach interrupts to INPUT pins.)
 * 
 */

void setup() {

  lcd.begin(16, 2);
  lcd.clear();

  pinMode(speakerPin, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(subdivPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(switchPin), switch_mode, FALLING);
  attachInterrupt(digitalPinToInterrupt(subdivPin), switch_subdiv, FALLING);

  analogReference(DEFAULT); //5 V

}

/*
 * Main loop function.
 * If metronome is not turned on, do nothing.
 * If the metronome is on,
 * calculate the tempo, and map analog reading to a tempo between MIN_TEMPO_BPM and MAX_TEMPO_BPM.
 * Print the current tempo and subdivision to the lcd.
 */

void loop() {

  if(!met_run){
    lcd.clear();
    lcd.print("Press");
    lcd.setCursor(0, 1);
    lcd.print("On-Off button");
    while(!met_run){
      continue;
      }
    }

    
  tempo = map(constrain(analogRead(tempoPin), 0, MAX_TEMPO_READ), 0, MAX_TEMPO_READ, MIN_TEMPO_BPM, MAX_TEMPO_BPM);
  tone(speakerPin, 500, 25); //does not block

  delay((unsigned int)((60000/(tempo*subdiv)))); //milliseconds per beat

  print_screen();

}

/*
 * If the metronome is on, turn it off.
 * If the metronome is off, turn it on.
 * Start the program with the metronome being off.
 */
void switch_mode(){
  if(millis() - met_run_time > 500){
    met_run = !met_run;
    met_run_time = millis();
    }

}

/*
 * Change subdivision to be quarters, eighths triplets, or sixteenths.
 */
void switch_subdiv(){
  if(met_run == true && millis() - subdiv_time > 500){
    subdiv_time = millis();
    if(subdiv < 4){
      subdiv++;
    }
    else{
      subdiv = 1;
    }
  }
}

/*
 * Print tempo and subdivision to the lcd screen.
 */

 void print_screen(){
  lcd.clear();
  lcd.print(tempo);
  lcd.setCursor(0, 1);
  switch(subdiv){
    case 1:
    lcd.print("Quarters");
    break;
    
    case 2:
    lcd.print("Eighths");
    break;
    
    case 3:
    lcd.print("Triplets");
    break;
    
    case 4:
    lcd.print("Sixteenths");
    break;
    
    }
  }




  
