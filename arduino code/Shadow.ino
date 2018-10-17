// HAKSTER Shadow v2.5 


// !ВНИМАНИЕ !WARNING
// (Ихсодный код проекта полностью не оптимизирован) 
// TODO: Скоро оптимизирую :)


// -------------------- НАСТРОЙКИ -------------------------
#define dR 0  // Значение цвтетов по умолчанию при первом запуске.
#define dG 0  // Значение цвтетов по умолчанию при первом запуске.
#define dB 0  // Значение цвтетов по умолчанию при первом запуске.
#define minTempD 20 // Значение минимальной температуры при первом запуске
#define maxTempD 60 // Значение максимальной температуры при первом запуске
// -------------------- НАСТРОЙКИ -------------------------

// -------------------- БИБЛИОТЕКИ ------------------------
#include <EEPROM.h>             // библиотека работы с постоянной памятью
#include <iarduino_OLED.h>      // библиотека (Графическая) OLed дисплея 128x64 I2C ssd1306 12864 0.96 дюймов
#include <OneWire.h>            // библиотека протокола датчиков
#include <TimerOne.h>           // библиотека таймера

extern uint8_t SmallFontRus[];  // Подключаем шрифт SmallFontRus.
extern uint8_t MediumFont[];    // Подключаем шрифт MediumFont.
extern uint8_t Img_Settings[];  // Подключаем изображение иконки настроек Img_Settings.
extern uint8_t Img_Arrow_left[];
extern uint8_t Img_Arrow_right[];

const uint8_t myImageInRAM[] = // Создаём массив myImageInRAM содержащий изображение смайлика. Массив хранится в области ОЗУ (RAM).
{   10, 10,           //             
    B00000011,        //           
    B00000011,        //            
    B00000000,        //
    B00011000,        //         
    B00011000,        //          
    B11111111,        //    
    B11111111,        //           
    B00011000,        //           
    B00011000,        //            
    B00000000,        //

    B00000000,        //            
    B00000000,        //            
    B00000000,        //
    B00000000,        //           
    B00000000,        //            
    B00000001,        //       
    B00000011,        //           
    B00000011,        //            
    B00000011,        //      
    B00000000//
};                    

// -------------------- БИБЛИОТЕКИ ------------------------

// ----------------------- ПИНЫ ---------------------------
#define FAN_PIN 9               // на мосфет вентиляторов
#define R_PIN 5                 // на мосфет ленты, красный
#define G_PIN 3                 // на мосфет ленты, зелёный
#define B_PIN 6                 // на мосфет ленты, синий
#define BTN1 A3                 // первая кнопка
#define BTN2 A2                 // вторая кнопка
#define BTN3 A1                 // вторая кнопка
OneWire ds(14);                 // датчик температуры
iarduino_OLED myOLED(0x3C);     // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x78 (если учитывать бит RW=0).
// ----------------------- ПИНЫ ---------------------------

// -------------------- ПЕРЕМЕННЫЕ ------------------------
int temperature, R, G, B, minTemp, maxTemp, time_menu, max_now_temp;
long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 100; // Определяем периодичность проверок
boolean btn1_sig, btn2_sig, btn3_sig,  menu_flag;
int i = 0;
// -------------------- ПЕРЕМЕННЫЕ ------------------------


void setup() {
  Serial.begin(9600);
  myOLED.begin();
  Timer1.initialize(40);   // поставить частоту ШИМ 25 кГц (40 микросекунд)

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(R_PIN, 0);
  digitalWrite(G_PIN, 0);
  digitalWrite(B_PIN, 0);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

if (!EEPROM.read(0)){
  EEPROM.write(0, 1);
  EEPROM.write(1, dR);
  EEPROM.write(2, dG);
  EEPROM.write(3, dB);
  EEPROM.write(4, minTempD);
  EEPROM.write(5, maxTempD);

  R = EEPROM.read(1);
  G = EEPROM.read(2);
  B = EEPROM.read(3);
  minTemp = EEPROM.read(4);
  maxTemp = EEPROM.read(5);
  
}
  R = EEPROM.read(1);
  G = EEPROM.read(2);
  B = EEPROM.read(3);
  minTemp = EEPROM.read(4);
  maxTemp = EEPROM.read(5);

  Timer1.pwm(FAN_PIN, 400);
  temperature = 0;
  myOLED.setFont(MediumFont);
  myOLED.invText();                                   // Указываем что цвет текста выводимого после данной функции должен быть инвертирован.
  myOLED.print("SHADOW LED", OLED_C, 31);             // Выводим текст по центру экрана, координата нижней части текста по оси Y равна 31. Текст будет написан черными буквами на белом фоне.
  myOLED.invText(false);    
  myOLED.setFont(SmallFontRus);  
  myOLED.print("*** by  HAKSTER ***", OLED_C, 40);
  myOLED.print("v:2.11", OLED_C, 60);
  delay(3000);
  myOLED.clrScr(); 
}
// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------
void loop() {
detectTemperature();
LEDcontrol();
buttonsTick();
OledControl();
}
// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------

