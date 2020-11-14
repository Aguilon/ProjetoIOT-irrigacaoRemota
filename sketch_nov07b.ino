int valvula = 8;
  
void setup() {
  pinMode(valvula, OUTPUT); // declara válvula como saída
  pinMode(A0, INPUT);
}

void loop() {
 // Leitura do sensor no pino A0 e armazenamento em SensorValue
 Serial.begin(9600);
 int sensorValue = analogRead(A0);
 Serial.print("Valor do sensor: ");
 Serial.println(sensorValue);
 
 int umidade = map(sensorValue, 1023, 0, 0, 200);
 Serial.print("Umidade em: ");
 Serial.print(umidade);
 Serial.println("%");
 // verifica se valor de leitura está acima de 600
 if (sensorValue > 200){
   digitalWrite(valvula,LOW);
   delay(1000);
   digitalWrite(valvula,HIGH);
   
 }
  else{
 // Interrompe a passagem de água
   digitalWrite(valvula,HIGH);
   delay(1000);
  }
  Serial.println("------");
  delay(3000);
}
