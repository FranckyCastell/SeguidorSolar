#define __DEBUG__
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DECLARACIONES PARA LA PANTALLA OLED
#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

// CREACION DE LOS SERVO MOTORES
Servo Shorizontal; // EJE HORIZONTAL
Servo Svertical;   // EJE VERTICAL

// LDR PINS
int ldr_01 = 33;       // PIN DERECHO
int ldr_02 = 35;       // PIN IZQUIERDO
int ldr_03 = 34;       // PIN ABAJO
int ldr_04 = 36;       // PIN ARRIBA
int Clean_Buffer = 39; // CLEAN BOOFER VALOR "GROUND" = 0

// VARIABLES DE LOS FOTORESISTENCIAS
int rightLDR = 0;  // VALOR DE LA FOTO RESISTENCIA ( ldr_01 )
int leftLDR = 0;   // VALOR DE LA FOTO RESISTENCIA ( ldr_02 )
int bottomLDR = 0; // VALOR DE LA FOTO RESISTENCIA ( ldr_03 )
int topLDR = 0;    // VALOR DE LA FOTO RESISTENCIA ( ldr_04 )

// DIFERENCIES ENTRE LES FOTORESISTENCIES
int horizontalError = 0; // DIFERENCIA ENTRE ( rightLDR & leftLDR )
int verticalError = 0;   // DIFERENCIA ENTRE ( topLDR & bottomLDR )

// POSICIONS DELS SERVOMOTORS INICIALS
int ShorizontalPos = 90; // DEFINIMOS POSICION INICIAL SERVO MOTOR HORIZONTAL
int SverticalPos = 90;   // DEFINIMOS POSICION INICIAL SERVO MOTOR VERTICALs

// VARIABLES PER ELS ANGLES
int acimut = 0;            // VALOR ANGULO ACIMUT
int inclinacio = 0;        // VALOR ANGULO INCLINACION
int posicioHorizontal = 0; // VALOR ANGULO POSICION SERVO HORIZONTAL
int posicioVertical = 0;   // VALOR ANGULO POSICION SERVO VERTICAL

// PINS SERVOMOTORS
void setup()
{

    // INICIAMOS PANTALLA OLED
#ifdef __DEBUG__
    Serial.begin(9600);
    delay(100);
    Serial.println("INICIANT PANTALL OLED");
#endif

    // INICIAMOS PANTALLA OLED EN LA DIRECCIÓN 0x3C
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
#ifdef __DEBUG__
        Serial.println("NO S'HA TROBAT LA PANTALLA OLED");
#endif
        while (true)
            ;
    }

    Shorizontal.attach(25); // DEFINIMOS SERVOMOTOR PIN 25 ( HORIZONTAL )
    Svertical.attach(26);   // DEFINIMOS SERVOMOTOR PIN 26 ( VERTICAL )
}

