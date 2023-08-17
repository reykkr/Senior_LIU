#include<LiquidCrystal.h>
#include <Keypad.h>
#include<EEPROM.h>
#include <Servo.h>


#define piezo 11
#define red 13
#define green 12

/*=============================================================*/



//for lcd
char pass[4], oldpass[4], newpass[4]; //password length

int i=0;

LiquidCrystal lcd(4,5,6,7,8,9);


/*=============================================================*/


//for keypad
char keys=0; //the key pressed on the keypad is defined by this char

const byte numrows = 4; 

const byte numcols = 4; 

char keymap[numrows][numcols] = { //keypad mapping

{'1','2','3','A'},

{'4','5','6','B'},

{'7','8','9','C'},

{'*','0','#','D'}

};

byte rowpins[numrows] = {A0,A1,A2,A3}; //row pins from 1-4

byte colpins[numcols] = {A4,A5,2,3}; //column pins from 1-4

Keypad myKeymap= Keypad(makeKeymap(keymap), rowpins, colpins, numrows, numcols);

/*=============================================================*/


//for servo
Servo servo;



/*=============================================================*/

//setup
void setup()

{
  

  //servo
  servo.attach(10);//servo pin
  servo.write(90);//servo(door)closed

  
  
  
  pinMode(red, OUTPUT);
  
  pinMode(green, OUTPUT);
  
  pinMode(piezo, OUTPUT);

//leds
   digitalWrite(red,HIGH);
  
   digitalWrite(green,LOW);
  
  
  //lcd
  
    lcd.begin(16,2);
  
  lcd.print("press *");
  lcd.setCursor(0,1);
 

}



/*the initial password that the program will start with
will be stored in the eeprom of the arduino*/

void password(){
  

for(int z=0;z<4;z++)/* password length is 6, the initial password
will start from 0-5 which will be recognized by using z+48(48=0 in ascii)*/
  
  
EEPROM.write(z, z+49);/*we use z+49(49=1 in ascii), so that we start our
password from 1 instead of 0*/

  for(int z=0;z<4;z++)

    oldpass[z]=EEPROM.read(z);

} 



/*=============================================================*/

//loop

void loop()

{
  
  
     
  
  //get the key pressed on the keypad
  
  char keys = myKeymap.getKey();
  

  //fill the pass with the keys pressed
  if (keys)

  {

    pass[i++]=keys;

    lcd.print(keys);

      }
  
  
  
  //start button
  if(keys=='*'){
    i=0;
 lcd.clear();
    

  lcd.print("Welcome");

  lcd.setCursor(0,1);//second line on lcd

  lcd.print("Admin Access");

  delay(500);

  lcd.clear();

  lcd.print("Enter Password");

  lcd.setCursor(0,1);

  password();
    
    
  }
  
  
  
  
  //delete key
  if(keys=='B'){
    
    i=0;
   lcd.clear();

  lcd.print("Enter Password");

  lcd.setCursor(0,1);

 password();
  }
  
  
  
  //change pass
  if(keys=='#')
    changepassword();
  
  
  /*==========================================================*/
  
  
  
  
  
  //correct pass
  if(i==4)

  {
  
/* read password  on eeprom,compares the ascii values of both
passwords */
    for(int z=0;z<4;z++)

      oldpass[z]=EEPROM.read(z);

    if(!(strncmp(pass, oldpass,4)))

    {

      lcd.clear();

      lcd.print("correct password");

      
      lcd.setCursor(0,1);

      lcd.print("access granted");
      
      delay(500); 
      
      digitalWrite(green,HIGH);
      digitalWrite(red,LOW);
      
     
      for(int s=90;s<=180;s++){
        servo.write(s);
      }
      
      lcd.clear();
      lcd.print("door open");
      
      delay(500);
      
      lcd.clear();
      
      
    }
    
    

    //incorrect pass
    else

    {
      digitalWrite(red,HIGH);
      digitalWrite(green,LOW);

      lcd.clear();

      lcd.print("incorrect");
      

      
     
      
             tone(piezo,1000,100);
     tone(piezo,1000,100);
delay(500);

      lcd.clear();
      

      lcd.print("# to Change");
      
      lcd.setCursor(0,1);

      lcd.print("or try again");

      delay(500);
      
     

      lcd.clear();

      lcd.print("Enter Password");

      lcd.setCursor(0,1);

      i=0;



    }
    
    

  }

}


/*===========================================================*/



//function for password reset
void changepassword()

{

  int z=0;

  lcd.clear();

  
  lcd.print("Current Password");//prompt the user to enter current(oldpass) password
  

  lcd.setCursor(0,1);

  while(z<4)

  {

    char key=myKeymap.getKey();

    if(key)

    {

      newpass[z++]=key;

      lcd.print(key);

       

    }

    keys=0;

  }

  delay(500);



   //to check if old and new pass match
  if((strncmp(newpass, oldpass, 4)))

  {

    lcd.clear();
    
    //if password old pass incorrect, try again

    lcd.print("incorrect pass");

    lcd.setCursor(0,1);

    lcd.print("Try Again");

    delay(500);

  }

  else

  {
  //if the current entered password is correct prompt user to enter new pass
    
    z=0;
    lcd.clear();

    lcd.print("New Password:");

    lcd.setCursor(0,1);

    while(z<4)

    {

      char key=myKeymap.getKey();

      if(key)

      {

        oldpass[z]=key;

        lcd.print(key);

        EEPROM.write(z,key);

        z++;

        

      }

    }
    
    lcd.clear();
 
    lcd.print("password Changed");

    delay(500);

  }

  lcd.clear();

  lcd.print("Enter Password");

  lcd.setCursor(0,1);

  i=0;
  

}




