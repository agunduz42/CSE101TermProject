#include <Servo.h>
#include <LiquidCrystal_I2C.h>

int trigPin1 = 3;
int echoPin1 = 4;
int trigPin2 = 5;
int echoPin2 = 6;
int trigPin3 = 7;
int echoPin3 = 8;
int trigPin4 = 9;
int echoPin4 = 10;
int buzzerPin = 11;


Servo motor1;
Servo motor2;
LiquidCrystal_I2C lcd(0x27,16,2);

int TotalParkingSlot = 4;
int FullParkingSlot;

int mesafe(int trigPin, int echoPin)
{
  int maxrange = 15;
  int minrange = 0;
  long duration, distance;

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  delay(50);

  if(distance >= maxrange || distance <= minrange)
  return 0;
  return distance;
}

void alarm()
{
  tone(buzzerPin, 1000);
  delay(700);
  noTone(buzzerPin);
}

void printWrongDirection()
{
  lcd.setCursor (0,0);
  lcd.print("Wrong  Direction"); 
  lcd.setCursor (0,1);
  lcd.print("   Turn Right   ");
}

void setup() 
{ 
  motor1.attach(1);
  motor2.attach(2);
  motor1.write(85);
  motor2.write(85);

  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.begin();  
  lcd.setCursor (0,0);
  lcd.print("  Car  Parking  ");
  lcd.setCursor (0,1);
  lcd.print("     System     ");
  delay (3000);
  lcd.clear();  
}

void loop()
{
  //ENTRANCE GATE
  if(mesafe(trigPin4,echoPin4) > 0 && mesafe(trigPin3,echoPin3) == 0 && FullParkingSlot < TotalParkingSlot)
  {
    while(mesafe(trigPin4,echoPin4) > 0)
    {
      motor2.write(0);
      if(mesafe(trigPin3,echoPin3) > 0) //1 0 ken kapı açık ;  1 1 ken kapı kapalı ; 0 1 ken kapı kapalı
      {
        while(mesafe(trigPin3,echoPin3) > 0)
        {
          delay(100);
        }
        motor2.write(90);
        FullParkingSlot += 1;
      }
      delay(80);
    }
    motor2.write(90);
  }

   else if(mesafe(trigPin4,echoPin4) > 0 && mesafe(trigPin3,echoPin3) == 0 && FullParkingSlot >= TotalParkingSlot)
   {
    
    lcd.setCursor (0,0);
    lcd.print(" Parking  Slots "); 
    lcd.setCursor (0,1);
    lcd.print("    Are Full    ");
    
    alarm();

       lcd.clear();
    }


//EXIT GATE  
  if(mesafe(trigPin1,echoPin1) > 0 && mesafe(trigPin2,echoPin2) == 0 && FullParkingSlot != 0)
  {
    while(mesafe(trigPin1,echoPin1) > 0)
    {
      motor1.write(0);
      if(mesafe(trigPin2,echoPin2) > 0)
      {
        while(mesafe(trigPin2,echoPin2) > 0)
        {
          delay(100);
        }
        motor1.write(90);
        FullParkingSlot -= 1;
      }
      delay(80);
    }
    motor1.write(90);
  }

  if(mesafe(trigPin3,echoPin3) >  0 || mesafe(trigPin2,echoPin2) > 0)
  {
    printWrongDirection();
    alarm();
    delay(750);
    lcd.clear();
  }

  {
   lcd.setCursor (0,0);
   lcd.print("Total Slot: ");
   lcd.print(TotalParkingSlot);

   lcd.setCursor (0,1);
   lcd.print("Full Slot: ");
   lcd.print(FullParkingSlot);
  }
}
 