void loop()
{
    // OLED CONSTANTS
    display.clearDisplay();              // BORRAMOS CONTENIDO DE LA PANTALLA OLED
    display.setTextSize(1);              // DEFINIMOS TAMAÑO DEL TEXTO QUE SE VA A MOSTRAR EL 1 ES EL TAMAÑO MINIMO
    display.setTextColor(SSD1306_WHITE); // COLOR DEL TEXTO
    // OLED CONSTANTS

    // OLED LOOP
    display.setCursor(25, 10);       // MOVEMOS EL CURSOR EN LA POSICION ( 25, 10 ) DE LA PANTALLA OLED | COLUMNA, FILA
    display.print("~~~ PAMFER ~~~"); // IMPRIMIMOS TEXTO

    display.setCursor(20, 30);   // MOVEMOS EL CURSOR EN LA POSICION ( 20, 30 ) DE LA PANTALLA OLED | COLUMNA, FILA
    acimut = Shorizontal.read(); // LEEMOS VALORES DEL SERVO HORIZONTAL
    display.print("- ACIMUT: "); // IMPRIMIMOS TEXTO
    display.print(acimut);       // IMPRIMIMOS EL ACIMUT A LA QUE SE ENCUENTRA EL SERVO MOTOR EN ESTE MOMENTO

    display.setCursor(20, 40);       // MOVEMOS EL CURSOR EN LA POSICION ( 20, 40 ) DE LA PANTALLA OLED | COLUMNA, FILA
    inclinacio = Svertical.read();   // LEEMOS VALORES DEL SERVO VERTICAL
    display.print("- INCLINACIO: "); // IMPRIMIMOS TEXTO
    display.println(inclinacio);     // IMPRIMIMOS LA INCLINACION A LA QUE SE ENCUENTRA EL SERVO MOTOR EN ESTE MOMENTO

    display.display(); // ACTUALIZAMOS LA PANTALLA CON LOS DATOS ANTERIORES
    // OLED LOOP

    // LDR PRINT
    analogRead(Clean_Buffer); // PONEMOS EL BUFFER A VALOR DE 0

    // LDR_01
    int valorLDR_01 = analogRead(ldr_01); // LEEMOS LA ENTRADA ANALOGICA DE UN LDR
    Serial.print("LDR_01: ");             // IMPRIMIMOS TEXTO
    Serial.print(valorLDR_01);            // IMPRIMIMOS EL VALOR DEL LDR_01

    // LDR_02
    int valorLDR_02 = analogRead(ldr_02); // LEEMOS LA ENTRADA ANALOGICA DE UN LDR
    Serial.print(" LDR_02: ");            // IMPRIMIMOS TEXTO
    Serial.print(valorLDR_02);            // IMPRIMIMOS EL VALOR DEL LDR_02

    // LDR_03
    int valorLDR_03 = analogRead(ldr_03); // LEEMOS LA ENTRADA ANALOGICA DE UN LDR
    Serial.print(" LDR_03: ");            // IMPRIMIMOS TEXTO
    Serial.print(valorLDR_03);            // IMPRIMIMOS EL VALOR DEL LDR_03

    // LDR_04
    int valorLDR_04 = analogRead(ldr_04); // LEEMOS LA ENTRADA ANALOGICA DE UN LDR
    Serial.print(" LDR_04: ");            // IMPRIMIMOS TEXTO
    Serial.print(valorLDR_04);            // IMPRIMIMOS EL VALOR DEL LDR_04
    // LDR PRINT

    // SERVO PRINT
    posicioHorizontal = Shorizontal.read(); // LEEMOS LA POSICION DEL SERVO MOTOR HORIZONTAL
    Serial.print(" SERVO HORIZONTAL: ");    // IMPRIMIMOS TEXTO
    Serial.print(posicioHorizontal);        // IMPRIMIMOS LA POSICION DEL SERVO MOTOR HORIZONTAL

    posicioVertical = Svertical.read(); // LEEMOS LA POSICION DEL SERVO MOTOR HORIZONTAL
    Serial.print(" SERVO VERTICAL: ");  // IMPRIMIMOS TEXTO
    Serial.println(posicioVertical);    // IMPRIMIMOS LA POSICION DEL SERVO MOTOR HORIZONTAL
    // SERVO PRINT

    // LOGICA
    rightLDR = analogRead(ldr_01);        // VALOR DE LA RESISTENCIA DERECHA
    leftLDR = analogRead(ldr_02);         // VALOR DE LA RESISTENCIA IZQUIERDA
    horizontalError = rightLDR - leftLDR; // DIFERENCIAS ENTRE LAS DOS

    if (horizontalError > 20) // CONDICION LA CUAL SI ES > 20 MOVER HACIA LA DERECHA
    {
        ShorizontalPos--;
        ShorizontalPos = constrain(ShorizontalPos, 0, 179);
        Shorizontal.write(ShorizontalPos);
    }

    else if (horizontalError < -20) // CONDICION LA CUAL SI ES < 20 MOVER HACIA LA IZQUIERDA
    {
        ShorizontalPos++;
        ShorizontalPos = constrain(ShorizontalPos, 0, 179);
        Shorizontal.write(ShorizontalPos);
    }

    topLDR = analogRead(ldr_04);        // VALOR DE LA RESISTENCIA DE ARRIBA
    bottomLDR = analogRead(ldr_03);     // VALOR DE LA RESISTENCIA DE ABAJO
    verticalError = topLDR - bottomLDR; // DIFERENCIAS ENTRE LAS DOS

    if (verticalError > 50) // CONDICION LA CUAL SI ES < 50 MOVER HACIA ARRIBA
    {
        SverticalPos++;
        SverticalPos = constrain(SverticalPos, 10, 120);
        Svertical.write(SverticalPos);
    }

    else if (verticalError < -50) // CONDICION LA CUAL SI ES > 50 MOVER HACIA ABAJO
    {
        SverticalPos--;
        SverticalPos = constrain(SverticalPos, 10, 120);
        Svertical.write(SverticalPos);
    }
    delay(400);
    // LOGICA
}
