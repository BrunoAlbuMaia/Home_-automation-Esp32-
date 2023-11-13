#include <WiFi.h>
#include <Ultrasonic.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int pinoRele = 15; // Pino manda sinal para o relé ligar ou desligar a lâmpada
const int pinoSensorMovimento = 2; // Pino que recebe dados do sensor ultrassônico
const int pinoTrigSensorUltrassonico = 13; // Pino Trig do Sensor Ultrassônico
const int pinoEchoSensorUltrassonico = 12; // Pino Echo do Sensor Ultrassonico

const char* baseUrl = "http://189.107.98.120:9000/";

class GestorDeLampada {
public:
  GestorDeLampada(const char* baseUrl, int pinoTrigSensorUltrassonico, int pinoEchoSensorUltrassonico, int pinoRele,int pinoSensorMovimento)
      : baseUrl(baseUrl), pinoTrigSensorUltrassonico(pinoTrigSensorUltrassonico), pinoEchoSensorUltrassonico(pinoEchoSensorUltrassonico), pinoRele(pinoRele), pinoSensorMovimento(pinoSensorMovimento) {
    estadoSensorMovimento = false;
  }

  

  void begin() {
    ConnectionWifi();
    pinMode(pinoRele, OUTPUT);
  }

  void loop() {
    String AtivacaoSensorMovimento = getFromAPI("home/VerificandoEquipamentos/", "AtivacaoSensorMovimento");
    bool sensorAtivado = (AtivacaoSensorMovimento == "true");
    
    int leitura = digitalRead(pinoSensorMovimento);
    
    controlarLampadaPorMovimento(sensorAtivado, leitura);

    String CapturarMedidaCaixa = getFromAPI("home/VerificandoEquipamentos/", "CapturarMedidaCaixa");
    bool SensorCapturarMedidaCaixa = (AtivacaoSensorMovimento == "true");
    if (CapturarMedidaCaixa == "true"){
      Serial.println("VOU MEDIR A DISTANCIA");
      medirDistancia();
    }
    
  }

private:
  const char* baseUrl;
  int pinoTrigSensorUltrassonico;
  int pinoEchoSensorUltrassonico;
  int pinoRele;
  int pinoSensorMovimento;
  bool estadoSensorMovimento;

float medirDistancia() {
    // Configurar o sensor ultrassônico
    Ultrasonic ultrasonic(pinoTrigSensorUltrassonico, pinoEchoSensorUltrassonico);

    // Medir a distância
    float distancia = ultrasonic.distanceRead();
    String url = "/hidro/ValorMedido/" + String(distancia);

    updateAPIH(url);
    delay(1000);
}


void ConnectionWifi() {
    String WifiName = "Wokwi-GUEST";
    String WifiPassword = "";

    Serial.print("Conectando-se ao Wi-Fi");
    WiFi.begin(WifiName, WifiPassword, 6);

    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
    }
    Serial.println(" Conectado!");
  }


String getFromAPI(String endpoint, String field) {
    if (WiFi.status() == WL_CONNECTED) {
      
      HTTPClient http;
      String url = baseUrl + endpoint;
      http.begin(url);
      int httpCode = http.GET();

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString(); 
        DynamicJsonDocument jsonDoc(1024); // Tamanho do documento pode variar dependendo da complexidade do JSON 
        deserializeJson(jsonDoc, payload); 

        const char* jsonData = jsonDoc[0][field];
        if (jsonData) {
          return String(jsonData);
        } else {
          return "Error";
        }

      } else {
        Serial.print("Falha na requisição GET: ");
        Serial.println(http.errorToString(httpCode).c_str());
      }

      http.end();
    }

    return "";
  }


void updateAPI(int idEquipamento, const char* key, const char* value) 
  {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = baseUrl;
    url += "home/AtualizarEquipamentos/";
    url += idEquipamento;


    // Crie um objeto JSON para os dados a serem enviados
    DynamicJsonDocument jsonDoc(256);  // Tamanho pode variar dependendo dos seus dados
    jsonDoc[key] = value;

    String jsonData;
    serializeJson(jsonDoc, jsonData);

    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.PATCH(jsonData);

    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Atualização bem-sucedida!");
    } else {
      Serial.print("Falha na atualização: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  }

void updateAPIH(String endpoint) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = baseUrl + endpoint;  // Concatene a URL base com o endpoint

    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.PATCH("");

    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Atualização bem-sucedida!");
    } else {
      Serial.print("Falha na atualização: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

void controlarLampadaPorMovimento(bool sensorAtivado, int leituraSensor) {
    
    if (sensorAtivado) {
      
      if (leituraSensor == HIGH) {

        // Verifica o status do sensor de movimento global
        if (estadoSensorMovimento == false) {
          Serial.println("Movimento detectado");
          updateAPI(1, "StatusLampada", "true");
          onOffRele();
          estadoSensorMovimento = true;
        }
        
        // Tempo durante o qual a lâmpada permanece acesa
        delay(5000); // Tempo de acionamento da lâmpada
      } else {
        // Desliga a lâmpada através do relé

        if (estadoSensorMovimento == true) {
          Serial.println("Sem movimento");
          updateAPI(1, "StatusLampada", "false");
          onOffRele();
          estadoSensorMovimento = false;
        }
        
      }
    } else if(sensorAtivado == 0) {
      //CASO O SENSOR NAO TIVER ATIVADOR, SERÁ USANDO O MODO LAMPADA
      onOffRele();
    }
  }


  void onOffRele(){
    String StatusLampada = getFromAPI("home/VerificandoEquipamentos/", "StatusLampada");
    if (StatusLampada == "true"){
      Serial.println("Estou no true do RELE");
      digitalWrite(pinoRele, HIGH);
      // Mantem a luz desligada
    }else{
      Serial.println("Estou no false do RELE");
       digitalWrite(pinoRele, LOW);
      //mantem a luz ligada
    }
  }



};

GestorDeLampada gestorLampada(baseUrl, pinoTrigSensorUltrassonico, pinoEchoSensorUltrassonico,pinoRele,pinoSensorMovimento); // Passe os pinos como argumentos.
void setup() {
  Serial.begin(9600);
  gestorLampada.begin();
}

void loop() {
  gestorLampada.loop();
}
