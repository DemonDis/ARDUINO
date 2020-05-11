//Подключаем библиотеки 
#include <DHT.h>
#include <SD.h>
#include <SPI.h>

//Создаем файлы
File Light_sensor_file;
File Temperature_sensor_file;
File Soil_sensor_file;
File Rain_sensor_file;
File CO2_sensor_file;

#define DHTPIN      A2     // вывод, к которому подключается датчик
#define DHTTYPE     DHT11  // DHT 11
#define PIN_RELAY   2      // Пин для подключение реле (вентилятор)
#define PIN_RELAY1  3      // Пин для подключение реле (свет)
#define PIN_RELAY2  4      // Пин для подключение реле (вода)

DHT dht(DHTPIN, DHTTYPE);

//Объявляем переменные
int   pinD0          =  A1;   // Пин к которому подключен света
int   CS_pin         =  10;   // Пин к которому подключен SD card (цифра)
int   pinD1          =  A2;   // Пин к которому подключен датчик температуры
int   pinD2          =  A0;   // Пин к которому подключен датчик влажности
int   pinD3          =  A3;   // Пин к которому подключен датчик влажности (дождь)
float pinD4          =  A4;   // Пин к которому подключен датчик CO2 (аналог)
byte  CO2_pinD       =  8;    // Пин к которому подключен датчик CO2 (цифр)
int   thresholdValue =  500;

void setup() 
{
  pinMode (CS_pin,   OUTPUT);
  pinMode (pinD0,    INPUT);          // Установим вывод A1 как вход
  pinMode (pinD1,    INPUT);
  pinMode (pinD2,    INPUT);
  pinMode (pinD3,    INPUT);
  pinMode (pinD4,    INPUT);
  pinMode (CO2_pinD, INPUT);
  
  dht.begin();
  Serial.begin (9600);                // Задаем скорость передачи данных
  
//Объявляем реле
    pinMode      (PIN_RELAY,  OUTPUT);  // Объявляем пин реле для датчика температуры
  digitalWrite   (PIN_RELAY,  HIGH);    // Выключаем реле для датчика температуры
    pinMode      (PIN_RELAY1, OUTPUT);  // Объявляем пин реле как выход для датчика света
  digitalWrite   (PIN_RELAY1, HIGH);    // Выключаем реле для датчика света
    pinMode      (PIN_RELAY2, OUTPUT);  // Объявляем пин реле как выход для датчика CO2
  digitalWrite   (PIN_RELAY2, HIGH);    // Выключаем реле для датчика CO2
if (SD.begin())
  {
    Serial.println("SD card is initialized and it is ready to use");
  }
    else
    {
      Serial.println("SD card is not initialized");
      return;
    }
}

