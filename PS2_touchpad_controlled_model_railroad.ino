/*
 * Arduino code to control a model railroad layout using a PS/2 touchpad.
 * 
 * Using a tochpad, three turnouts and one track power can be controlled.
 * The code can be modified to control more than one track power at a cost of a turnout.
 * 
 * Watch the video:-https://youtu.be/CZaF_8HFH-k
 * 
 * Made by Tech Build:-https://www.youtube.com/channel/UCNy7DyfhSD9jsQEgNwETp9g?sub_confirmation=1
 * 
 * Feel free to modify the code to adapt it to your application.
 */
#include<Adafruit_MotorShield.h>
#include<ps2.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *loco = AFMS.getMotor(1);
Adafruit_DCMotor *turnoutA = AFMS.getMotor(2);
Adafruit_DCMotor *turnoutB = AFMS.getMotor(3);
Adafruit_DCMotor *turnoutC = AFMS.getMotor(4);

PS2 mouse(6, 5);//PS2 mouse(Clock, Data);

#define statLED 13//LED on pin 13 will light up whenever touchpad registers any difference in reading.

char mstat;
char mx;
char my;
int m, x, y;
int s = 0;
int sp;
int limit = 1020;//Value proportional to the maximum value of the duty cycle of PWM waves
                 //for track power(255).

void loco_run()
{
  if (sp > 0)
  {
    loco->setSpeed(sp);
    loco->run(FORWARD);
  }

  if (sp < 0)
  {
    loco->setSpeed(-sp);
    loco->run(BACKWARD);
  }

  if (sp == 0)
  {
    loco->setSpeed(sp);
    loco->run(RELEASE);
  }

}

void turnoutA_straight()
{
  turnoutA->run(FORWARD);
  delay(25);
  turnoutA->run(RELEASE);
}

void turnoutA_side()
{
  turnoutA->run(BACKWARD);
  delay(25);
  turnoutA->run(RELEASE);
}

void turnoutB_straight()
{
  turnoutB->run(FORWARD);
  delay(25);
  turnoutB->run(RELEASE);
}

void turnoutB_side()
{
  turnoutB->run(BACKWARD);
  delay(25);
  turnoutB->run(RELEASE);
}

void turnoutC_straight()
{
  turnoutC->run(FORWARD);
  delay(25);
  turnoutC->run(RELEASE);
}

void turnoutC_side()
{
  turnoutC->run(BACKWARD);
  delay(25);
  turnoutC->run(RELEASE);
}

void Read()//Function to read data from touchpad.
{
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

  m = (int)mstat;
  x = (int)mx;
  y = (int)my;
}


void setup() {
  // put your setup code here, to run once:

  AFMS.begin(960);

  pinMode(statLED, OUTPUT);

  //Initializing the PS/2 touchpad
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
  
  turnoutA->setSpeed(255);
  turnoutB->setSpeed(255);
  turnoutC->setSpeed(255);

}

void loop() {
  // put your main code here, to run repeatedly:

  Read();
  
  if (m != 8) digitalWrite(statLED, HIGH);
  else digitalWrite(statLED, LOW);

  sp = map(s, -limit, limit, -255, 255);

  if (x > 0 && y > -4 && y < 4 && s < limit) s++;

  if (x < 0 && y > -4 && y < 4 && s > -limit) s--;


  if (y > 20 && x > -4 && x < 4) turnoutA_side();

  if (y < -20 && x > -4 && x < 4) turnoutA_straight();


  if (x > 20 && y > 20) turnoutB_side();

  if (x < -20 && y < -20) turnoutB_straight();


  if (x > 20 && y < -20) turnoutC_side();

  if (x < -20 && y > 20) turnoutC_straight();

  loco_run();

  if (m == 9)
  {
    if (s > 0) for (s = s; s != 0; s--)
      {
        Read();
        if(m != 8) break;
        sp = map(s, -limit, limit, -255, 255);
        loco_run();
        delay(20);
      }

    if (s < 0) for (s = s; s != 0; s++)
      {
        Read();
        if(m != 8) break;
        sp = map(s, -limit, limit, -255, 255);
        loco_run();
        delay(20);
      }
  }


}
