#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x27, 20, 4);

SemaphoreHandle_t lcdMutex;

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

void TaskBusy(void *pvParameters) {
  for (;;) {
    volatile long x = 0;
    for (long i = 0; i < 500000; i++) {
      x += i;
    }
    taskCount++;
    if (taskCount % 10 == 0) {
      Serial.print(freeMemory());
      Serial.print(" bytes | Task #");
      Serial.print(taskCount);
      Serial.print(" | Response: ");
      Serial.print(responseTime);
      Serial.println("ms");
    }
    vTaskDelay(1);
  }
}

void TaskButton1(void *pvParameters) {
  for (;;) {
    if (digitalRead(2) == LOW) {
      pressTime = millis();
      digitalWrite(8, HIGH);
      responseTime = millis() - pressTime;
      if (xSemaphoreTake(lcdMutex, portMAX_DELAY)) {
        lcd1.setCursor(0, 1);
        lcd1.print("Response: ");
        lcd1.print(responseTime);
        lcd1.print("ms        ");
        lcd1.setCursor(0, 3);
        lcd1.print("BTN1 Pressed!       ");
        xSemaphoreGive(lcdMutex);
      }
      Serial.print(freeMemory());
      Serial.print(" bytes | Task #");
      Serial.print(taskCount);
      Serial.print(" | Response: ");
      Serial.print(responseTime);
      Serial.println("ms  <-- BTN1");
    } else {
      digitalWrite(8, LOW);
    }
    vTaskDelay(1);
  }
}

void TaskButton2(void *pvParameters) {
  for (;;) {
    if (digitalRead(3) == LOW) {
      pressTime = millis();
      digitalWrite(9, HIGH);
      responseTime = millis() - pressTime;
      if (xSemaphoreTake(lcdMutex, portMAX_DELAY)) {
        lcd1.setCursor(0, 1);
        lcd1.print("Response: ");
        lcd1.print(responseTime);
        lcd1.print("ms        ");
        lcd1.setCursor(0, 3);
        lcd1.print("BTN2 Pressed!       ");
        xSemaphoreGive(lcdMutex);
      }
      Serial.print(freeMemory());
      Serial.print(" bytes | Task #");
      Serial.print(taskCount);
      Serial.print(" | Response: ");
      Serial.print(responseTime);
      Serial.println("ms  <-- BTN2");
    } else {
      digitalWrite(9, LOW);
    }
    vTaskDelay(1);
  }
}

void TaskButton3(void *pvParameters) {
  for (;;) {
    if (digitalRead(4) == LOW) {
      pressTime = millis();
      digitalWrite(10, HIGH);
      responseTime = millis() - pressTime;
      if (xSemaphoreTake(lcdMutex, portMAX_DELAY)) {
        lcd1.setCursor(0, 1);
        lcd1.print("Response: ");
        lcd1.print(responseTime);
        lcd1.print("ms        ");
        lcd1.setCursor(0, 3);
        lcd1.print("BTN3 Pressed!       ");
        xSemaphoreGive(lcdMutex);
      }
      Serial.print(freeMemory());
      Serial.print(" bytes | Task #");
      Serial.print(taskCount);
      Serial.print(" | Response: ");
      Serial.print(responseTime);
      Serial.println("ms  <-- BTN3");
    } else {
      digitalWrite(10, LOW);
    }
    vTaskDelay(1);
  }
}

void TaskIdle(void *pvParameters) {
  for (;;) {
    if (digitalRead(2) == HIGH && digitalRead(3) == HIGH && digitalRead(4) == HIGH) {
      if (xSemaphoreTake(lcdMutex, portMAX_DELAY)) {
        lcd1.setCursor(0, 3);
        lcd1.print("Press a button...   ");
        xSemaphoreGive(lcdMutex);
      }
    }
    vTaskDelay(10);
  }
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
  lcd1.print("  FreeRTOS Mode    ");
  lcd1.setCursor(0, 1);
  lcd1.print("Response: --ms      ");
  lcd1.setCursor(0, 2);
  lcd1.print("Load: ACTIVE        ");
  lcd1.setCursor(0, 3);
  lcd1.print("Press a button...   ");

  Serial.println("=== FreeRTOS Mode (With Load) ===");
  Serial.println("Free Memory | Task Count | Response Time");

  lcdMutex = xSemaphoreCreateMutex();

  xTaskCreate(TaskButton1, "BTN1", 128, NULL, 3, NULL);
  xTaskCreate(TaskButton2, "BTN2", 128, NULL, 3, NULL);
  xTaskCreate(TaskButton3, "BTN3", 128, NULL, 3, NULL);
  xTaskCreate(TaskBusy,   "BUSY", 128, NULL, 1, NULL);
  xTaskCreate(TaskIdle,   "IDLE", 128, NULL, 2, NULL);

  vTaskStartScheduler();
}

void loop() {
}
