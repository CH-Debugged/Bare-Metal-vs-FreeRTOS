#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x27, 20, 4);

unsigned long pressTime = 0;
unsigned long responseTime = 0;
int taskCount = 0;

extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;
  if ((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__heap_start);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);
  return free_memory;
}

void busyTask() {
  volatile long x = 0;
  for (long i = 0; i < 100000; i++) {
    x += i;
  }
  taskCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  lcd1.init();
  lcd1.backlight();

  lcd1.setCursor(0, 0);
  lcd1.print("  Bare-Metal Mode  ");
  lcd1.setCursor(0, 1);
  lcd1.print("Response: --ms      ");
  lcd1.setCursor(0, 2);
  lcd1.print("Load: NONE          ");
  lcd1.setCursor(0, 3);
  lcd1.print("Press a button...   ");

  Serial.println("=== Bare-Metal Mode (No Load) ===");
  Serial.println("Free Memory | Task Count | Response Time");
}

void loop() {
  busyTask();

  if (taskCount % 50 == 0) {
    Serial.print(freeMemory());
    Serial.print(" bytes | Task #");
    Serial.print(taskCount);
    Serial.print(" | Response: ");
    Serial.print(responseTime);
    Serial.println("ms");
  }

  // Button 1
  if (digitalRead(2) == LOW) {
    pressTime = millis();
    digitalWrite(8, HIGH);
    responseTime = millis() - pressTime;
    lcd1.setCursor(0, 1);
    lcd1.print("Response: ");
    lcd1.print(responseTime);
    lcd1.print("ms        ");
    lcd1.setCursor(0, 3);
    lcd1.print("BTN1 Pressed!       ");
    Serial.print(freeMemory());
    Serial.print(" bytes | Task #");
    Serial.print(taskCount);
    Serial.print(" | Response: ");
    Serial.print(responseTime);
    Serial.println("ms  <-- BTN1");
  } else {
    digitalWrite(8, LOW);
  }

  // Button 2
  if (digitalRead(3) == LOW) {
    pressTime = millis();
    digitalWrite(9, HIGH);
    responseTime = millis() - pressTime;
    lcd1.setCursor(0, 1);
    lcd1.print("Response: ");
    lcd1.print(responseTime);
    lcd1.print("ms        ");
    lcd1.setCursor(0, 3);
    lcd1.print("BTN2 Pressed!       ");
    Serial.print(freeMemory());
    Serial.print(" bytes | Task #");
    Serial.print(taskCount);
    Serial.print(" | Response: ");
    Serial.print(responseTime);
    Serial.println("ms  <-- BTN2");
  } else {
    digitalWrite(9, LOW);
  }

  // Button 3
  if (digitalRead(4) == LOW) {
    pressTime = millis();
    digitalWrite(10, HIGH);
    responseTime = millis() - pressTime;
    lcd1.setCursor(0, 1);
    lcd1.print("Response: ");
    lcd1.print(responseTime);
    lcd1.print("ms        ");
    lcd1.setCursor(0, 3);
    lcd1.print("BTN3 Pressed!       ");
    Serial.print(freeMemory());
    Serial.print(" bytes | Task #");
    Serial.print(taskCount);
    Serial.print(" | Response: ");
    Serial.print(responseTime);
    Serial.println("ms  <-- BTN3");
  } else {
    digitalWrite(10, LOW);
  }

  if (digitalRead(2) == HIGH && digitalRead(3) == HIGH && digitalRead(4) == HIGH) {
    lcd1.setCursor(0, 3);
    lcd1.print("Press a button...   ");
  }
}
