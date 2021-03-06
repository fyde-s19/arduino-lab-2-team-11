/******************************************************

 RUN ME ON YOUR MEGA!!!

 This program sends your network and blynk token information
 to the WiFi module over serial, then should recieve characters over
 serial from the WiFi module in return.

 1) Wire Module:
 Wire according to this picture: https://www.forward.com.au/pfod/ESP8266/GPIOpins/ESP8266_01.jpg
 - Connect VCC and CH_PD to 3.3V on the arduino
 - Connect RX1 on the mega to TX on the programmer
 - Connect TX1 on the mega to RX on the programmer
 - Connect GND to GND

 2) Go to Tools -> Board and select Arduino/Genuino Mega or Mega 2560

 3) Sign up for Blynk (https://www.blynk.cc/) and install the app on your phone.
 MAKE SURE TO CREATE AND WRITE DOWN YOUR TOKEN!!!
 Then, go to Sketch -> Include Library -> Manage Libraries and search 'Blynk' and
 install the library

 4) Change the network name, password, and blynk token in the code to reflect your own

 5) Finish writing the serial recieve in the 'void loop()'
 It needs to:
 - Continuously look for serial input on Serial 1
 - Take any character recieved on Serial 1 and print
 the data out through the main Serial

 6) Open a serial monitor by going to
 Tools -> Serial Monitor and set to 9600 baud once open to see output
 and run!

 *****************************/
#include <SimpleTimer.h>


#define RDY     2       // Input from 8266
#define ACK     3       // Output to 8266
#define ESP_RST 4       // Reset to ESP
#define led     13

int     Pin_Number  = 255;
int     Pin_Integer = 0;
float   Pin_Float   = 0.0;

//char    ssid[32]        = "iPhoneXR";
//char    pass[32]        = "bullybully";
char    ssid[32]        = "EE-IOT-Platform-02";
char    pass[32]        = "dUQQE?&W44x7";

char    auth[256]  = "0ef47a7eee99415c88c8c2f2f2e374b9";   // For FYDE projects only


// **********************************
void DebugPrint(void)
{

    // Serial.print(Pin_Number);
    // Serial.print(",");
    // Serial.print(Pin_Integer);
    // Serial.print(",");
    // Serial.println(Pin_Float);
}

// =====================================================================
// This routine sets up the Wifi connection
// =====================================================================

void Wifi_Setup(void)
{
    // Serial.println("In WiFI_Setup routine");

    char inchar = '0';      // Assign NULL value

    while ((digitalRead(RDY) != 1))

    {
        // Wait for ESP8266 indicate it is ready for programming data
        // Serial.println("Waiting for data from ESP");
        delay(2000);
    }

    digitalWrite(ACK, HIGH);    // Acknowledge that RDY went HIGH

    Serial.print(auth);        // This data is shipped in CSV format
    Serial.print(",");
    Serial.print(ssid);
    Serial.print(",");
    Serial.print(pass);
    Serial.print(",");         // NOTE: Dangling comma is needed for the 8266 code
    Serial.print("\n");

    while ((digitalRead(RDY) != 0))
    {

        if ((Serial.available() > 0))
        {

            inchar = Serial.read();  // assigns one byte as serial.read()'s only input one byte at a time
            // Serial.print(inchar);
        }
        else if (Serial.available() == 0)
        {

        }
    }
    digitalWrite(ACK, LOW);     // Acknowledge that RDY went LOW

    // Serial.println("Exiting WiFI_Setup routine");

}

// =====================================================================
// This routine reads the serial input from the ESP8266 to the MEGA
// =====================================================================

void ESP8266_to_Mega(void)
{

    while (Serial.available() > 0)
    {
        Serial.println("In serial input");
        // look for the next valid integer in the incoming serial stream:

        Pin_Number  = Serial.parseInt();
        Pin_Integer = Serial.parseInt();
        Pin_Float   = Serial.parseFloat();

        // Look for the newline.
        if (Serial.read() == '\n')
        {
            //DebugPrint();
        }
        Parser();     // Go to parsing routine
    }

}


int     Val             = 50;
int     led_val         = 4;
int     brightness      = 255;
int     sensorPin       = A0;    // select the input pin for the potentiometer
float   sensorValueOld  = 0.0;  // variable to store the value coming from the sensor
float   sensorValueNew  = 0.0;

// ===================   PARSER   ==================================================

void Parser(void)
{
    Serial.println("In the parser");

    if ((Pin_Number == 1) && (Pin_Integer == 1))
    {

        DebugPrint();
        digitalWrite(led, HIGH);

    }

    if ((Pin_Number == 1) && (Pin_Integer == 0))
    {
        DebugPrint();
        digitalWrite(led, LOW);
    }

    if (Pin_Number == 5)
    {
        sensorValueOld = Pin_Float;

        DebugPrint();
    }

}

// **********************************
void ReadSensors(void)
{

    sensorValueNew = analogRead(sensorPin);

    if (abs(sensorValueNew - (sensorValueOld)) > 0)
    {
        Serial.print(51);
        Serial.print(",");
        Serial.print(0);
        Serial.print(",");
        Serial.print((sensorValueNew / 1023) * 5);
        Serial.print("\n");

        // Serial.print(51);
        // Serial.print(",");
        // Serial.print(0);
        // Serial.print(",");
        // Serial.print((sensorValueNew / 1023) * 5);
        // Serial.print("\n");
        sensorValueOld = sensorValueNew;

        Serial.print("Analog pin value = ");
        Serial.print((sensorValueNew / 1023) * 5);
        Serial.print("V \n");
    }
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// THIS IS THE MAIN SETUP CODE. IT IS ONLY RUN ONCE
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void setup()
{
    pinMode(led, OUTPUT);
    pinMode(RDY, INPUT_PULLUP);     // RDY Signal from 8266 (default is HIGH)
    pinMode(ACK, OUTPUT);           // ACK Signal to 8266
    pinMode(ESP_RST, OUTPUT);
    digitalWrite(ACK, LOW);         // Negate RDY signal to 8266

    digitalWrite(ESP_RST, LOW);     // Assert reset to ESP8266

    // ----------------------------------------------------------------------------
    // Start debug serial port at 9600 bps and wait for port to open:
    //
    Serial.begin(9600);
    while (!Serial) {  }            // wait for serial port to connect. Needed for native USB port only

    // ----------------------------------------------------------------------------
    // Start debug serial port at 9600 bps and wait for port to open:
    //
    Serial.begin(9600);
    while (!Serial) {  }           // wait for serial port to connect. Needed for native USB port only
    digitalWrite(ESP_RST, HIGH);    // Negate reset to ESP8266

    delay(1000);                    // Wait for ESP8266 to come out of reset

    //Wifi_Setup();                   // Send the WiFi login data to the ESP8266

    digitalWrite(led, HIGH);        // Indicate we are alive and well
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// THIS IS THE MAIN LOOP. IT RUNS CONTINOUSLY
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void loop()
{

    //ESP8266_to_Mega();
    //delay(400);
    //ReadSensors();
    Serial.println("Hello world");
    delay(300);
}
