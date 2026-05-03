#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// LCD I2C con la direccion que ya encontraste: 0x20
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Cantidad de filas y columnas del teclado
const byte FILAS = 4;
const byte COLUMNAS = 4;

// Distribucion de teclas del teclado 4x4
char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pines del teclado conectados al Arduino
byte pinesFilas[FILAS] = {9, 8, 7, 6};
byte pinesColumnas[COLUMNAS] = {5, 4, 3, 2};

// Se crea el objeto teclado
Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

// Posicion inicial de la pelota
int columna = 0;
int fila = 0;

// Caracter personalizado con forma de pelota
byte pelota[8] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000
};

void setup()
{
  // Inicializa el LCD
  lcd.init();

  // Enciende la luz de fondo
  lcd.backlight();

  // Crea el caracter personalizado en la posicion 0
  lcd.createChar(0, pelota);

  // Limpia la pantalla
  lcd.clear();

  // Mensaje inicial
  lcd.setCursor(0, 0);
  lcd.print("Punto movil");
  lcd.setCursor(0, 1);
  lcd.print("2 4 6 8");
  delay(1500);

  lcd.clear();

  // Dibuja la pelota en la posicion inicial
  lcd.setCursor(columna, fila);
  lcd.write(byte(0));
}

void loop()
{
  // Lee la tecla presionada
  char tecla = teclado.getKey();

  // Si se presiono una tecla
  if (tecla)
  {
    // Borra la posicion anterior
    lcd.setCursor(columna, fila);
    lcd.print(" ");

    // Tecla 4: izquierda
    if (tecla == '4')
    {
      columna--;
    }

    // Tecla 6: derecha
    if (tecla == '6')
    {
      columna++;
    }

    // Tecla 2: arriba
    if (tecla == '2')
    {
      fila--;
    }

    // Tecla 8: abajo
    if (tecla == '8')
    {
      fila++;
    }

    // Limites horizontales del LCD: columnas 0 a 15
    if (columna < 0)
    {
      columna = 0;
    }

    if (columna > 15)
    {
      columna = 15;
    }

    // Limites verticales del LCD: filas 0 y 1
    if (fila < 0)
    {
      fila = 0;
    }

    if (fila > 1)
    {
      fila = 1;
    }

    // Dibuja la pelota en la nueva posicion
    lcd.setCursor(columna, fila);
    lcd.write(byte(0));
  }
}