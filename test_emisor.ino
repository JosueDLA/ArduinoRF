/*
 *Programa para usar emisor 433Mhz
*/

#include <VirtualWire.h>

const int gnd = 11;   //Pin 11 como tierra para el modulo RF
const int vcc = 10;   //Pin 12 como fuente de alimentacion para el modulo RF
const int data = 9;  //Pin 10 como salida para el modulo RF

const int led = 13;   //se utiliza el led conectado al pin 13 para saber cuando se envia un mensaje

void setup()
{
  Serial.begin(9600);  
  Serial.println("Emisor: Setup");

  //Se configura el pin para funcionar como VCC
  pinMode(vcc, OUTPUT);
  digitalWrite(vcc, HIGH);

  //Se configura el pin para funcionar como GND
  pinMode(gnd, OUTPUT);
  digitalWrite(gnd, LOW);

  // Se inicializa el RF
  vw_setup(2000); // velocidad: Bits per segundo
  vw_set_tx_pin(data); 
}

void loop()
{
  String txtMsg;
  
  while(Serial.available() > 0){
    txtMsg = Serial.readString();
    Serial.print(txtMsg);

    char buf[txtMsg.length()];
    txtMsg.toCharArray(buf, txtMsg.length());

    digitalWrite(13, true); // Parpadeamos  el led al enviar 
    vw_send((uint8_t *)buf, txtMsg.length());
    vw_wait_tx(); //Esperamos hasta que el mensaje se envie
    digitalWrite(13, false);
    delay(2000);
  }

  /*const char *msg = "Hola Mundo";
  
  digitalWrite(13, true); // Parpadeamos  el led al enviar 
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie
  digitalWrite(13, false);
  delay(2000);*/
}
