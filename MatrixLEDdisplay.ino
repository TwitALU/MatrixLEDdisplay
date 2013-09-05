/***********************************************
File Name:	MatrixLEDdisplay.ino
Version:	1.0
Date:		15/08/2013
Author:		Jaimesh Mistry
Purpose:	Display OPCODE and countdown for
the Twitalu.
***********************************************/

// Includes
#include <Arduino.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Wire.h>


//Defines
#define CLK 8 
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define ledPin 13
#define SLAVE_ADDRESS 0x10

// Globals
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
int currentMillis;
int serialData = 0;
int timer2_counter = 0;
int countdown_counter = 0;
int countdown_timer = 0;
int recieve_data_count = 0;
bool temp = true;
byte i2cData[3];
int data_index = 0;

char twit_check_str[]   = "Checking Twitter...";
char twit_send_str[] = "Tweeting...";
char wait_str[] = "Waiting...";
int textX   = matrix.width();
int twit_check_textMin = sizeof(twit_check_str) * -6;
int twit_send_textMin = sizeof(twit_send_str) * -6;
int wait_textMin = sizeof(wait_str) * -6;

void setup() {
	// Matrix display initialisation
	matrix.begin();

	// I2C intilisation
	pinMode(13, OUTPUT);
	Serial.begin(9600);			// start serial for output
	// initialize i2c as slave
	Wire.begin(SLAVE_ADDRESS);

	// define callbacks for i2c communication
	Wire.onReceive(receiveData);
	Wire.onRequest(sendData);

	// setup hearbeat LED
	pinMode(ledPin, OUTPUT);

	currentMillis = millis();

	// Display startup screen
	display_twitalu_logo();

	// Print to serial to indiate end of setup
	Serial.println("Ready!");
}