void loop()
{
  Light_sensor_file = SD.open("Light.txt", FILE_WRITE);
  if (Light_sensor_file) 
      {   
      Light_sensor();
    }
  else
    {
    Serial.println("error opening Light.txt");
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Temperature_sensor_file = SD.open("Temp.txt", FILE_WRITE);
  if (Temperature_sensor_file) 
      {   
      Temperature_sensor();
    }
  else
    {
    Serial.println("error opening Temp.txt");
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Soil_sensor_file = SD.open("Soil.txt", FILE_WRITE);
  if (Soil_sensor_file) 
      {   
      Soil_sensor();
    }
  else
    {
    Serial.println("error opening Soil.txt");
    }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Rain_sensor_file = SD.open("Rain.txt", FILE_WRITE);
  if (Rain_sensor_file) 
      {   
      Rain_sensor();
    }
  else
    {
    Serial.println("error opening Rain.txt");
    }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  CO2_sensor_file = SD.open("CO2.txt", FILE_WRITE);
  if (CO2_sensor_file) 
      {   
      CO2_sensor();
    }
  else
    {
    Serial.println("error opening CO2.txt");
    }
  delay(1000); // Одна секунда
 }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Light_sensor(){
 int xD0;
  xD0 = digitalRead (pinD0);
  Light_sensor_file.print     ("IN = "); 
  Light_sensor_file.println   (xD0, DEC); 
  Light_sensor_file.print     ("Sensor: ");
  Light_sensor_file.println   ("ON");
  Light_sensor_file.println   ("OFF"); 
  Serial.print    ("IN = ");
  Serial.println  (xD0, DEC);
  Serial.print    ("Sensor: ");
  
  if (xD0 == HIGH)
   {           
     Serial.println ("ON");
       Serial.println("Включем свет");
      digitalWrite(PIN_RELAY1, LOW); // Включаем реле
   }
  else
   { 
    Serial.println ("OFF");
      Serial.println("Выключаем свет");  
      digitalWrite(PIN_RELAY1, HIGH); // Отключаем реле
   }
  delay(1000); // Одна секунда
Light_sensor_file.flush();   
Light_sensor_file.close();
}

//----------------------------------------------------
void Temperature_sensor() {
  delay(250);
  float h = dht.readHumidity();
 // Считывание температуры в цельсиях
  float t = dht.readTemperature();
 // Проверка на ошибку
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
else
  { 
  Temperature_sensor_file.print("Humidity: ");
  Temperature_sensor_file.print(h);
  Temperature_sensor_file.print(" %\t");
  Temperature_sensor_file.print("Temperature: ");
  Temperature_sensor_file.print(t);
  Temperature_sensor_file.println(" *C ");
    
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  }
if (t >= 26)
    {
     Serial.println("Включаем вентилятор");
     digitalWrite(PIN_RELAY, LOW); // Включаем реле
    } 
  else
    {
      Serial.println("Выключаем вентилятор");
      digitalWrite(PIN_RELAY, HIGH); // Отключаем реле
    }
  delay(1000); // Одна секунда
Temperature_sensor_file.flush();   
Temperature_sensor_file.close();
}

//----------------------------------------------------
void Soil_sensor()
{
  int SensorValue = analogRead(pinD2); //take a sample
   Serial.print(SensorValue); Serial.print(" - ");
   
    Soil_sensor_file.print(SensorValue);
    Soil_sensor_file.print(" - ");
  if(SensorValue >= 1000) {  
   Serial.println("Sensor is not in the Soil or DISCONNECTED");
  }
  if(SensorValue < 1000 && SensorValue >= 600) { 
   Serial.println("Soil is DRY");     // Сухо
     Soil_sensor_file.println("Soil is DRY");
   }
  if(SensorValue < 600 && SensorValue >= 370) {
   Serial.println("Soil is HUMID");   // Влажно
     Soil_sensor_file.println("Soil is HUMID");
  }
  if(SensorValue < 370) {
   Serial.println("Sensor in WATER");  // Очень вдажно
     Soil_sensor_file.println("Sensor in WATER");
     }  
        
if (SensorValue >= 650)
    {
     Serial.println("Включаем помпу");
     digitalWrite(PIN_RELAY2, LOW); // Включаем реле 
    } 
  else
    {
     Serial.println("Выключаем помпу");
     digitalWrite(PIN_RELAY2, HIGH); // Отключаем реле
    }
  delay (1000);
Soil_sensor_file.flush();   
Soil_sensor_file.close();
}

//----------------------------------------------------
void Rain_sensor()
{
  int sensorValue = analogRead(pinD3);
  Serial.print(sensorValue);
  Rain_sensor_file.print(sensorValue);
  if(sensorValue < thresholdValue){
    Serial.println(" - It's wet");  // Влажно
    Rain_sensor_file.println(" - It's wet");
  }
  else {
    Serial.println(" - It's dry");  //  Мокро
    Rain_sensor_file.println(" - It's dry");
  }
  delay (1000);
Rain_sensor_file.flush();   
Rain_sensor_file.close();
}

//----------------------------------------------------
void  CO2_sensor() {

  pinD4 = analogRead(pinD4);
  CO2_pinD = digitalRead(3);

  Serial.print("Current value: "); // Аналоговое значение
  Serial.println(pinD4);
  CO2_sensor_file.print("Current value: ");
  CO2_sensor_file.println(pinD4);
  
  Serial.print("Threshold: ");     // Цифровое значение
  Serial.println(CO2_pinD);
  CO2_sensor_file.print("Threshold: ");
  CO2_sensor_file.println(CO2_pinD);
   
  delay (1000);
CO2_sensor_file.flush();  
CO2_sensor_file.close();
}
