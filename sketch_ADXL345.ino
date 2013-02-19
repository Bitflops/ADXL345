//Sketch reading ADXL345 accelerometer measures and outputs them over the serial port
//Serial output can be read into processing to display graphs of the acceleration in x, y, and z direction
//The wiring of the Sensor on Arduino is as following: 
//SCL to pin A5
//SDA to pin A4
//GND to GND
//VCC to 3.3V

//This program is free software: you can redistribute it and/or modify
//it under the terms of the version 3 GNU General Public License as
//published by the Free Software Foundation.

//Put together by Bitflops on Feb-2013

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345.h>

Adafruit_ADXL345 accel = Adafruit_ADXL345(12345);

#define DEVICE (0x53) // Device address as specified in data sheet

byte _buff[6];

char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;  //Data Format Register
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1

String gRange; 

int sensitivity; // Sensitivity factor to convert raw values in g's

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600); // start serial for output. Make sure you set your Serial Monitor to the same!
  Serial.print("init");

  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeTo(DATA_FORMAT, 0x0A); //Write 0x00 for 2G, 0x01 for 4G, 0x0A for 8G,0x0B for 16G
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeTo(POWER_CTL, 0x08);
  //displaySensorDetails();
  //displayRange();
  //displayDataRate();
}

void loop()
{
  readAccel(); // read the x/y/z tilt
  delay(10); // read every 0,01 seconds for fast display in Processing
}

void readAccel() {
  uint8_t howManyBytesToRead = 6;
  readFrom(DATAX0, howManyBytesToRead, _buff); //read the acceleration data from the ADXL345

  // each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  float x = (((int)_buff[1]) << 8) | _buff[0];
  float y = (((int)_buff[3]) << 8) | _buff[2];
  float z = (((int)_buff[5]) << 8) | _buff[4];
  Serial.print("DATA, ");
  Serial.print("x: ");
  Serial.print( x/ sensitivity );
  Serial.print(" y: ");
  Serial.print( y / sensitivity);
  Serial.print(" z: ");
  Serial.print( z / sensitivity); 

  switch (accel.getRange()) {
  case 0:
    gRange = "+-2g";
    sensitivity = 256;
    break;
  case 1:
    gRange = "+-4g";
    sensitivity = 132;
    break;
  case 2:
    gRange = "+-8g";
    sensitivity = 256;
    break;
  case 3:
    gRange = "+/-16g";
    sensitivity = 256;
    break;
  default: 
    gRange = "n/a";
  }
  Serial.print(" Range: ");
  Serial.println(gRange);
}

void writeTo(byte address, byte val) {
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.write(address); // send register address
  Wire.write(val); // send value to write
  Wire.endTransmission(); // end transmission
}

// Reads num bytes starting from address register on device in to _buff array
void readFrom(byte address, int num, byte _buff[]) {
  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.write(address); // sends address to read from
  Wire.endTransmission(); // end transmission

    Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.requestFrom(DEVICE, num); // request 6 bytes from device

  int i = 0;
  while(Wire.available()) // device may send less than requested (abnormal)
  {
    _buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); // end transmission
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution);Serial.println(" m/s^2");         
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 

  switch(accel.getRange())
  {
  case ADXL345_RANGE_16_G:
    Serial.print  ("16 "); 
    break;
  case ADXL345_RANGE_8_G:
    Serial.print  ("8 "); 
    break;
  case ADXL345_RANGE_4_G:
    Serial.print  ("4 "); 
    break;
  case ADXL345_RANGE_2_G:
    Serial.print  ("2 "); 
    break;
  default:
    Serial.print  ("?? "); 
    break;
  }  
  Serial.println(" g");  
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 

  switch(accel.getDataRate())
  {
  case ADXL345_DATARATE_3200_HZ:
    Serial.print  ("3200 "); 
    break;
  case ADXL345_DATARATE_1600_HZ:
    Serial.print  ("1600 "); 
    break;
  case ADXL345_DATARATE_800_HZ:
    Serial.print  ("800 "); 
    break;
  case ADXL345_DATARATE_400_HZ:
    Serial.print  ("400 "); 
    break;
  case ADXL345_DATARATE_200_HZ:
    Serial.print  ("200 "); 
    break;
  case ADXL345_DATARATE_100_HZ:
    Serial.print  ("100 "); 
    break;
  case ADXL345_DATARATE_50_HZ:
    Serial.print  ("50 "); 
    break;
  case ADXL345_DATARATE_25_HZ:
    Serial.print  ("25 "); 
    break;
  case ADXL345_DATARATE_12_5_HZ:
    Serial.print  ("12.5 "); 
    break;
  case ADXL345_DATARATE_6_25HZ:
    Serial.print  ("6.25 "); 
    break;
  case ADXL345_DATARATE_3_13_HZ:
    Serial.print  ("3.13 "); 
    break;
  case ADXL345_DATARATE_1_56_HZ:
    Serial.print  ("1.56 "); 
    break;
  case ADXL345_DATARATE_0_78_HZ:
    Serial.print  ("0.78 "); 
    break;
  case ADXL345_DATARATE_0_39_HZ:
    Serial.print  ("0.39 "); 
    break;
  case ADXL345_DATARATE_0_20_HZ:
    Serial.print  ("0.20 "); 
    break;
  case ADXL345_DATARATE_0_10_HZ:
    Serial.print  ("0.10 "); 
    break;
  default:
    Serial.print  ("???? "); 
    break;
  }  
  Serial.println(" Hz");
  Serial.println();  
}

