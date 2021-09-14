
/* Sam C; Adi R; Andrew C; */

unsigned long  snake_time = 0; 
int system_state = 0; 
int LED_PIN_1 = 11; 
int LED_PIN_2 = 10; 
int LED_PIN_3 = 6;
int LED_PIN_4 = 5;
int POT_PIN = A0; 
int pot_value = 0; 
int led_state = 0x00;
long led_num = 0x00; 
const int buttonPin = 2;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 35; 
unsigned long current_time = 0;
uint32_t breathe_period = 4096; 
uint32_t pong_period    = 4096; 
bool breathe_rising = true; 
bool prev_time = LOW;
uint32_t led_pin_to_change = 0; 
int first_time = true; 
unsigned long start_time = 0;
unsigned long previousMillis = 0;  


void set_all_leds(uint8_t set_val){
//  scale set_val based on pot_value
//  set_val = (uint8_t)(((float) set_val) * (((float) pot_value) / 1024.0)); 
  set_val = map(pot_value, 0, 1023, 0, set_val); 
  analogWrite(LED_PIN_1, set_val); 
  analogWrite(LED_PIN_2, set_val); 
  analogWrite(LED_PIN_3, set_val); 
  analogWrite(LED_PIN_4, set_val); 
}

void pong_on(long led_pin_num){
   // shut off everything 
   led_state &= 0x00; 
   set_all_leds(led_state); 

   uint8_t set_val = 0xFF; 
   set_val = map(pot_value, 0, 1023, 0, set_val);
   // turn on the desired led pin number 
   if (led_pin_num == 0){
      analogWrite(LED_PIN_1, set_val); 
   } else if (led_pin_num == 1){
      analogWrite(LED_PIN_2, set_val); 
   } else if (led_pin_num == 2){
      analogWrite(LED_PIN_3, set_val); 
   } else if (led_pin_num == 3 or led_pin_num == 4){
      analogWrite(LED_PIN_4, set_val); 
   }
}

void reset_after_state_change(){
 first_time = true; 
 breathe_rising = true; 
 led_state = 0x00; 
 set_all_leds(led_state); 
 led_pin_to_change = 0; 
}


void button_press(){
      // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);
//     Serial.println(system_state);
  
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:
  
    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }
  
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
  
      // if the button state has changed:
      
      if (reading != buttonState) {
        buttonState = reading;
  
        // only toggle the LED if the new button state is HIGH
        if (buttonState == HIGH) {
  
          //ledState = !ledState;
          system_state++;
        }
        if (system_state > 4){
            system_state = 0;
          }
      }
    }

  // set the LED:
  //digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Test");
  pinMode(buttonPin, INPUT);
  pinMode(LED_PIN_1,OUTPUT);
  pinMode(LED_PIN_2,OUTPUT);
  pinMode(LED_PIN_3,OUTPUT);
  pinMode(LED_PIN_4,OUTPUT);  
  // digitalWrite(buttonPin, system_state);

}

void loop () {
//   Serial.println(system_state);

  button_press();
 
 // read in pot for input

 pot_value = analogRead(POT_PIN); 
// Serial.println(pot_value); 
//
  switch(system_state){
    case 0: // ON
      led_state &= 0x00; 
      set_all_leds(led_state); 
      break; 
    case 1: // OFF
      led_state |= 0xFF; 
      set_all_leds(led_state); 
      break;
    case 2: // FLASH
      start_time = millis();
    
      if (start_time - previousMillis >= 1000) {
      // save the last time you blinked the LED 
      previousMillis = start_time;
     
      // if the LED is off turn it on and vice-versa:
      if (prev_time == LOW) {
        led_state |= 0xFF;
        prev_time = HIGH;
      } else {
        led_state &= 0x00;
        prev_time = LOW; 
      }
  
      // set the LED with the ledState of the variable:
      //digitalWrite(ledPin, ledState);
      set_all_leds(led_state);         
      }
      break; 
    case 3: // BREATHE
//      button_press();
      if (first_time) {
          start_time = millis();
          first_time = false; 
      }  

      if (breathe_rising){
        current_time = millis()-start_time;
        led_state = map(current_time, 0, 2000, 0, 255); 
        set_all_leds(led_state); // TODO check types
        if (led_state == 255){
          breathe_rising = false; 
          start_time = start_time + current_time;
        }
      } else {
        current_time = millis()-start_time;
        led_state = map(current_time, 0, 2000, 255, 0);
        set_all_leds(led_state); // TODO check types
        if (led_state == 0){
          breathe_rising = true; 
          start_time = start_time + current_time;
        }
      }
      break; 

    case 4: // PONG
      if (first_time) {
          start_time = millis();
          first_time = false; 
      }  

      current_time = millis()-start_time;
      // select LED to turn on 
      led_num = map(current_time, 0, 2000, 0, 4); 
      pong_on(led_num); 
      if (led_num == 4){
        start_time = start_time + current_time;
      }
//     
      break; 
    default: 
//      button_press();

      Serial.println("hi"); 
      break;   
    
 }
 }
