#include <Servo.h> // IMPORTAM LA LLIBRERIA SERVO

// IMPORTAM LES LLIBRERIES PER A LA PANTALLA OLED
#include <Wire.sh>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // DEFINIM ANCHURA DE LA PANTALLA EN PIXELS
#define SCREEN_HEIGHT 64 // DEFINIM ALTURA DE LA PANTALLA EN PIXELS

// DECLARACIONS PER A LA PANTALLA
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// CREACIO DELS SERVOS
Servo Shorizontal; // CREACIO DE L'OBJECTE SERVO PER A L'EIX HORIZONTAL
Servo Svertical;   // CREACIO DE L'OBJECTE SERVO PER A L'EIX VERTICAL

// LDR PINS
int ldr_01 = 33;       // PIN DRETA
int ldr_02 = 35;       // PIN ESQUERRA
int ldr_03 = 34;       // PIN BAIX
int ldr_04 = 36;       // PIN DALT
int Clean_Buffer = 39; // PIN PER A DEFINIR GROUD = 0

// VARIABLES DE LOS FOTORESISTENCIAS
int rightLDR = 0;
int leftLDR = 0;
int bottomLDR = 0;
int topLDR = 0;

// DIFERENCIES ENTRE LES FOTORESISTENCIES
int horizontalError = 0;
int verticalError = 0;
int ShorizontalPos = 90;
int SverticalPos = 90;

// PINS SERVOMOTORS
void setup()
{
    Serial.begin(9600);
    Shorizontal.attach(25); // SERVOMOTOR 1
    Svertical.attach(26);   // SERVOMOTOR 2
}

void loop()
{
    // OLED CONSTANTS
    display.clearDisplay();      // BORRAMOS CONTENIDO DE LA PANTALLA OLED
    display.setTextSize(1);      // DEFINIMOS TAMAÑO DEL TEXTO QUE SE VA A MOSTRAR EL 1 ES EL TAMAÑO MINIMO
    display.setTextColor(WHITE); // COLOR DEL TEXTO
    // OLED CONSTANTS

    // OLED LOOP
    display.setCursor(0, 10);        // MOVEM EL CURSOS A LA POSICIO (0, 10) DE LA PANTALLA OLED
    acimut = Shorizontal.read();     // LLEGIM VALORS DEL SERVO HORIZONTAL
    display.print("ACIMUT", acimut); // IMPRIMIM L'ACIMUT A LA QUE ES TROBA EL SERVO EN AQUEST MOMENT

    display.setCursor(0, 20);                    // MOVEM EL CURSOS A LA POSICIO (0, 20) DE LA PANTALLA OLED
    inclinacio = Svertical.read();               // LLEGIM VALORS DEL SERVO VERTICAL
    display.println("INCLINACIO: ", inclinacio); // IMPRIMIM LA INCLINACIÓ A LA QUE ES TROBA EL SERVO EN AQUEST MOMENT

    display.display(); // ACTUALIZAM LA PANTALLA AMB ELS DATOS ANTERIORS
    // OLED LOOPs

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
    Serial.println(posicion);

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
    delay(100); // 25
                // LOGICA
}