void display_clear(){
	matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void display_twitalu_logo(){
	matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 4, matrix.Color333(4, 4, 4)); matrix.drawPixel(1, 5, matrix.Color333(3, 3, 3)); matrix.drawPixel(1, 6, matrix.Color333(3, 3, 3)); matrix.drawPixel(1, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(1, 8, matrix.Color333(3, 3, 3)); matrix.drawPixel(1, 9, matrix.Color333(4, 4, 4)); matrix.drawPixel(1, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 4, matrix.Color333(2, 2, 2)); matrix.drawPixel(2, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(2, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(2, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(2, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(2, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(2, 10, matrix.Color333(3, 3, 3)); matrix.drawPixel(2, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(3, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(3, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(3, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(3, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 6, matrix.Color333(2, 2, 2)); matrix.drawPixel(4, 7, matrix.Color333(5, 5, 5)); matrix.drawPixel(4, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 9, matrix.Color333(4, 4, 4)); matrix.drawPixel(4, 10, matrix.Color333(3, 3, 3)); matrix.drawPixel(4, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(5, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(5, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(5, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(5, 10, matrix.Color333(6, 6, 6)); matrix.drawPixel(5, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 6, matrix.Color333(4, 4, 4)); matrix.drawPixel(6, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(6, 8, matrix.Color333(3, 3, 3)); matrix.drawPixel(6, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(6, 10, matrix.Color333(1, 1, 1)); matrix.drawPixel(6, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(7, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(7, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 10, matrix.Color333(2, 2, 2)); matrix.drawPixel(8, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 6, matrix.Color333(4, 4, 4)); matrix.drawPixel(9, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(9, 8, matrix.Color333(3, 3, 3)); matrix.drawPixel(9, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(9, 10, matrix.Color333(1, 1, 1)); matrix.drawPixel(9, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(10, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 6, matrix.Color333(1, 1, 1)); matrix.drawPixel(11, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 10, matrix.Color333(3, 3, 3)); matrix.drawPixel(11, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 6, matrix.Color333(4, 4, 4)); matrix.drawPixel(12, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(12, 8, matrix.Color333(3, 3, 3)); matrix.drawPixel(12, 9, matrix.Color333(4, 4, 4)); matrix.drawPixel(12, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 4, matrix.Color333(2, 2, 2)); matrix.drawPixel(13, 5, matrix.Color333(1, 1, 1)); matrix.drawPixel(13, 6, matrix.Color333(4, 4, 4)); matrix.drawPixel(13, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 4, matrix.Color333(4, 4, 4)); matrix.drawPixel(14, 5, matrix.Color333(4, 4, 4)); matrix.drawPixel(14, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(14, 8, matrix.Color333(3, 3, 3)); matrix.drawPixel(14, 9, matrix.Color333(3, 3, 3)); matrix.drawPixel(14, 10, matrix.Color333(4, 4, 4)); matrix.drawPixel(14, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 4, matrix.Color333(2, 2, 2)); matrix.drawPixel(15, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(15, 10, matrix.Color333(6, 6, 6)); matrix.drawPixel(15, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 4, matrix.Color333(5, 5, 5)); matrix.drawPixel(16, 5, matrix.Color333(3, 3, 3)); matrix.drawPixel(16, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 7, matrix.Color333(2, 2, 2)); matrix.drawPixel(16, 8, matrix.Color333(3, 3, 3)); matrix.drawPixel(16, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 10, matrix.Color333(1, 1, 1)); matrix.drawPixel(16, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 7, matrix.Color333(3, 3, 3)); matrix.drawPixel(17, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 9, matrix.Color333(2, 2, 2)); matrix.drawPixel(17, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 6, matrix.Color333(6, 6, 6)); matrix.drawPixel(18, 7, matrix.Color333(6, 6, 6)); matrix.drawPixel(18, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(18, 9, matrix.Color333(6, 6, 6)); matrix.drawPixel(18, 10, matrix.Color333(6, 6, 6)); matrix.drawPixel(18, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 6, matrix.Color333(4, 4, 4)); matrix.drawPixel(19, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 10, matrix.Color333(4, 4, 4)); matrix.drawPixel(19, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 7, matrix.Color333(1, 1, 1)); matrix.drawPixel(20, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(20, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(20, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 7, matrix.Color333(1, 1, 1)); matrix.drawPixel(21, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(21, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(21, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 6, matrix.Color333(4, 4, 4)); matrix.drawPixel(22, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(22, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(22, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(22, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(22, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 4, matrix.Color333(6, 6, 6)); matrix.drawPixel(23, 5, matrix.Color333(5, 5, 5)); matrix.drawPixel(23, 6, matrix.Color333(5, 5, 5)); matrix.drawPixel(23, 7, matrix.Color333(4, 4, 4)); matrix.drawPixel(23, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(23, 9, matrix.Color333(5, 5, 5)); matrix.drawPixel(23, 10, matrix.Color333(4, 4, 4)); matrix.drawPixel(23, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(24, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 4, matrix.Color333(6, 6, 6)); matrix.drawPixel(25, 5, matrix.Color333(5, 5, 5)); matrix.drawPixel(25, 6, matrix.Color333(5, 5, 5)); matrix.drawPixel(25, 7, matrix.Color333(5, 5, 5)); matrix.drawPixel(25, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(25, 9, matrix.Color333(4, 4, 4)); matrix.drawPixel(25, 10, matrix.Color333(6, 6, 6)); matrix.drawPixel(25, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(26, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(26, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(26, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(26, 10, matrix.Color333(4, 4, 4)); matrix.drawPixel(26, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 6, matrix.Color333(5, 5, 5)); matrix.drawPixel(27, 7, matrix.Color333(5, 5, 5)); matrix.drawPixel(27, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(27, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(27, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(27, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 6, matrix.Color333(5, 5, 5)); matrix.drawPixel(28, 7, matrix.Color333(5, 5, 5)); matrix.drawPixel(28, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(28, 9, matrix.Color333(1, 1, 1)); matrix.drawPixel(28, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(28, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(29, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(29, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(29, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(29, 10, matrix.Color333(4, 4, 4)); matrix.drawPixel(29, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 6, matrix.Color333(5, 5, 5)); matrix.drawPixel(30, 7, matrix.Color333(5, 5, 5)); matrix.drawPixel(30, 8, matrix.Color333(5, 5, 5)); matrix.drawPixel(30, 9, matrix.Color333(6, 6, 6)); matrix.drawPixel(30, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 15, matrix.Color333(7, 7, 7)); 
}

//void display_twitter_logo(){
//	matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(0, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(1, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(2, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(3, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(4, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(5, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(6, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 2, matrix.Color333(6, 6, 6)); matrix.drawPixel(7, 3, matrix.Color333(5, 5, 5)); matrix.drawPixel(7, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 6, matrix.Color333(6, 6, 6)); matrix.drawPixel(7, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 13, matrix.Color333(6, 6, 6)); matrix.drawPixel(7, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(7, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 2, matrix.Color333(1, 1, 1)); matrix.drawPixel(8, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 5, matrix.Color333(3, 3, 3)); matrix.drawPixel(8, 6, matrix.Color333(1, 1, 1)); matrix.drawPixel(8, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(8, 8, matrix.Color333(6, 6, 6)); matrix.drawPixel(8, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(8, 13, matrix.Color333(3, 3, 3)); matrix.drawPixel(8, 14, matrix.Color333(4, 4, 4)); matrix.drawPixel(8, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 2, matrix.Color333(6, 6, 6)); matrix.drawPixel(9, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(9, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(9, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(9, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(9, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(9, 8, matrix.Color333(1, 1, 1)); matrix.drawPixel(9, 9, matrix.Color333(3, 3, 3)); matrix.drawPixel(9, 10, matrix.Color333(3, 3, 3)); matrix.drawPixel(9, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(9, 13, matrix.Color333(1, 1, 1)); matrix.drawPixel(9, 14, matrix.Color333(2, 2, 2)); matrix.drawPixel(9, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 3, matrix.Color333(5, 5, 5)); matrix.drawPixel(10, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 11, matrix.Color333(4, 4, 4)); matrix.drawPixel(10, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(10, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 14, matrix.Color333(0, 0, 0)); matrix.drawPixel(10, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(11, 4, matrix.Color333(2, 2, 2)); matrix.drawPixel(11, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 11, matrix.Color333(2, 2, 2)); matrix.drawPixel(11, 12, matrix.Color333(4, 4, 4)); matrix.drawPixel(11, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 14, matrix.Color333(0, 0, 0)); matrix.drawPixel(11, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(12, 4, matrix.Color333(5, 5, 5)); matrix.drawPixel(12, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 14, matrix.Color333(0, 0, 0)); matrix.drawPixel(12, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(13, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 14, matrix.Color333(0, 0, 0)); matrix.drawPixel(13, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(14, 5, matrix.Color333(2, 2, 2)); matrix.drawPixel(14, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(14, 14, matrix.Color333(1, 1, 1)); matrix.drawPixel(14, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(15, 3, matrix.Color333(3, 3, 3)); matrix.drawPixel(15, 4, matrix.Color333(2, 2, 2)); matrix.drawPixel(15, 5, matrix.Color333(1, 1, 1)); matrix.drawPixel(15, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(15, 14, matrix.Color333(3, 3, 3)); matrix.drawPixel(15, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(16, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 13, matrix.Color333(0, 0, 0)); matrix.drawPixel(16, 14, matrix.Color333(6, 6, 6)); matrix.drawPixel(16, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 1, matrix.Color333(2, 2, 2)); matrix.drawPixel(17, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(17, 13, matrix.Color333(2, 2, 2)); matrix.drawPixel(17, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(17, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 1, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 12, matrix.Color333(0, 0, 0)); matrix.drawPixel(18, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(18, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 1, matrix.Color333(1, 1, 1)); matrix.drawPixel(19, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 10, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 11, matrix.Color333(0, 0, 0)); matrix.drawPixel(19, 12, matrix.Color333(6, 6, 6)); matrix.drawPixel(19, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(19, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 1, matrix.Color333(4, 4, 4)); matrix.drawPixel(20, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 8, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 9, matrix.Color333(0, 0, 0)); matrix.drawPixel(20, 10, matrix.Color333(1, 1, 1)); matrix.drawPixel(20, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(20, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 4, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 5, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 6, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 7, matrix.Color333(0, 0, 0)); matrix.drawPixel(21, 8, matrix.Color333(2, 2, 2)); matrix.drawPixel(21, 9, matrix.Color333(5, 5, 5)); matrix.drawPixel(21, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(21, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 1, matrix.Color333(5, 5, 5)); matrix.drawPixel(22, 2, matrix.Color333(0, 0, 0)); matrix.drawPixel(22, 3, matrix.Color333(0, 0, 0)); matrix.drawPixel(22, 4, matrix.Color333(3, 3, 3)); matrix.drawPixel(22, 5, matrix.Color333(6, 6, 6)); matrix.drawPixel(22, 6, matrix.Color333(6, 6, 6)); matrix.drawPixel(22, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(22, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 1, matrix.Color333(6, 6, 6)); matrix.drawPixel(23, 2, matrix.Color333(5, 5, 5)); matrix.drawPixel(23, 3, matrix.Color333(5, 5, 5)); matrix.drawPixel(23, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(23, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(24, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(25, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(26, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(27, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(28, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(29, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(30, 15, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 0, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 1, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 2, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 3, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 4, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 5, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 6, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 7, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 8, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 9, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 10, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 11, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 12, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 13, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 14, matrix.Color333(7, 7, 7)); matrix.drawPixel(31, 15, matrix.Color333(7, 7, 7)); 
//}

void display_string(String in_string){
	// Shorten the string to three characters if nessecary
	if (in_string.length() > 3){
		String new_string = "AAA";

		new_string.setCharAt(0, in_string.charAt(0));
		new_string.setCharAt(1, in_string.charAt(1));
		new_string.setCharAt(2, in_string.charAt(2));

		in_string = new_string;
	}

	// Clear the display before writing
	display_clear();

	// Write 1 to 3 characters centrally on the display
	if (in_string.length() == 3){//three characters
		char char0 = in_string.charAt(0);
		char char1 = in_string.charAt(1);
		char char2 = in_string.charAt(2);

		matrix.setTextSize(2);    // size 1 == 8 pixels high
		matrix.setTextColor(matrix.Color333(1,1,1));
		matrix.setCursor(0, 1);
		matrix.print(char0);
		matrix.setCursor(11, 1);
		matrix.print(char1);
		matrix.setCursor(22, 1);
		matrix.print(char2);
	} else if (in_string.length() == 2){ //two characters
		char char0 = in_string.charAt(0);
		char char1 = in_string.charAt(1);

		matrix.setTextSize(2);    // size 1 == 8 pixels high
		matrix.setTextColor(matrix.Color333(1,1,1));
		matrix.setCursor(5, 1);
		matrix.print(char0);
		matrix.setCursor(16, 1);
		matrix.print(char1);
	} else if (in_string.length() == 1){ //one character
		char char0 = in_string.charAt(0);

		matrix.setTextSize(2);    // size 1 == 8 pixels high
		matrix.setTextColor(matrix.Color333(1,1,1));
		matrix.setCursor(11, 1);
		matrix.print(char0);
	}



}

// callback for received data
// It must recieve 2 bytes the 2nd byte will be ignored if it's not needed
void receiveData(int byteCount){
	while(Wire.available()) {
		byte temp = Wire.read();

		i2cData[data_index] = temp;
		data_index++;

		Serial.println(data_index);

		if(data_index == 2){
			Serial.println("Command recieved");
			i2cData[2] = 0x01;
			data_index = 0;
			Serial.println("ready for next command");
		}
	}
}

// callback for sending data
void sendData(){
	Wire.write(i2cData[0]);
	Wire.write(i2cData[1]);
	Wire.write(i2cData[2]);
}

void loop(){
	if(i2cData[2] == 0x00){
		digitalWrite(ledPin, LOW);
	}
	if(i2cData[2] == 0x01 | i2cData[2] == 0x02){
		digitalWrite(ledPin, HIGH);
		//Serial.println("command loop");
		// decode and process data
		switch (i2cData[0]){
		case 0x00:	// clear display
			Serial.println("clear display");	
			display_clear();
			i2cData[2] = 0x00;
			break;	

		case 0x01:	// display add
			Serial.println("display add");		
			display_string("ADD");
			i2cData[2] = 0x00;
			break;	

		case 0x02:	// display sub
			Serial.println("display sub");		
			display_string("SUB");
			i2cData[2] = 0x00;
			break;

		case 0x03:	// display mul
			Serial.println("display mul");		
			display_string("MUL");
			i2cData[2] = 0x00;
			break;

		case 0x04:	// display div
			Serial.println("display div");		
			display_string("DIV");
			i2cData[2] = 0x00;
			break;

		case 0x05:	// display and
			Serial.println("display and");		
			display_string("AND");
			i2cData[2] = 0x00;
			break;	

		case 0x06:	// display or
			Serial.println("display or");		
			display_string("OR");
			i2cData[2] = 0x00;
			break;	

		case 0x07:	// display xor
			Serial.println("display xor");		
			display_string("XOR");
			i2cData[2] = 0x00;
			break;	

		case 0x08:	// display ror
			Serial.println("display ror");		
			display_string("ROR");
			i2cData[2] = 0x00;
			break;	

		case 0x09:	// display rol
			Serial.println("display rol");		
			display_string("ROL");
			i2cData[2] = 0x00;
			break;

		case 0x0A: // display checking twitter
				Serial.println("display checking twitter");
				if(i2cData[2] == 0x01){
					matrix.setTextWrap(false); // Allow text to run off right edge
					matrix.setTextSize(1);
					matrix.fillScreen(0);
					i2cData[2] = 0x02;
				}
				if(i2cData[2] == 0x02){
					matrix.fillScreen(0);
					matrix.setCursor(textX, 4);
					matrix.print(twit_check_str);

					if((--textX) < twit_check_textMin){
						textX = matrix.width();
						display_twitalu_logo();
						i2cData[2]=0x00;
					}

					matrix.swapBuffers(false);
				}
				break;

		case 0x0B: // display sending tweets
				Serial.println("display sending tweets");
				if(i2cData[2] == 0x01){
					matrix.setTextWrap(false); // Allow text to run off right edge
					matrix.setTextSize(1);
					matrix.fillScreen(0);
					i2cData[2] = 0x02;
				}
				if(i2cData[2] == 0x02){
					matrix.fillScreen(0);
					matrix.setCursor(textX, 4);
					matrix.print(twit_send_str);

					if((--textX) < twit_send_textMin){
						textX = matrix.width();
						display_twitalu_logo();
						i2cData[2]=0x00;
					}

					matrix.swapBuffers(false);
				}
				break;

		case 0x0C: // display waiting
				Serial.println("display waiting");
				if(i2cData[2] == 0x01){
					matrix.setTextWrap(false); // Allow text to run off right edge
					matrix.setTextSize(1);
					matrix.fillScreen(0);
					i2cData[2] = 0x02;
				}
				if(i2cData[2] == 0x02){
					matrix.fillScreen(0);
					matrix.setCursor(textX, 4);
					matrix.print(wait_str);

					if((--textX) < wait_textMin){
						textX = matrix.width();
						display_twitalu_logo();
						i2cData[2]=0x00;
					}

					matrix.swapBuffers(false);
				}
				break;

		case 0x0D:	// start counter from data recieved
			if(i2cData[2] == 0x01){
				//Serial.println("start cnt");	
				countdown_counter = int(i2cData[1]);
				i2cData[2] = 0x02;
			}

			if (countdown_counter >= 0 && i2cData[2] == 0x02){
				currentMillis = millis();

				if ((currentMillis - countdown_timer) >= 1000){ // 1000 milliseconds
					countdown_timer = currentMillis;
					//Serial.println(currentMillis);
					//Serial.println(countdown_counter);

					display_string(String(countdown_counter, DEC));
					countdown_counter--;

					if (countdown_counter == -1){
						display_twitalu_logo();
						i2cData[2] = 0x00;
					}
				}
			}
			break;

		case 0xF0:	// display TEST
			Serial.println("display test");		
			display_string("TEST");
			i2cData[2] = 0x00;
			break;	

		case 0xF1:	// display twitalu logo
			Serial.println("twitalu logo");		
			display_twitalu_logo();
			i2cData[2] = 0x00;
			break;

		case 0xFF: // stop command
			if(i2cData[1] == 0xFF){
				Serial.println("all stop");
				countdown_counter = -1;
				display_clear();
				i2cData[2] = 0x00;
			}
			break;
			
		default:
			Serial.println("Error");
			display_clear();
			i2cData[2] = 0x00;
			break;
		}
	}
}