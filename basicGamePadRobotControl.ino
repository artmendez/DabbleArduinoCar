// Robot controlado por Bluetooth usando Dabble y un módulo L298N
// por A. Méndez (mendez@tec.mx)
// configuración inicial para controlar un robot con Dabble y un módulo Bluetooth
// es necesario tener la librería Dabble instalada en el IDE de Arduino
// para hacer esto, ve a Sketch -> Incluir Librería -> Administrar Bibliotecas
// y busca "Dabble" para instalarla. Haz lo mismo con "SoftwareSerial" si no la tienes ya instalada.
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
// incluimos la librería SoftwareSerial para la comunicación Bluetooth.
// si la línea marca un error, asegúrate de tener instalada la librería SoftwareSerial correctamente.
#include <SoftwareSerial.h> 
// incluimos la librería Dabble para usar sus funciones.
// si la línea marca un error, asegúrate de tener instalada la librería Dabble correctamente.
#include <Dabble.h>

// aquí se definen los pines de los motores
// en el L298N IN1 e IN2 controlan el motor izquierdo
// IN3 e IN4 controlan el motor derecho
// vamos a usar los pines 2, 3, 4 y 5 del Arduino
// y los IN1, IN2, IN3 e IN4 respectivamente en el L298N (asegúrate de conectar los pines correctamente)
// del Arduino al L298N, el pin 2 va a IN1, el pin 3 a IN2, el pin 4 a IN3 y el pin 5 a IN4
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// esta es la función de configuración inicial se ejecuta una vez al iniciar el programa o cuando el Arduino se reinicia
void setup() {
  // configura la velocidad de transmisión Bluetooth. 
  //Dabble usa 9600 por defecto. asigna los pines 9 y 10 para la comunicación SoftwareSerial.
  //Esto es necesario, pues si se usaran el 0 y 1, sería necesario desconectar el módulo Bluetooth cada vez que se quiera subir
  //un nuevo programa al Arduino.
  Dabble.begin(9600, 7, 8); 
  pinMode(IN1, OUTPUT); // configura los pines de los motores como salidas
  pinMode(IN2, OUTPUT); // configura los pines de los motores como salidas
  pinMode(IN3, OUTPUT); // configura los pines de los motores como salidas
  pinMode(IN4, OUTPUT); // configura los pines de los motores como salidas
}

// después de haner ejecutado la función setup(), la función loop() se ejecuta una y otra vez de manera infinita
// o hasta que el Arduino se apague, se reinicie, se cargue un nuevo programa, se quede sin energía, 
// sea desconectado, sea destruído o el universo colapse. XD
void loop() {
  Dabble.processInput(); // procesa la entrada de Dabble (necesario para que Dabble funcione correctamente)

  // controla el robot según la entrada del GamePad
  if (GamePad.isUpPressed()) forward(); // si se presiona el botón de arriba, avanza hacia adelante llamando a la función forward()
  else if (GamePad.isDownPressed()) backward(); // si se presiona el botón de abajo, retrocede llamando a la función backward()
  else if (GamePad.isLeftPressed()) left(); // si se presiona el botón de la izquierda, gira a la izquierda llamando a la función left()
  else if (GamePad.isRightPressed()) right(); // si se presiona el botón de la derecha, gira a la derecha llamando a la función right()
  else stopMotors(); // si no se presiona ningún botón, detiene los motores llamando a la función stopMotors()
}

// El módulo L298N usa cuatro pines para controlar dos motores de corriente continua (DC).
// cada motor tiene dos pines de control (IN1 e IN2 para el motor izquierdo, IN3 e IN4 para el motor derecho).
// dependiendo de cómo se configuren estos pines (HIGH o LOW), los motores giran en diferentes direcciones o se detienen.
// Si ambos pines de un motor están en LOW, el motor se detiene.
// Si ambos pines de un motor están en HIGH, el motor también se detiene (frenado).
// Si el primer pin está en HIGH y el segundo en LOW, el motor gira en una dirección.
// Si el primer pin está en LOW y el segundo en HIGH, el motor gira en la dirección opuesta.
// Si algún motor gira en dirección opuesta a lo que esperas, intercambia la conexión entre
// los pines IN1 e IN2 o IN3 e IN4 en el L298N para corregir la dirección.
// asegúrate que el jumper de ENA y ENB en el L298N esté colocado para habilitar los motores.
// también asegúrate que el jumper de alimentación del L298N esté colocado si estás alimentando el L298N directo de la batería.
// esto hará que el L298N funcione correctamente y proporcionará suficiente corriente a los motores, evitando que el Arduino se reinicie debido a una caída de voltaje.
// también podrás disponer de 5V de la terminal 3 de salida del L298N para alimentar el Arduino.

// a continuación se definen las funciones para controlar el movimiento del robot.

// función para mover el robot hacia adelante
// funciona activando los pines IN1 e IN3 y desactivando IN2 e IN4
// lo que hace que ambos motores giren hacia adelante
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// funcion para mover el robot hacia atrás
// funciona activando los pines IN2 e IN4 y desactivando IN1 e IN3
// lo que hace que ambos motores giren hacia atrás
void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// función para girar el robot a la izquierda
// funciona activando IN2 e IN3 y desactivando IN1 e IN4
// lo que hace que el motor izquierdo gire hacia atrás y el motor derecho gire hacia adelante
void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// función para girar el robot a la derecha
// funciona activando IN1 e IN4 y desactivando IN2 e IN3
// lo que hace que el motor izquierdo gire hacia adelante y el motor derecho gire hacia atrás
void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// función para detener ambos motores
// funciona desactivando todos los pines IN1, IN2, IN3 e IN4
// lo que hace que ambos motores se detengan
// también puedes activar ambos pines (HIGH) de un motor (por ejemplo, IN1 e IN2) para frenar el motor rápidamente
// digamos que si ambos son LOW, el motor se detiene de manera pasiva (rueda libre)
// y si ambos son HIGH, el motor se detiene de manera activa (frenado activo)
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// el módulo L298N puede recibir señales PWM en los pines ENA y ENB para controlar la velocidad de los motores
// pero en este ejemplo simple no se está utilizando esa funcionalidad
// para controlar la velocidad, tendrías que conectar los pines ENA y ENB a pines PWM del Arduino
// El Arduino UNO tiene pines PWM en los pines 3, 5, 6, 9, 10 y 11, puedes identificarlos por el símbolo "~" al lado del número del pin
// la señal PWM permite variar la velocidad del motor ajustando el ciclo de trabajo (duty cycle) de la señal
// pero para mantener este ejemplo simple, solo estamos usando señales digitales HIGH y LOW para controlar el movimiento básico del robot
// si deseas explorar el control de velocidad, puedes investigar sobre la función analogWrite() en Arduino y cómo usarla con el L298N.
// en la librería Dabble, también puedes usar el joystick del GamePad para controlar la velocidad de los motores.
// puedes buscar en la documentación de Dabble cómo obtener los valores del joystick y mapearlos a señales PWM para controlar la velocidad de los motores..
// esto puede ser un buen ejercicio para mejorar tus habilidades de programación y control de hardware con Arduino y Dabble.
// también puedes experimentar con diferentes configuraciones y mejoras en el código para adaptar el robot a tus necesidades específicas.
// ¡diviértete programando y construyendo tu robot controlado por Bluetooth con Dabble y Arduino!
// :)    /A. Méndez