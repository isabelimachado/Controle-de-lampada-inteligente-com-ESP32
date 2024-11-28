//codigo com LDR - luminosidade 21/11/224

//LED - Azul > DI - Roxo > VCC - Cinza > GND

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h> 

#define PIN 27   
#define BUZZER_PIN 23    
#define LDR_PIN 34    
#define DHTPIN 25
#define NUMLED 8      

Adafruit_NeoPixel pixels(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "Augusto";
const char *password = "zzpp8888";

WebServer server(80);
DHT dht(DHTPIN, DHT11);

bool lampState = false;
bool alertState = false;
bool lumiState = false;

const float tempLimite = 28.0;
const int lumiLimite = 35;
float temperaturaAtual = 0.0; 
unsigned long ultimoTempoNota = 0;  
int indiceNota = 0;                 
unsigned long duracaoNota = 0;      
bool tocandoNota = false; 


int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle de Lâmpadas Inteligentes</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f0f0; margin: 0; padding: 0; }
        h1 { color: #333; margin-top: 20px; }
        .container { display: flex; justify-content: center; align-items: center; flex-direction: column; padding: 20px; }
        .btn { background-color: #4CAF50; color: white; padding: 15px 30px; margin: 10px; border: none; border-radius: 5px; font-size: 18px; cursor: pointer; transition: background-color 0.3s ease; }
        .btn:hover { background-color: #45a049; }
        .btn:active { background-color: #3e8e41; }
        .info { background-color: #2196f3; }
        .footer { margin-top: 50px; font-size: 14px; color: #666; }
        #temperatura-text { font-size: 24px; margin-top: 15px; color: #333; }
        #temperatura-alta-text { font-size: 20px; margin-top: 15px; color: #F00; }
        #luminosidade-text { font-size: 24px; margin-top: 15px; color: #333; }
        .slider { -webkit-appearance: none; appearance: none; width: 300px; height: 15px; background: #ddd; outline: none; opacity: 0.8; transition: opacity 0.2s; margin: 0 auto; }
        .slider:hover { opacity: 1; }
        .slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; background: #4CAF50; cursor: pointer; border-radius: 50%; }
        .slider::-moz-range-thumb { width: 25px; height: 25px; background: #4CAF50; cursor: pointer; border-radius: 50%; }
    </style>
</head>
<body>
    <h1>Controle de Lâmpadas Inteligentes</h1>
    <div class="container">
        <h3>Temperatura</h3>
        <div id="temperatura-text">Carregando temperatura...</div>
        <div id="temperatura-alta-text"></div>
        <h3>Luminosidade</h3>
        <div id="luminosidade-text">Carregando luminosidade...</div>
        <br><br>
        <button class="btn" onclick="ligar()">Ligar Lâmpada</button>
        <button class="btn" onclick="desligar()">Desligar Lâmpada</button>
        <button class="btn info" onclick="modoFesta()">Modo Festa</button>
    </div>

    <div class="footer">
        <p>Desenvolvido por Isabeli e Augusto • Informática - 2137</p>
    </div>

    <script>
        var muitoQuente = false;
        function alerta(temp) {
          document.getElementById('temperatura-text').innerText = temp + ' °C';
          if (temp >= 30.0 && !muitoQuente) {
            muitoQuente = true;
            document.getElementById('temperatura-alta-text').innerText = 'Temperatura muito alta!';
            alert("A lâmpada está muito quente! Espere ela esfriar.");
          } else if (temp <= 30.0) {
            muitoQuente = false;
            document.getElementById('temperatura-alta-text').innerText = '';
          }
        }
        function buscarTemperatura() {
            fetch('/temperatura')
                .then(response => response.text())
                .then(data => {
                  alerta(data);
                });
        }

        function buscarLuminosidade() {
            fetch('/luminosidade')
                .then(response => response.text())
                .then(data => {
                    // Exibe a luminosidade em formato de porcentagem
                    document.getElementById('luminosidade-text').innerText = data;
                });
        }

        function ligar() {
            fetch('/ligar');
        }

        function desligar() {
            fetch('/desligar');
        }

        function modoFesta() {
            fetch('/modoFesta');
        }

        setInterval(buscarTemperatura, 500);
        setInterval(buscarLuminosidade, 500);
    </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pixels.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }

  Serial.println("Conectado!");

  Serial.print("IP do servidor: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("lampada-inteligente")) {
    Serial.println("Servidor mDNS iniciado");
  }

  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });
  
  server.on("/temperatura", []() {
    if (temperaturaAtual >= tempLimite && !alertState) {
      for (int i = 0; i < NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      }
      pixels.show();
      alertState = true;
    }
    else if (temperaturaAtual < tempLimite && alertState) {
      for (int i = 0; i < NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      alertState = false; 
    }
    server.send(200, "text/plain", String(temperaturaAtual));
  });

  server.on("/luminosidade", []() {
    if (alertState) {  
      server.send(200, "text/plain", "Função de luminosidade desativada devido à temperatura alta.");
      return;  
    }

    int ldrValue = analogRead(LDR_PIN); 
    int luminosidade = constrain(map(ldrValue, 35, 1400, 0, 100), 0, 100);
    String luminosidadeStr = String(luminosidade) + "% de luminosidade";

    if (luminosidade < lumiLimite && !lumiState) {
      for (int i = 0; i < NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 0));  
      }
      pixels.show();
      lumiState = true;  
    }
    else if (luminosidade >= lumiLimite && lumiState) {
      for (int i = 0; i < NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
      }
      pixels.show();
      lumiState = false;  
    }

    server.send(200, "text/plain", luminosidadeStr);
  });
  
  server.on("/ligar", []() {
    float temp = dht.readTemperature();
    if(temp < tempLimite && !alertState) {
      for (int i = 0; i < NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      }
      pixels.show();
      lampState = true;
      server.send(200, "text/plain", "Lâmpada ligada");
    }
  });

  server.on("/desligar", []() {
    if(!alertState) {
      for (int i = 0; i < NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      lampState = false;
      server.send(200, "text/plain", "Lâmpada desligada");
    }
  });
  
  server.on("/modoFesta", []() {
    if (!alertState) {
      tocandoNota = true;  
      indiceNota = 0;         
      server.send(200, "text/plain", "Modo festa ativado");
    }
  });

  server.begin();
  dht.begin();
}

void loop() {
  server.handleClient(); 
  static unsigned long ultimoTempo = 0;  

  if (millis() - ultimoTempo >= 500) { 
    ultimoTempo = millis();
    float temp = dht.readTemperature();
    if (!isnan(temp)) {
      temperaturaAtual = temp;  
    }
  }
  
  if (tocandoNota) {
    if (millis() - ultimoTempoNota >= duracaoNota) {  
      if (indiceNota < 77) {  
        duracaoNota = 1000 / noteDurations[indiceNota];
        tone(BUZZER_PIN, melody[indiceNota], duracaoNota);
        int pausaEntreNotas = duracaoNota * 1.20;

        led_festa(random(255), random(255), random(255)); 

        ultimoTempoNota = millis();  
        indiceNota++;  
        duracaoNota = pausaEntreNotas;  
      }
      else {
        tocandoNota = false;  
        led_festa(0, 0, 0);  
        noTone(BUZZER_PIN);  
      }
    }
  }  
}

void led_festa( int r, int g, int b) {
  for(int i=0; i<NUMLED; i++) { 
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();  
  }
}

