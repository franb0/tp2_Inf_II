#include <Wire.h>                 // Libreria necesaria para comunicacion I2C
#include <LiquidCrystal_I2C.h>     // Libreria para manejar el LCD con modulo I2C

// Se crea el objeto lcd.
// 0x20 es la direccion I2C encontrada con el scanner.
// 16 indica la cantidad de columnas.
// 2 indica la cantidad de filas.
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Pin donde esta conectado el boton.
// En este circuito el boton va entre D7 y GND.
const int boton = 7;

// Mensaje que se va a mostrar en la pantalla.
String mensaje = " Aprendiendo a Programar";

// Esta variable indica desde que letra del mensaje se empieza a mostrar.
int posicion = 0;

// Esta variable indica el sentido del desplazamiento.
// Si vale 1, avanza en un sentido.
// Si vale -1, avanza en el sentido contrario.
int direccion = 1;

// Guarda el estado anterior del boton.
// Se inicia en HIGH porque usamos INPUT_PULLUP.
// Con INPUT_PULLUP, el boton sin presionar se lee como HIGH.
int botonAntes = HIGH;

// Caracter personalizado para la flecha hacia la derecha.
// Cada fila representa una linea de puntos del caracter.
// El LCD permite crear caracteres de 5 columnas por 8 filas.
byte flechaDer[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

// Caracter personalizado para la flecha hacia la izquierda.
byte flechaIzq[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

void setup()
{
  // Inicializa el LCD.
  lcd.init();

  // Enciende la luz de fondo del LCD.
  lcd.backlight();

  // Configura el boton como entrada con resistencia interna.
  // Esto permite conectar el boton directamente entre el pin D7 y GND,
  // sin usar una resistencia externa.
  pinMode(boton, INPUT_PULLUP);

  // Carga los caracteres personalizados en la memoria del LCD.
  // El caracter 0 sera la flecha derecha.
  // El caracter 1 sera la flecha izquierda.
  lcd.createChar(0, flechaDer);
  lcd.createChar(1, flechaIzq);
}

void loop()
{
  // Lee el estado actual del boton.
  int botonAhora = digitalRead(boton);

  /*
    Como se usa INPUT_PULLUP:
    - Boton sin presionar: HIGH
    - Boton presionado: LOW

    Esta condicion detecta solamente el instante en que el boton
    pasa de no estar presionado a estar presionado.
    Asi se evita que una sola pulsacion cambie muchas veces el sentido.
  */
  if (botonAhora == LOW && botonAntes == HIGH)
  {
    // Invierte el sentido del desplazamiento.
    // Si direccion era 1, pasa a -1.
    // Si direccion era -1, pasa a 1.
    direccion = direccion * -1;

    // Pequena pausa para evitar el rebote mecanico del boton.
    delay(200);
  }

  // Guarda el estado actual del boton para compararlo en la proxima vuelta.
  botonAntes = botonAhora;

  // Limpia la pantalla antes de escribir el nuevo cuadro del desplazamiento.
  lcd.clear();

  // Ubica el cursor en la columna 0, fila 0.
  lcd.setCursor(0, 0);

  // Escribe la flecha correspondiente al sentido actual.
  if (direccion == 1)
  {
    lcd.write(byte(0));   // Muestra la flecha hacia la derecha.
  }
  else
  {
    lcd.write(byte(1));   // Muestra la flecha hacia la izquierda.
  }

  /*
    El LCD tiene 16 columnas.
    Como la flecha ocupa la primera columna,
    quedan 15 columnas disponibles para el mensaje.
  */
  for (int i = 0; i < 15; i++)
  {
    /*
      Se calcula que caracter del mensaje corresponde mostrar.

      posicion indica desde donde empieza el texto.
      i indica la columna que se esta escribiendo.
    */
    int indice = posicion + i;

    /*
      Si el indice supera el largo del mensaje,
      se vuelve al principio. Esto hace que el mensaje
      se repita continuamente.
    */
    if (indice >= mensaje.length())
    {
      indice = indice - mensaje.length();
    }

    /*
      Si el indice queda negativo,
      se vuelve al final del mensaje.
      Esto permite que el desplazamiento inverso funcione.
    */
    if (indice < 0)
    {
      indice = indice + mensaje.length();
    }

    // Imprime en el LCD el caracter correspondiente.
    lcd.print(mensaje[indice]);
  }

  // Actualiza la posicion para que en la proxima vuelta el texto se mueva.
  posicion = posicion + direccion;

  // Si la posicion pasa el final del mensaje, vuelve al inicio.
  if (posicion >= mensaje.length())
  {
    posicion = 0;
  }

  // Si la posicion queda antes del inicio, vuelve al final.
  if (posicion < 0)
  {
    posicion = mensaje.length() - 1;
  }

  // Pausa que controla la velocidad del desplazamiento.
  delay(300);
}