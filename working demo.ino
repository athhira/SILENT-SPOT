#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);


const int SENSOR_AREA1 = 36;  
const int SENSOR_AREA2 = 39;  
const int SENSOR_AREA3 = 34;  


int noiseArea1 = 0;
int noiseArea2 = 0;
int noiseArea3 = 0;


const int SAMPLES = 50;        
const int SAMPLE_DELAY = 20;   
const int UPDATE_INTERVAL = 2000; 

unsigned long lastUpdate = 0;

void setup() {
  
  Serial.begin(115200);
  Serial.println("Noise Monitoring System Starting...");
  
  
  lcd.init();
  lcd.backlight();
  
  
  lcd.setCursor(0, 0);
  lcd.print("Noise Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  
  
  pinMode(SENSOR_AREA1, INPUT);
  pinMode(SENSOR_AREA2, INPUT);
  pinMode(SENSOR_AREA3, INPUT);
  
  Serial.println("System Ready!");
}

void loop() {
  unsigned long currentTime = millis();
  
  
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    
    noiseArea1 = readNoiseLevelAverage(SENSOR_AREA1);
    noiseArea2 = readNoiseLevelAverage(SENSOR_AREA2);
    noiseArea3 = readNoiseLevelAverage(SENSOR_AREA3);
    
    Serial.println("\n========== Noise Levels ==========");
    Serial.print("Area 1: ");
    Serial.print(noiseArea1);
    Serial.println(" dB");
    Serial.print("Area 2: ");
    Serial.print(noiseArea2);
    Serial.println(" dB");
    Serial.print("Area 3: ");
    Serial.print(noiseArea3);
    Serial.println(" dB");
    
    int quietestArea = findQuietestArea();
    int quietestLevel = getQuietestLevel();
    
    Serial.println("----------------------------------");
    Serial.print("Quietest Area: Area ");
    Serial.println(quietestArea);
    Serial.print("Noise Level: ");
    Serial.print(quietestLevel);
    Serial.println(" dB");
    Serial.println("==================================\n");
    
    displayResults(quietestArea, quietestLevel);
  }
}

int readNoiseLevelAverage(int pin) {
  long sum = 0;
  
  for (int i = 0; i < SAMPLES; i++) {
    int rawValue = analogRead(pin);
    int dbValue = map(rawValue, 0, 4095, 30, 100);
    sum += dbValue;
    delay(SAMPLE_DELAY);
  }
  
  return sum / SAMPLES;
}

int findQuietestArea() {
  if (noiseArea1 <= noiseArea2 && noiseArea1 <= noiseArea3) {
    return 1;
  } else if (noiseArea2 <= noiseArea1 && noiseArea2 <= noiseArea3) {
    return 2;
  } else {
    return 3;
  }
}

int getQuietestLevel() {
  int minNoise = noiseArea1;
  if (noiseArea2 < minNoise) minNoise = noiseArea2;
  if (noiseArea3 < minNoise) minNoise = noiseArea3;
  return minNoise;
}

void displayResults(int quietestArea, int quietestLevel) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Quietest: Area ");
  lcd.print(quietestArea);
  
  lcd.setCursor(0, 1);
  lcd.print("Level: ");
  lcd.print(quietestLevel);
  lcd.print(" dB");
  
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A1:");
  lcd.print(noiseArea1);
  lcd.print(" A2:");
  lcd.print(noiseArea2);
  
  lcd.setCursor(0, 1);
  lcd.print("A3:");
  lcd.print(noiseArea3);
  lcd.print(" dB");
}