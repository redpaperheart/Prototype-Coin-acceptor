


const int coinIntPin = 0;          //interruptPin 0 is digital pin 2
volatile boolean newCoin = false;                  
volatile int pulseCount;           //counts pulse for 
volatile int timeOut = 0;          //counts timeout after last coin inserted
String coinType;
int coinTypeInt;

void setup(){
    Serial.begin(9600);                 
    attachInterrupt(coinIntPin, coinInserted, RISING);   
    establishContact();  
}

void coinInserted(){
    newCoin = true; 
    pulseCount++;
    timeOut = 0;
}

void loop(){
    if (Serial.available() > 0) {
         if (newCoin == true){
            //Serial.println(timeOut);
            if(pulseCount == 3 || timeOut>25){           //timeOut insures that the pulses have finished before the coin Type is determined
              coinSwitch();
            }
            timeOut++;
            delay(5);
        }  
    }
}

void coinSwitch(){
    switch (pulseCount) {                                //pulseCount can be anything from 1 to 50, programmed in the Coin Acceptor. There can be up to 4 cases. 
        case 1:
            //coinType = "nickel";
            //Serial.println("Coin Type: " + coinType);
            coinTypeInt = 1;
            Serial.println(coinTypeInt,DEC);            
            pulseCount = 0;
            newCoin = false;
            break;
        case 2:
            //coinType = "dime";
            //Serial.println("Coin Type: " + coinType);
            coinTypeInt = 2;
            Serial.println(coinTypeInt,DEC);            
            pulseCount = 0;
            newCoin = false;
            break;
        case 3:
            //coinType = "quarter";
            //Serial.println("Coin Type: " + coinType);
            coinTypeInt = 3;
            Serial.println(coinTypeInt,DEC);
            pulseCount = 0;
            newCoin = false;
            break; 
    }
}

void establishContact() {                                //serial handshake to insure cinder connects to the arduino
    while (Serial.available() <= 0) {
        Serial.print('*');   // send a capital A
        delay(300);  
      }
}

