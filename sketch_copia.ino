#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "AGG";
const char* password = "flash1800";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[80];
char notifica[80];
int umidade;
int value = 0;
String inString = "";

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  inString="";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    inString+=(char)payload[i];
  }
}

int valvula = D5;
int sensor = A0;
int sensorValue = 3;
  
void setup() {
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(valvula, OUTPUT); // declara válvula como saída
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Serial.begin(9600);
   
  sensorValue = analogRead(sensor);
  umidade = map(sensorValue, 1023, 0, 0, 200);
  
  Serial.print("Valor do sensor: "); // Testes
  Serial.println(sensorValue);
  Serial.print("Umidade em: ");
  Serial.print(umidade);
  Serial.println("%");
   
  // verifica se valor de leitura está acima de 600
  if (sensorValue > 600){
    digitalWrite(valvula,HIGH); // Libera água por 1s
    snprintf (notifica, 75, "%d", 1);
    client.publish("oicAguilon/molhado", notifica); 
    Serial.print("aaaaaaaaaaaaaaaaaa");
  } else{
     digitalWrite(valvula,LOW);// Interrompe a passagem de água
     delay(1000);
    }

  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    snprintf (msg, 75, "%d", umidade);
    client.publish("oicAguilon/umidade", msg);
  }
  Serial.println("------");
  delay(3000);
  }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("oicAguilon/hello", "ESP Conectado");
      // ... and resubscribe
      client.subscribe("oicAguilon/umidade");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(10000);
    }
  }
}
