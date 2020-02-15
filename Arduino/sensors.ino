
static const uint8_t D2   = 4;
#define ONE_WIRE_BUS D2
struct tSensor
{
  DeviceAddress address;
  int16_t celsium;
} sensorsDt[8];

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


uint8_t  deviceCount;

void printAddress(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
} 
void senSetup()
{
  sensors.begin();
  sensors.setResolution(12);
  sensors.setWaitForConversion(true);
  Serial.println("sensors started");
  deviceCount = sensors.getDeviceCount();
  Serial.print("sensors ");
  Serial.print(deviceCount);
  Serial.println(".");
  for (int i = 0;  i < deviceCount;  i++)
  {

    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    sensors.getAddress(sensorsDt[i].address, i);
    printAddress(sensorsDt[i].address);
    Serial.println(" ");
  }
}

uint8_t senLoop(uint8_t* buf)
{
    sensors.requestTemperatures();
  Serial.println("Printing addresses...");
  for (int i = 0;  i < deviceCount;  i++)
  {
    
    sensorsDt[i].celsium = sensors.getTempCByIndex(i)*10;
    Serial.print(i+1);
    Serial.print(": t - ");
    Serial.print(sensorsDt[i].celsium);
    Serial.println(" C ");
    Serial.print(sizeof(tSensor));
    uint8_t *dt = (uint8_t*)&sensorsDt[i];
    for(uint8_t j=0;j<sizeof(tSensor);j++)
      *buf++ = *dt++;
    delay(100);
  }
  return deviceCount * sizeof(tSensor);
}
