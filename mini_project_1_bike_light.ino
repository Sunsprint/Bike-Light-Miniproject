

#include <neopixel> // FIX ME 

enum enum_light_state ={
  OFF     = 0, 
  ON      = 1, 
  FLASH   = 2, 
  BREATHE = 3, 
  PONG    = 4, 
}

uint32_t start_time; 
uint8_t led_state = 0x00; 
enum_light_state system_state = OFF; 
bool first_time = true;  


// LED are on pins 11, 10, 6, 5
int LED_PIN_1 = 11;
int LED_PIN_2 = 10;
int LED_PIN_3 = 6;
int LED_PIN_4 = 5;
const int buttonPin = 2;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 

void set_all_leds(uint8_t set_val){
  
}
void setup() {
  Serial.begin(9600);
  Serial.println("Test");
  pinMode(buttonPin, INPUT);
  pinMode(LED_PIN_1,OUTPUT);
  pinMode(LED_PIN_2,OUTPUT);
  pinMode(LED_PIN_3,OUTPUT);
  pinMode(LED_PIN_4,OUTPUT);  
}

void loop () {

// debounce stuff 
  int reading = digitalRead(buttonPin);

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
        if (system_state > 4){
          system_state = 0;
       else{
        system_state++;
       }
      }
    }
  }
 
//  read in pot for input

  switch(system_state){
    case OFF: 
      led_state &= 0x00; 
      set_all_leds(led_state); 
    case ON: 
      led_state |= 0xFF; 
      set_all_leds(led_state); 
      
    case FLASH: 
      if (first_time) {
        start_time = millis();
        first_time = false; 
      }      
      
      if (millis() - start_time >= 500) {
        first_time = true; 
        led_state != led_state; 
        set_all_leds(led_state);         
      }

    case BREATHE: 

    case PONG: 
    
    default: 
      
    
  }
}
