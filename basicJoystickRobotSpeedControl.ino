// Robot controlado por Bluetooth usando Dabble, Joystick y PWM para velocidad variable
// Modificado para incluir control de velocidad por A. Méndez (mendez@tec.mx)
//
// Esta versión utiliza el módulo "Joystick" de la app Dabble en lugar de solo las flechas.
// Esto nos permite controlar no solo la dirección, sino qué tan rápido se mueven los motores.
//
// REQUISITO DE HARDWARE:
// Para controlar la velocidad, necesitamos usar pines PWM (Pulse Width Modulation).
// El L298N tiene pines "ENA" y "ENB". Normalmente tienen un jumper plástico que los mantiene siempre
// a máxima velocidad. Para este código, RETIRA esos jumpers y conecta:
// ENA -> Pin 6 de Arduino (Motor Izquierdo)
// ENB -> Pin 9 de Arduino (Motor Derecho)

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

// Definición de pines para los motores
// Mantenemos IN1-IN4 para la dirección (pines digitales)
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// Pines para control de velocidad (PWM)
// Los pines PWM en Arduino Uno suelen tener un símbolo "~" (3, 5, 6, 9, 10, 11)
#define ENA 6 // Control de velocidad Motor Izquierdo
#define ENB 9 // Control de velocidad Motor Derecho

void setup() {
  // configura la velocidad de transmisión Bluetooth. 
  //Dabble usa 9600 por defecto. asigna los pines 9 y 10 para la comunicación SoftwareSerial.
  //Esto es necesario, pues si se usaran el 0 y 1, sería necesario desconectar el módulo Bluetooth cada vez que se quiera subir
  //un nuevo programa al Arduino.
  Dabble.begin(9600, 9, 10); 
  // inicia la comunicación Bluetooth

  // Configurar pines de dirección como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configurar pines de velocidad (PWM) como salida
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  // Esta función procesa la entrada de Dabble
  Dabble.processInput(); 

  // Lógica del Joystick para control de velocidad y dirección
  // El Joystick de Dabble nos da valores en el eje X (izquierda/derecha) y Y (arriba/abajo).
  // Los valores van aproximadamente de -7 a 7.
  int xVal = GamePad.getXaxisData();
  int yVal = GamePad.getYaxisData();

  // Para controlar un robot tipo tanque (diferencial) con un solo joystick, usamos una mezcla:
  // Velocidad Izquierda = Y + X
  // Velocidad Derecha   = Y - X
  // Esto hace que al mover el joystick en diagonal, un motor gire más rápido que el otro, logrando un giro suave.
  int leftSpeed = yVal + xVal;
  int rightSpeed = yVal - xVal;

  // Ahora necesitamos "mapear" (convertir) estos valores pequeños (-7 a 7) 
  // al rango que entiende el motor (0 a 255 para la velocidad PWM).
  // Multiplicamos por 30 para escalar aprox de 7 a 210, o usamos map().
  // Usaremos una constante de multiplicación simple para mantener el código limpio.
  leftSpeed = leftSpeed * 35;  // 7 * 35 = 245 (cerca del máximo 255)
  rightSpeed = rightSpeed * 35;

  // control del motor izquierdo
  if (leftSpeed > 0) {
    // Si el valor es positivo, vamos hacia adelante
    moveLeftMotor(leftSpeed, true); 
  } else if (leftSpeed < 0) {
    // Si es negativo, vamos hacia atrás (pasamos el valor positivo usando abs())
    moveLeftMotor(abs(leftSpeed), false);
  } else {
    // Si es cero, frenamos
    stopLeftMotor();
  }

  // control del motor derecho
  if (rightSpeed > 0) {
    moveRightMotor(rightSpeed, true);
  } else if (rightSpeed < 0) {
    moveRightMotor(abs(rightSpeed), false);
  } else {
    stopRightMotor();
  }
}

// Funciones auxiliares para mover los motores con velocidad variable:
// Estas funciones están separadas para cada motor para que el código principal (loop) sea más fácil de leer.
// Estas funciones reciben la velocidad (0-255) y la dirección (true=adelante, false=atrás).

// función para mover el motor izquierdo
void moveLeftMotor(int speed, boolean forward) {
  // Nos aseguramos que la velocidad no pase de 255 (el límite de Arduino)
  if (speed > 255) speed = 255; 

  // Escribimos la velocidad en el pin ENA
  analogWrite(ENA, speed);

  // Configuramos la dirección
  // Si forward es true, IN1=HIGH e IN2=LOW (adelante)
  // Si es false, IN1=LOW e IN2=HIGH (atrás)
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

//función para mover el motor derecho
void moveRightMotor(int speed, boolean forward) {
  if (speed > 255) speed = 255;

  // Escribimos la velocidad en el pin ENB
  analogWrite(ENB, speed);

  // Configuramos la dirección
  // Si forward es true, IN3=HIGH e IN4=LOW (adelante)
  // Si es false, IN3=LOW e IN4=HIGH (atrás)
  if (forward) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

//función para detener el motor izquierdo
void stopLeftMotor() {
  analogWrite(ENA, 0); // Velocidad 0
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

//función para detener el motor derecho
void stopRightMotor() {
  analogWrite(ENB, 0); // Velocidad 0
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Explicación adicional sobre el uso de PWM:
// PWM (Modulación por Ancho de Pulso) simula un voltaje análogo encendiendo y apagando el pin muy rápido.
// analogWrite(pin, 0) apaga el pin (0% de potencia).
// analogWrite(pin, 127) mantendría el pin encendido el 50% del tiempo, haciendo que el motor gire a la mitad de su potencia.
// analogWrite(pin, 255) es 100% de potencia.
// Recuerda que si la velocidad es muy baja (ej. menos de 60), es posible que los motores zumben pero no tengan fuerza para mover el robot.
// Si eso pasa, puedes añadir una "zona muerta" en el código para ignorar valores muy bajos.

// ¡Espero que disfrutes tu robot con control de velocidad variable! 
// Ahora podrás hacer maniobras mucho más precisas y suaves. XD
// /A. Méndez




