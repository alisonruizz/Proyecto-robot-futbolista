#include <SoftwareSerial.h>

// Definimos una clase para el control del motor
class MotorController {
  private: //las siguientes variables y métodos son privados, es decir, solo accesibles dentro de la propia clase.
    int pinA1, pinA2, pinB1, pinB2;
  
  public: // las siguientes variables y métodos son públicos, es decir, accesibles desde fuera de la clase.
    MotorController(int pa1, int pa2, int pb1, int pb2) //Constructor que inicializa los pines, se llama cuando se crea un objeto de MotorController
      : pinA1(pa1), pinA2(pa2), pinB1(pb1), pinB2(pb2) {} 
    //Lista de inicialización del constructor que asigna los valores de los parámetros (pa1, pa2, pb1, pb2) a las variables miembro (pinA1, pinA2, pinB1, pinB2).

    void setup() { //Configura los pines como salidas
      pinMode(pinA1, OUTPUT);
      pinMode(pinA2, OUTPUT);
      pinMode(pinB1, OUTPUT);
      pinMode(pinB2, OUTPUT);
    }
    
    void avanzar() {
      digitalWrite(pinA1, HIGH);
      digitalWrite(pinA2, LOW);
      digitalWrite(pinB1, HIGH);
      digitalWrite(pinB2, LOW);
      delay(100);
    }
    
    void retroceder() {
      digitalWrite(pinA1, LOW);
      digitalWrite(pinA2, HIGH);
      digitalWrite(pinB1, LOW);
      digitalWrite(pinB2, HIGH);
      delay(100);
    }
    
    void girarDerecha() {
      digitalWrite(pinA1, HIGH);
      digitalWrite(pinA2, LOW);
      digitalWrite(pinB1, LOW);
      digitalWrite(pinB2, HIGH);
      delay(100);
    }
    
    void girarIzquierda() {
      digitalWrite(pinA1, LOW);
      digitalWrite(pinA2, HIGH);
      digitalWrite(pinB1, HIGH);
      digitalWrite(pinB2, LOW);
      delay(100);
    }
    
    void detener() {
      digitalWrite(pinA1, LOW);
      digitalWrite(pinA2, LOW);
      digitalWrite(pinB1, LOW);
      digitalWrite(pinB2, LOW);
      delay(100);
    }
};

// Definimos los apuntadores a funciones
typedef void (MotorController::*MotorAction)();

// Define un tipo MotorAction que es un apuntador a una 
//función miembro de MotorController que no toma argumentos y no retorna nada.

MotorController motor(5, 6, 9, 10); //Crea una instancia/objeto motor de 
//MotorController inicializando los pines correspondientes a los motores.

SoftwareSerial bluetooth(2, 3); // TX, RX

int bt; // Variable bt como entero

void setup() {
  bluetooth.begin(9600);
  motor.setup(); //Llama al método setup de la instancia
  // motor para configurar los pines de los motores.
}

void loop() {
  static MotorAction action = nullptr;
  // Declara una variable estática action de tipo MotorAction 
  //inicializada a puntero nulo.
  //La palabra clave static hace que action retenga su valor entre iteraciones del bucle.

  if (bluetooth.available()) { // BLUETOOTH DISPONIBLE
    bt = bluetooth.read(); // leer y guardar datos bluetooth
    
    switch (bt) { //Evalúa el valor de bt y asigna el apuntador a función action según el comando recibido
      case 'F': action = &MotorController::avanzar; break; //Asigna la dirección del método avanzar a action.
      case 'B': action = &MotorController::retroceder; break; //Asigna la dirección del método retroceder a action.
      case 'R': action = &MotorController::girarDerecha; break; //Asigna la dirección del método girarDerecha a action.
      case 'L': action = &MotorController::girarIzquierda; break; //Asigna la dirección del método girarIzquierda a action.
      case 'S': action = &MotorController::detener; break; //Asigna la dirección del método detener a action.
      default: action = nullptr; break; //Si no coincide con ninguno de los casos, action se establece a nullptr.
    }

    if (action) { //Si action no es nullptr, llama a la función correspondiente
      (motor.*action)(); //corresponde al llamado de un método miembro de la clase MotorController utilizando un apuntador a función 
      //Utiliza el apuntador a función para llamar al método correspondiente en la instancia motor.
    }
  }
}

