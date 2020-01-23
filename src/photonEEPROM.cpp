/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#line 1 "/Users/abdulhannanmustajab/Desktop/Projects/IoT/Particle/tempLogger/photon-EEPROM/photonEEPROM/src/photonEEPROM.ino"
#include "RFID.h"

/* Define the pins used for the SS (SDA) and RST (reset) pins for BOTH hardware and software SPI */
/* Change as required */
void setup();
void loop();
int scanCard(String Command);
#line 5 "/Users/abdulhannanmustajab/Desktop/Projects/IoT/Particle/tempLogger/photon-EEPROM/photonEEPROM/src/photonEEPROM.ino"
#define SS_PIN A2 // Same pin used as hardware SPI (SS)
#define RST_PIN D2

/* Define the pins used for the DATA OUT (MOSI), DATA IN (MISO) and CLOCK (SCK) pins for SOFTWARE SPI ONLY */
/* Change as required and may be same as hardware SPI as listed in comments */
#define MOSI_PIN A5 // hardware SPI: A5
#define MISO_PIN A4 //     "     " : A4
#define SCK_PIN A3  //     "     " : A3

/* Create an instance of the RFID library */
#if defined(_USE_SOFT_SPI_)
RFID RC522(SS_PIN, RST_PIN, MOSI_PIN, MISO_PIN, SCK_PIN); // Software SPI
#else
RFID RC522(SS_PIN, RST_PIN); // Hardware SPI
#endif

char cardID[16];
// Variables related to program

void setup()
{
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  Serial.begin(9600);

#if !defined(_USE_SOFT_SPI_)
  /* Enable the HW SPI interface */
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();
#endif

  /* Initialise the RFID reader */
  RC522.init();
  Particle.variable("cardID", cardID);
  Particle.function("scanCard", scanCard);
}

void loop()
{
}

int scanCard(String Command)
{
  if (Command == "1")
  {
    digitalWrite(D5, HIGH);
    Serial.println("TURNING RED ON:");
    // Red Light means card is ready to be scanned.
    /* Has a card been detected? */

    if (RC522.isCard())
    {
      digitalWrite(D5, LOW);
      Serial.println("ENTERED LOOP:");

      /* If so then get its serial number */
      RC522.readCardSerial();
      Serial.println("GREEEN ON:");

      Serial.println("Card detected:");

      /* Output the serial number to the UART */

      snprintf(cardID, sizeof(cardID), "%02X-%02X-%02X-%02X", RC522.serNum[0], RC522.serNum[1], RC522.serNum[2], RC522.serNum[3]);

      Serial.print(cardID);
      Serial.println();
      digitalWrite(D4, HIGH);
    }
    else
    {
      delay(1000);
      scanCard("1");
    }
  }
}