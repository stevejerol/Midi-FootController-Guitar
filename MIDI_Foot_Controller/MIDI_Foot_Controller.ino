#include<Bounce2.h>
#include <MIDI.h>
#include <ShiftRegister74HC595.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

ShiftRegister74HC595<1> sr(11, 12, 13);
MIDI_CREATE_DEFAULT_INSTANCE();
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define NUM_BUTTONS 10
#define CC1 2
#define CC2 3
#define CC3 4
#define CC4 5
#define BUP 6
#define PC1 7
#define PC2 8
#define PC3 9
#define PC4 10
#define BDOWN 14
int BUTTON_PINS[NUM_BUTTONS] = {CC1,CC2, CC3, CC4, BUP,PC1,PC2,PC3,PC4,BDOWN};

int bank=0;
int flag=0;

Bounce * buttons = new Bounce[NUM_BUTTONS];




void setup() {
  // put your setup code here, to run once:

for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
    buttons[i].interval(25);}

    MIDI.begin(MIDI_CHANNEL_OMNI);
    Serial.begin(115200);
    sr.setAllLow();
    //initialize lcd screen
    lcd.init();
  // turn on the backlight
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Guitar Rig");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("PATCH : 1A");
    lcd.setCursor(0,1);
    lcd.print("BANK : 1");
    }

void loop() {
  // put your main code here, to run repeatedly:
  
   for (int i = 0; i < NUM_BUTTONS; i++) 
   {
    // Update the Bounce instance :
    buttons[i].update();
    if(buttons[i].fell())
      {
          flag=1;
          if(i<4)
          {
            if(i==0)
            {
              MIDI.sendControlChange(1,127,1);              
            }
            if(i==1)
            {
              MIDI.sendControlChange(2,127,1);
            }
            if(i==2)
            {
              MIDI.sendControlChange(3,127,1);
            }
            if(i==3)
            {
              MIDI.sendControlChange(4,127,1);
            }
            if(sr.get(i)==0){sr.set(i,HIGH);}
            else sr.set(i,LOW);
            
          }
       else if(i>=5 && i<=8)
            {
               lcd.setCursor(8,0);
               lcd.print(bank+1);
               lcd.setCursor(9,0);
               if(i==5)
               {
                  MIDI.sendProgramChange(((bank*4)+1),1);
                  lcd.print("A");
               }
               if(i==6)
               {
                  MIDI.sendProgramChange(((bank*4)+2),1);
                  lcd.print("B");
               }     
               if(i==7)
               {
                  MIDI.sendProgramChange(((bank*4)+3),1);
                  lcd.print("C");
               }
               if(i==8)
               {
                  MIDI.sendProgramChange(((bank*4)+4),1);
                  lcd.print("D");
               }
               for(int c=4;c<8;c++)
               {
                sr.set(c,LOW);
               }
               sr.set(i-1,HIGH);
            }
       else if(i==4 || i==9)
            {
              if(i==4)
                {
                  bank=bank+1;
                  
                }
              else if(i==9 && bank!=0)
                   {
                      bank=bank-1;
                   }
              lcd.setCursor(7,1);
              lcd.print(bank+1);     
            }
        }}
      }
