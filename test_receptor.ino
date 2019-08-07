/*
 * Programa para usar receptor 433Mhz
 * Utilizando el modulo ENC28J60 para la distribucion de la informacion
*/

#include <VirtualWire.h>
#include <EtherCard.h>

#define STATIC 0    // Cambiar a 1 para deshabilitar el DHCP

#if STATIC
//Direccion IP
static byte myip[] = { 192,168,1,200 };
//Direccion del Getaway
static byte gwip[] = { 192,168,1,1 };
#endif

// ethernet mac address - debe ser unica en la red
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

// tcp/ip para enviar y recivir buffer
byte Ethernet::buffer[500];
BufferFiller bfill;

const int gnd = 4;   //Pin 4 como tierra para el modulo RF
const int vcc = 3;   //Pin 3 como fuente de alimentacion para el modulo RF
const int data = 2;  //Pin 2 como salida para el modulo RF
String txtMsg = "@User~Placeholder~ ";

static word homePage(String mensaje) {

  char temp[mensaje.length()+1];
  mensaje.toCharArray(temp, mensaje.length());
  
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR("$S\n"), temp);
  return bfill.position();
}

void setup()
{
  Serial.begin(9600);  // Debugging only
  Serial.println("Receptor: Setup");

  //Se configura el pin para funcionar como VCC
  pinMode(vcc, OUTPUT);
  digitalWrite(vcc, HIGH);

  //Se configura el pin para funcionar como GND
  pinMode(gnd, OUTPUT);
  digitalWrite(gnd, LOW);

  // Cambiar 'SS' al pin SLV, si no se uso la coneccion por defecto
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");

  #if STATIC
    ether.staticSetup(myip, gwip);
  #else
    if(!ether.dhcpSetup())
      Serial.println("Fallo el DHCP");
  #endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  // Se inicializa el RF
  vw_set_rx_pin(data);
  vw_setup(2000);         // velocidad: Bits per segundo
  vw_rx_start();          // Se inicia como receptor
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  //verificamos si hay un dato valido en el RF
  if (vw_get_message(buf, &buflen)) {
    int i;
  
    //verificamos si hay un dato valido en el RF
    Serial.print("Mensaje: ");
    txtMsg = "";
    
    for (i = 0; i < buflen; i++) {
        //Serial.print((char)buf[i]);
        txtMsg+=(char)buf[i];
    }
    Serial.println(txtMsg);
  }

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage(txtMsg)); // send web page data
    
  //memset(buf, 0, sizeof(buf));
  //memset(buflen, 0, sizeof(buflen));
}