// ---- Функция получение значений температуры с датчика DS18b20
int detectTemperature(){
 
  byte data[2];
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);
 
  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    lastUpdateTime = millis();
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);
    data[0] = ds.read();
    data[1] = ds.read();
 
    // Формируем значение
    temperature = (data[1] << 8) + data[0]; temperature = temperature >> 4;
   
  }
}

// ---- Функция парсинга данных полученых с serial порта 
// Пока не используется 
void parsing(){
  while (Serial.available() > 0) {
    if  (int ser = Serial.parseInt()){
      
    }
  }
}

// ---- Функция управления RGB лентой 
void LEDcontrol() {
 
  int b = map(temperature,minTemp,maxTemp, 0, 250);
  b = constrain(b, 0, 250);  

  int Rf = (b * R / 100);
  int Gf = (b * G / 100);
  int Bf = (b * B / 100);

  // Исправление бага с миганием разными цветами на высокой температуре
  if(Rf >= 255 || Rf < 0){
    Rf = 255;
  }
  if(Gf >= 255 || Gf < 0){
    Gf = 255;
  }
  if(Bf >= 255 || Bf < 0){
    Bf = 255;
  }
  
  analogWrite(R_PIN, Rf);
  analogWrite(G_PIN, Gf);
  analogWrite(B_PIN, Bf); 
}

// ---- Функция обработки нажатия кнопок 
void buttonsTick() {
  btn1_sig = !digitalRead(BTN1);
  btn2_sig = !digitalRead(BTN2);
  btn3_sig = !digitalRead(BTN3);

  if (btn1_sig && btn2_sig){
    time_menu = 0;
    myOLED.clrScr();
    time_menu = 0;
    menu_flag = !menu_flag;
    if(menu_flag){
    EEPROM.update(1, R);
    EEPROM.update(2, G);
    EEPROM.update(3, B);
    EEPROM.update(4, minTemp);
    EEPROM.update(5, maxTemp);
    }
  }

}

