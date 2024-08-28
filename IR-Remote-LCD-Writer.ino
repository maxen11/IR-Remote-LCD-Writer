#include <LiquidCrystal.h>
#include "IRremote.h"
//#include "IR.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);

#define RIGHT (0xFFC23D)
#define LEFT (0xFF22DD)
#define UP (0xFF629D)
#define DOWN (0xFFA857)

int reciever = 6;
IRrecv irrecv(reciever);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

int posX = 0;
int posY = 0;

char alphabet[] = {' ','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char alphaLower[] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char text[32];
//bool isUpper = true;

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn();
  reset();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.blink();
 // printText();
  //lcd.setCursor(posX, posY);
}

void loop(){ 
  if(irrecv.decode(&results)){
    switch(results.value){
      case RIGHT:
          //posX = posX+1;
          posX++;
          Serial.println("RIGHT");
        if(posX > 15){
          posY = posY ^ 1;
          posX = 0;
        }
        lcd.setCursor(posX, posY);
        break;
    case LEFT:
      posX--;
      lcd.setCursor(posX, posY);
      Serial.println("LEFT");
      if(posX < 0){
        posY = posY ^ 1;
        posX = 15; 
      }
      lcd.setCursor(posX, posY);
      break;
    case UP:
      Serial.println("UP");
      int pos;
      pos = getCurrPos();
      text[pos] = checkNextChar(text[pos], 1);
      printText();
      break;
    case DOWN:
        Serial.println("DOWN");
        int ind;
        ind = getCurrPos();
        text[ind] = checkNextChar(text[ind], -1);
        printText();
        break;
    case 0xFFE21D: // func/stop
      reset();
      break;
    case 0xFF906F: // UpArrow
     toUpper();
     break;
    case 0xFFE01F: // DownArrow
      toLower();
      break;
    default:
      break;
    }
    irrecv.resume(); 
  }  
}

int getCurrPos(){
  int index;
   if(posY == 0){
     index = posX;
   }else if(posY == 1){
     index = posX+15;
   }
  return index;
}

char checkNextChar(char character, int next){
  char newChar;
  for(int i = 0; i < 27; i++){

    if(alphaLower[i] == character){
          newChar = alphaLower[(i+next)%27];
          break;
        } 
     else if(alphabet[i] == character){
          newChar = alphabet[(i+next)%27];
          break;
        }
  }
  Serial.println("newChar:");
  Serial.println(newChar);
  return newChar;
}

/*void printText(){
  Serial.println("text: ");
  Serial.println(text);
  lcd.clear();
  int x = 0;
  int y = 0;
  char* first = "";
  lcd.setCursor(x,y);
  for(int i = 0; i < 27; i++){
    if(i == 16){
      y = 1;
      x = 0;
      lcd.setCursor(x,y);
      lcd.print(first);
      first = "";
    }
    first[i] = text[i];
    //lcd.setCursor(x,y);
    Serial.println(first);
  }
  lcd.print(first);
  //lcd.print(text);
  lcd.setCursor(posX, posY);
}*/

void printText(){
  Serial.println("text: ");
  Serial.println(text);
  lcd.clear();
  lcd.print(text);
  lcd.setCursor(posX, posY);
}

void toLower(){
  int index = getCurrPos();
  char newChar;
  char character = text[index];
  for(int i = 0; i < 27; i++){
      if(alphabet[i] == character){
        newChar = alphaLower[i];
        text[index] = newChar;
        printText();
        break;
      }
    }
}

void toUpper(){
  int index = getCurrPos();
  char newChar;
  char character = text[index];
  for(int i = 0; i < 27; i++){
      if(alphaLower[i] == character){
          newChar = alphabet[i];
          text[index] = newChar;
          printText();
          break;
        }
   }
}

void reset(){
  lcd.clear();
  for(int i = 0; i < 32; i++)
    text[i] = ' ';
  posX = 0;
  posY = 0;   
}


// Other buttons 
//    switch(results.value)
//  {
//  case 0xFFA25D: Serial.println("POWER"); break;
//  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
//  case 0xFF629D: Serial.println("VOL+"); break;
//  case 0xFF22DD: Serial.println("FAST BACK");    break;
//  case 0xFF02FD: Serial.println("PAUSE");    break;
//  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
//  case 0xFFE01F: Serial.println("DOWN");    break;
//  case 0xFFA857: Serial.println("VOL-");    break;
//  case 0xFF906F: Serial.println("UP");    break;
//  case 0xFF9867: Serial.println("EQ");    break;
//  case 0xFFB04F: Serial.println("ST/REPT");    break;
//  case 0xFF6897: Serial.println("0");    break;
//  case 0xFF30CF: Serial.println("1");    break;
//  case 0xFF18E7: Serial.println("2");    break;
//  case 0xFF7A85: Serial.println("3");    break;
//  case 0xFF10EF: Serial.println("4");    break;
//  case 0xFF38C7: Serial.println("5");    break;
//  case 0xFF5AA5: Serial.println("6");    break;
//  case 0xFF42BD: Serial.println("7");    break;
//  case 0xFF4AB5: Serial.println("8");    break;
//  case 0xFF52AD: Serial.println("9");    break;
//  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  
//  default: 
//    Serial.println(" other button   ");

 // }// End Case
//}
