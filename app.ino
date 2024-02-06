#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define __DEBUG__
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64   // alto pantalla OLED

// DECLARACIONS PER A LA PANTALLA
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

// CREACIO DELS SERVOS
Servo Shorizontal; // CREACIO DE L'OBJECTE SERVO PER A L'EIX HORIZONTAL
Servo Svertical;   // CREACIO DE L'OBJECTE SERVO PER A L'EIX VERTICAL

// LDR PINS
int ldr_01 = 33;       // DEFINIM PIN DRETA
int ldr_02 = 35;       // DEFINIM PIN ESQUERRA
int ldr_03 = 34;       // DEFINIM PIN BAIX
int ldr_04 = 36;       // DEFINIM PIN DALT
int Clean_Buffer = 39; // DEFINIM PIN PER A DEFINIR GROUD = 0

// VARIABLES DE LOS FOTORESISTENCIAS
int rightLDR = 0;
int leftLDR = 0;
int bottomLDR = 0;
int topLDR = 0;

// DIFERENCIES ENTRE LES FOTORESISTENCIES
int horizontalError = 0;
int verticalError = 0;

// POSICIONS DELS SERVOMOTORS INICIALS
int ShorizontalPos = 90;
int SverticalPos = 90;

// VARIABLES PER ELS ANGLES
int acimut = 0;
int inclinacio = 0;
int posicion = 0;

// PINS SERVOMOTORS
void setup()
{
#ifdef __DEBUG__
    Serial.begin(9600);
    delay(100);
    Serial.println("INICIANT PANTALL OLED");
#endif

    // Iniciar pantalla OLED en la dirección 0x3C
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
#ifdef __DEBUG__
        Serial.println("NO S'HA TROBAT LA PANTALLA OLED");
#endif
        while (true)
            ;
    }
    Shorizontal.attach(25); // DEFINIM SERVOMOTOR 1 PIN
    Svertical.attach(26);   // DEFINIM SERVOMOTOR 2 PIN
}

void loop()
{
    // OLED CONSTANTS
    display.clearDisplay();              // BORRAMOS CONTENIDO DE LA PANTALLA OLED
    display.setTextSize(1);              // DEFINIMOS TAMAÑO DEL TEXTO QUE SE VA A MOSTRAR EL 1 ES EL TAMAÑO MINIMO
    display.setTextColor(SSD1306_WHITE); // COLOR DEL TEXTO
    // OLED CONSTANTS

    // OLED LOOP
    display.setCursor(25, 10); // MOVEM EL CURSOS A LA POSICIO (0, 10) DE LA PANTALLA OLED // COLUMNA , FILERA
    display.print("~~~ PAMFER ~~~");

    display.setCursor(20, 30);   // MOVEM EL CURSOS A LA POSICIO (0, 10) DE LA PANTALLA OLED // COLUMNA , FILERA
    acimut = Shorizontal.read(); // LLEGIM VALORS DEL SERVO HORIZONTAL
    display.print("- ACIMUT: ");
    display.print(acimut); // IMPRIMIM L'ACIMUT A LA QUE ES TROBA EL SERVO EN AQUEST MOMENT

    display.setCursor(20, 40);     // MOVEM EL CURSOS A LA POSICIO (0, 20) DE LA PANTALLA OLED // COLUMNA , FILERA
    inclinacio = Svertical.read(); // LLEGIM VALORS DEL SERVO VERTICAL
    display.print("- INCLINACIO: ");
    display.println(inclinacio); // IMPRIMIM LA INCLINACIÓ A LA QUE ES TROBA EL SERVO EN AQUEST MOMENT

    display.display(); // ACTUALIZAM LA PANTALLA AMB ELS DATOS ANTERIORS
    // OLED LOOP

    // LDR PRINT
    analogRead(Clean_Buffer); // Ponemos el Buffer a 0

    // LDR_01
    int valorLDR_01 = analogRead(ldr_01); // Leemos la entrada analogica de un LDR
    Serial.print("LDR_01: ");             // Texto
    Serial.print(valorLDR_01);            // Imprimimos el valor de un LDR en este caso

    // LDR_02
    int valorLDR_02 = analogRead(ldr_02); // Leemos la entrada analogica de un LDR
    Serial.print(" LDR_02: ");            // Texto
    Serial.print(valorLDR_02);            // Imprimimos el valor de un LDR en este caso

    // LDR_03
    int valorLDR_03 = analogRead(ldr_03); // Leemos la entrada analogica de un LDR
    Serial.print(" LDR_03: ");            // Texto
    Serial.print(valorLDR_03);            // Imprimimos el valor de un LDR en este caso

    // LDR_04
    int valorLDR_04 = analogRead(ldr_04); // Leemos la entrada analogica de un LDR
    Serial.print(" LDR_04: ");            // Texto
    Serial.print(valorLDR_04);            // Imprimimos el valor de un LDR en este caso
    // LDR PRINT

    // SERVO PRINT
    posicion = Shorizontal.read();
    Serial.print(" SERVO HORIZONTAL: ");
    Serial.print(posicion);

    posicion = Svertical.read();
    Serial.print(" SERVO VERTICAL: ");
    Serial.println(posicion);
    // SERVO PRINT

    rightLDR = analogRead(ldr_01);        // VALOR DE LA RESISTENCIA DRETA
    leftLDR = analogRead(ldr_02);         // VALOR DE LA RESISTENCIA ESQUERRA
    horizontalError = rightLDR - leftLDR; // DIFERENCIES DELS DOS SENSORS

    // LOGICA
    if (horizontalError > 20) // CONDICIO LA CUAL SI ES > 20 MOURE CAP A LA DRETA
    {
        ShorizontalPos--;
        ShorizontalPos = constrain(ShorizontalPos, 0, 179);
        Shorizontal.write(ShorizontalPos);
    }

    else if (horizontalError < -20) // CONDICIO LA CUAL SI ES < 20 MOURE CAP A LA ESQUERRA
    {
        ShorizontalPos++;
        ShorizontalPos = constrain(ShorizontalPos, 0, 179);
        Shorizontal.write(ShorizontalPos);
    }

    topLDR = analogRead(ldr_04);        // VALOR DE LA RESISTENCIA DALT
    bottomLDR = analogRead(ldr_03);     // VALOR DE LA RESISTENCIA BAIX
    verticalError = topLDR - bottomLDR; // Difference between top and bottom readings

    if (verticalError > 50) // CONDICIO LA CUAL SI ES < 50 MOURE CAP A DALT
    {
        SverticalPos++;
        SverticalPos = constrain(SverticalPos, 10, 120);
        Svertical.write(SverticalPos);
    }

    else if (verticalError < -50) // CONDICIO LA CUAL SI ES > 50 MOURE CAP A BAIX
    {
        SverticalPos--;
        SverticalPos = constrain(SverticalPos, 10, 120);
        Svertical.write(SverticalPos);
    }
    delay(1000); // 25
                // LOGICA
}