// ---- Функция вывода на Oled экран
// TODO: Оптимизировать!!!
void OledControl(){

  if(menu_flag){
    int menuNamber[2];

        switch (i) {
         case 4:
              
              myOLED.update();
              myOLED.setFont(SmallFontRus); 
              myOLED.drawImage(Img_Settings, OLED_L, OLED_T);
              myOLED.print("Настройки диапазона"    ,      10, 8);
              myOLED.drawLine   (0, 10, 128, 10,        1);   
              myOLED.print("Мин. темп:"    ,      0, 24); 
              myOLED.print(minTemp    ,      70, 24); 
              myOLED.invText();
              myOLED.print("Макс. темп:"    ,      0, 34);
              myOLED.invText(false); 
              myOLED.print(maxTemp    ,      70, 34);
              myOLED.drawImage(Img_Arrow_left, 0, 44);
              myOLED.print("Назад"    ,      8, 44);
              if (btn1_sig){
                maxTemp++;
              }
              if (btn2_sig){
                maxTemp--;
              }
            break;
         case 3:

              myOLED.update();
              myOLED.setFont(SmallFontRus); 
              myOLED.drawImage(Img_Settings, OLED_L, OLED_T);
              myOLED.print("Настройки диапазона"    ,      10, 8);
              myOLED.drawLine   (0, 10, 128, 10,        1);  
              myOLED.invText();
              myOLED.print("Мин темп:"    ,      0, 24);
              myOLED.invText(false); 
              myOLED.print(minTemp    ,      70, 24); 
              myOLED.print("Макс темп:"    ,      0, 34); 
              myOLED.print(maxTemp    ,      70, 34); 
              myOLED.drawImage(Img_Arrow_left, 0, 44);    
              myOLED.print("Назад"    ,      8, 44);
              if (btn1_sig){
                minTemp++;
              }
              if (btn2_sig){
                minTemp--;
              }
            break;
         case 2:
              
              myOLED.update();
              myOLED.setFont(SmallFontRus); 
              myOLED.drawImage(Img_Settings, OLED_L, OLED_T);
              myOLED.print("Настройки цвета"    ,      10, 8);
              myOLED.drawLine   (0, 10, 128, 10,        1);   
              myOLED.print("Красный:"    ,      0, 24); 
              myOLED.print(R    ,      70, 24); 
              myOLED.print("Зеленый:"    ,      0, 34); 
              myOLED.print(G    ,      70, 34);
              myOLED.invText();       
              myOLED.print("Синий:"    ,      0, 44);
              myOLED.invText(false); 
              myOLED.print(B    ,      70, 44); 
              myOLED.drawImage(Img_Arrow_right, 37, 54);  
              myOLED.print("Вперед"    ,      0, 54);
              if (btn1_sig){
                B++;
              }
              if (btn2_sig){
                B--;
              }
            break;
         case 1:
              myOLED.update();
              myOLED.setFont(SmallFontRus); 
              myOLED.drawImage(Img_Settings, OLED_L, OLED_T);
              myOLED.print("Настройки цвета"    ,      10, 8);
              myOLED.drawLine   (0, 10, 128, 10,        1); 
                
              myOLED.print("Красный:"    ,      0, 24); 
              myOLED.print(R    ,      70, 24);   
              myOLED.invText();
              myOLED.print("Зеленый:"    ,      0, 34);
              myOLED.invText(false); 
              myOLED.print(G    ,      70, 34);       
              myOLED.print("Синий:"    ,      0, 44); 
              myOLED.print(B    ,      70, 44);
              myOLED.drawImage(Img_Arrow_right, 37, 54);  
              myOLED.print("Вперед"    ,      0, 54);
              if (btn1_sig){
                G++;
              }
              if (btn2_sig){
                G--;
              } 
            break;
         default:
              myOLED.update();
              myOLED.setFont(SmallFontRus); 
              myOLED.drawImage(Img_Settings, OLED_L, OLED_T);
              myOLED.print("Настройки цвета"    ,      10, 8);
              myOLED.drawLine   (0, 10, 128, 10,        1); 
              myOLED.invText();  
              myOLED.print("Красный:"    ,      0, 24);
              myOLED.invText(false); 
              myOLED.print(R    ,      70, 24);   
              myOLED.print("Зеленый:"    ,      0, 34); 
              myOLED.print(G    ,      70, 34);       
              myOLED.print("Синий:"    ,      0, 44); 
              myOLED.print(B    ,      70, 44);
              myOLED.drawImage(Img_Arrow_right, 37, 54);  
              myOLED.print("Вперед"    ,      0, 54);
              if (btn1_sig){
                R++;
              }
              if (btn2_sig){
                R--;
              }
          }
          
          time_menu = time_menu + 1;
          if  (btn1_sig || btn2_sig || btn3_sig){
            time_menu = 0;
          }
          Serial.println(time_menu, DEC);
          if (time_menu == 50){
            i = 0;
            myOLED.clrScr();
            menu_flag = !menu_flag;
            EEPROM.update(1, R);
            EEPROM.update(2, G);
            EEPROM.update(3, B);
            EEPROM.update(4, minTemp);
            EEPROM.update(5, maxTemp);
    
            }
         if (btn3_sig){

          i++;
          if  (i > 4) { i = 0; 
          myOLED.clrScr();
          }
          if (i == 3){
            myOLED.clrScr(); 
          }
          
          
          delay(10);
          myOLED.update();
        }

    }else{
      myOLED.update();// Обновляем информацию на дисплее.
      myOLED.setFont(MediumFont);
      myOLED.drawImage(myImageInRAM, 0, 10, IMG_RAM);
      myOLED.print(temperature, 13, OLED_T);
      if(temperature > max_now_temp){
        max_now_temp = temperature;
      }
      myOLED.drawLine   (0, 24, 128, 24,        1);
      myOLED.drawLine   (0, 23, 128, 23,        1);
      myOLED.drawLine   (0, 22, 128, 22,        1);
        myOLED.invText();   
      myOLED.print("  FENIX  ", OLED_C, OLED_C);
        myOLED.invText(false);   
      myOLED.drawLine   (0, 40, 128, 40,        1);
       myOLED.drawLine   (0, 41, 128, 41,        1);
       myOLED.drawLine   (0, 42, 128, 42,        1);
       
//myOLED.drawCircle (80, 30,       5, true,  1);
      myOLED.setFont(SmallFontRus);
      myOLED.print("Max:", 70, OLED_B);
      myOLED.setFont(MediumFont);
      myOLED.print(max_now_temp, OLED_R, OLED_B);
      if(temperature < 100){
          myOLED.print("  ", 38, OLED_T);
        }else{
          }
    }
}

 

