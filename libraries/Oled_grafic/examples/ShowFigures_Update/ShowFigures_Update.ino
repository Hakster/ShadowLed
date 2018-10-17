//  Раскомментируйте для программной реализации шины I2C:              //
//  #define pin_SW_SDA 3                                               // Назначение любого вывода Arduino для работы в качестве линии SDA программной шины I2C.
//  #define pin_SW_SCL 9                                               // Назначение любого вывода Arduino для работы в качестве линии SCL программной шины I2C.
//  Раскомментируйте для совместимости с большинством плат:            //
//  #include <Wire.h>                                                  // Библиотека iarduino_OLED будет использовать методы и функции библиотеки Wire.
//  Ссылки для ознакомления:                                           //
//  Подробная информация о подключении модуля к шине I2C:              // http://wiki.iarduino.ru/page/i2c_connection/
//  Подробная информация о функциях и методах библиотеки:              // http://wiki.iarduino.ru/page/OLED_trema/
//  Библиотека  iarduino_OLED_txt (текстовая) экономит ОЗУ:            // http://iarduino.ru/file/341.html
//  Бибилиотека iarduino_OLED     (графическая):                       // http://iarduino.ru/file/340.html
                                                                       // 
#include <iarduino_OLED.h>                                             // Подключаем библиотеку iarduino_OLED.
iarduino_OLED myOLED(0x3C);                                            // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.
                                                                       //
void setup(){                                                          //
    myOLED.begin();                                                    // Инициируем работу с дисплеем.
    myOLED.autoUpdate(false);                                          // Запрещаем автоматический вывод данных. Информация на дисплее будет обновляться только после обращения к функции update().
}                                                                      //
                                                                       //
void loop(){                                                           //
    myOLED.drawRect   (10, 10,  50, 50, true , true );                 // Прорисовываем квадрат по двум точкам: 10x10, 50x50       , закрасить - true  (да) , цвет - true  (белый).
    myOLED.drawRect   (15, 15,  45, 45, true , false);                 // Прорисовываем квадрат по двум точкам: 15x15, 45x45       , закрасить - true  (да) , цвет - false (черный).
    myOLED.drawLine   (10, 10,  50, 50,        false);                 // Прорисовываем линию через две точки:  10x10, 50x50       ,                          цвет - false (черный).
    myOLED.drawLine   (10, 50,  50, 10,        false);                 // Прорисовываем линию через две точки:  10x50, 50x10       ,                          цвет - false (черный).
    myOLED.drawCircle (30, 30,      10, false, true );                 // Прорисовываем круг с центром с точке  30x30 и радиусом 10, закрасить - false (нет), цвет - true  (белый).
    myOLED.drawCircle (30, 30,       5, true , true );                 // Прорисовываем круг с центром с точке  30x30 и радиусом 5 , закрасить - true  (да) , цвет - true  (белый).
    myOLED.drawPixel  (30, 30,                 false);                 // Прорисовываем точку в координате      30x30,                                        цвет - false (черный).
    myOLED.drawRect   (60, 10, 100, 50, false, true );                 // Прорисовываем квадрат по двум точкам: 60x10, 100x50      , закрасить - false (нет), цвет - true  (белый).
    myOLED.drawRect   (65, 15,  95, 45, true , true );                 // Прорисовываем квадрат по двум точкам: 65x15,  95x45      , закрасить - true  (да) , цвет - true  (белый).
    myOLED.drawLine   (60, 10, 100, 50,        true );                 // Прорисовываем линию через две точки:  60x10, 100x50      ,                          цвет - true  (белый).
    myOLED.drawLine   (60, 50, 100, 10,        true );                 // Прорисовываем линию через две точки:  60x50, 100x10      ,                          цвет - true  (белый).
    myOLED.drawCircle (80, 30,      10, false, false);                 // Прорисовываем круг с центром с точке  80x30 и радиусом 10, закрасить - false (нет), цвет - false (черный).
    myOLED.drawCircle (80, 30,       5, true,  false);                 // Прорисовываем круг с центром с точке  80x30 и радиусом 10, закрасить - true  (да) , цвет - false (черный).
    myOLED.drawPixel  (80, 30,                 true );                 // Прорисовываем точку в координате      80x30,                                        цвет - true  (белый).
    myOLED.update();                                                   // Обновляем информацию на дисплее.
    delay(2000);                                                       // Ждём 2 секунды.
    myOLED.clrScr();                                                   // Чистим экран.
    myOLED.update();                                                   // Обновляем информацию на дисплее.
    delay(1000);                                                       // Ждём 1 секунду.
}                                                                      //
