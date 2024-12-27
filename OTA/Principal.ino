#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ShiftOutMega.h> // multiplexador saida
#include <NTPClient.h>

  
// _________________________________________________________________________________

//     DEFINIÇÕES WIFI     


#ifndef STASSID
#define STASSID "Costa22"
#define STAPSK "pimenta403"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;


// _________________________________________________________________________________

//    PINOS


// PINOS DIGITAIS

byte 
d0 = 16, // Pino de LATCH conectado à porta D0 (GPIO16) - SR 12 (MULTIPLEXADOR DE SAIDA)
d1 = 5,  // SCL [VAZIO]
d2 = 4,  // SDA [VAZIO]
d3 = 0,  // Pino de DADOS conectado à porta D3 (GPIO5)  - SR 14 (MULTIPLEXADOR DE SAIDA)
d4 = 2,  // Pino de CLOCK conectado à porta D4 (GPIO4)  - SR 11 (MULTIPLEXADOR DE SAIDA)
d5 = 14, // pino s0 (MULTIPLEXADOR DE ENTRADA)
d6 = 12, // pino s1 (MULTIPLEXADOR DE ENTRADA)
d7 = 13, // pino s2 (MULTIPLEXADOR DE ENTRADA)
d8 = 15; // pino s3 (MULTIPLEXADOR DE ENTRADA)

byte pinos[9] = {16, 5, 4, 0, 2, 14, 12, 13, 15};

// _________________________________________________________________________________

//     MULTIPLEXADOR SAÍDA 


// MULTIPLEXADOR SHIFT REGISTER COM 8 PORTAS DE SAIDA, EXPANSÍVEL
// CI SN74HC595N
int quantidade_de_multiplexadores = 3;
ShiftOutMega MUX_SAIDA(d0, d3, d4, quantidade_de_multiplexadores);


// _________________________________________________________________________________

//     MULTIPLEXADOR ENTRADA


byte portaMUX_Entrada[16][4] 
{
  {0,0,0,0}, // entrada 0
  {0,0,0,1}, // entrada 1
  {0,0,1,0}, // entrada 2
  {0,0,1,1}, // entrada 3
  {0,1,0,0}, // entrada 4
  {0,1,0,1}, // entrada 5
  {0,1,1,0}, // entrada 6
  {0,1,1,1}, // entrada 7
  {1,0,0,0}, // entrada 8
  {1,0,0,1}, // entrada 9
  {1,0,1,0}, // entrada 10
  {1,0,1,1}, // entrada 11
  {1,1,0,0}, // entrada 12
  {1,1,0,1}, // entrada 13
  {1,1,1,0}, // entrada 14
  {1,1,1,1}  // entrada 15
};

void Leitura_MUX();             // FUNÇÃO  DE LEITURA -> USAR AO INVES DE AnalogRead()

int PinosMUXEntrada[4]{4,3,2,1}, // array de endereçamento dos pinos do multiplexador de saída 
DadosSensores[16][10],           // Armazena as 10 últimas leituras de cada um dos 16 sensores
Valor[16];                       // Valor Atual 



// _________________________________________________________________________________

//      DATA & HORA     


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "clock.isc.org", -10800);
String DiaSemana[7]={"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
String MesExtenso[12]={"Janeiro", "Fevereiro", "March", "April", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

time_t epochTime;

String HorarioFormatado;
int Hora;
int Minuto;
int Segundo;
String DiaDaSemana;

int DiaDoMes;
int Mes;
int Ano;
String NomeDoMes;
String Data;

//DateTime Agora(Ano, Mes, DiaDoMes, Hora, Minuto, Segundo);

void DADOSHORA();



// _________________________________________________________________________________

//     FUNÇÕES

void PiscaLED();
void ExibeValores();
//void FILTRA_OSCILACAO();



void setup()
{

  Serial.begin(115200);
  
  // Declaração de pinos digitais
  for (int i = 0; i < sizeof(pinos); i++)
  {
    pinMode(pinos[i], OUTPUT);
  }
  pinMode(ledPin, OUTPUT);    // Declaração do LED IMBUTIDO
  pinMode(A0, INPUT);         // Declaração do único pino analógico

  
  setupWIFI();                // CONEXÃO WIFI
  setupOTA();                  // CONFIGURAÇÕES OTA
  PrimeiraAquisicao();          // PRIMEIRA AQUISIÇÃO DE DADOS
  timeClient.begin();           // DATA E HORA

}


void loop() 
{
  ArduinoOTA.handle();      // COMUNICAÇÃO OTA
  timeClient.update();      // BUSCA DADOS DE DATA E HORA EM UM SERVIDOR ONLINE
  DADOSHORA();              // ATRIBUI DADOS DE DATA E HORA A VARIÁVEIS
  Leitura_MUX();            // ATUALIZA OS DADOS DOS SENSORES
  PiscaLED();               // PISCA LED IMBUTIDO PARA ACUSAR FUNCIONAMENTO
  ExibeValores();           // MOSTRA VALORES NO MONITOR SERIAL (comentar após código finalizado)

  delay(2000);
}


// _________________________________________________________________________________

//     FUNÇÃO DE EXIBIÇÃO DE VALORES NO MONITOR SERIAL

void ExibeValores()
{
  for(int p=0; p<=15; p++) // TESTE
  {
    Serial.print(String(p) + ": ");
    for(int k = 0; k <= 9; k++)
    {
      Serial.print(String(DadosSensores[p][k]));
      if(k < 9)
      {
        Serial.print(", ");
      }

      if(k == 9)
      {
        Serial.print(" -------- MÉDIA : " + String(Valor[p]));
      }
    }
    Serial.println("");
  }
  Serial.println("");
  Serial.println("Horario: "  + String(HorarioFormatado));
  Serial.println("Dia Da Semana: "  + String(DiaDaSemana));
  Serial.println("Data: " + String(Data));
  Serial.println("");

}

// _________________________________________________________________________________

//     FUNÇÃO DE ATRIBUIÇÃO DE VALORES DE HORA A VARIAVEIS

void DADOSHORA()
{
  epochTime = timeClient.getEpochTime();
  HorarioFormatado = timeClient.getFormattedTime();
  Hora = timeClient.getHours();
  Minuto = timeClient.getMinutes();
  Segundo = timeClient.getSeconds();
  DiaDaSemana = DiaSemana[timeClient.getDay()];
 
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  DiaDoMes = ptm->tm_mday;
  Mes = ptm->tm_mon+1;
  Ano = ptm->tm_year+1900;
  NomeDoMes = MesExtenso[Mes-1];
  Data = String(DiaDoMes) + "/" + String(Mes) + "/" + String(Ano);  //Print complete date:

  //Agora = Agora(Ano, Mes, DiaDoMes, Hora, Minuto, Segundo);
}

// _________________________________________________________________________________
