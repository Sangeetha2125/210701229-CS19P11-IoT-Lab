#define BLYNK_TEMPLATE_ID "TMPL358S56nvi"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "2Ee_rmUV3s8ktY8nOlwTjrhjeTLkfsTG";
char ssid[] = "OPPO A31";
char pass[] = "sangoppoa31";

#define gasSensor 34 
#define buzzer 2
#define light 4
#define trigger 33
#define echo 32
#define relay 5

#define tankHeight 14

void gasSensorMonitor()
{
  int gasLevel = analogRead(gasSensor);
  gasLevel = map(gasLevel,0,4095,0,100);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);
  Blynk.virtualWrite(V7, gasLevel);

  if(gasLevel>=50){
    digitalWrite(buzzer,HIGH);
    Blynk.logEvent("gas_leakage","Alert! Gas leak detected in the kitchen. Turn off the gas cylinder immediately. Avoid using electrical devices. Ventilate the area. Stay safe!");
  }
  else{
    digitalWrite(buzzer,LOW);
  }
}

void waterLevelMonitor()
{
  long duration;
  int distance;
  const int thresholdDistance = 4;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  Blynk.virtualWrite(V0, tankHeight - distance);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance <= thresholdDistance) {
    digitalWrite(relay, LOW);
    Serial.println("Water level high: Pump OFF");
  } else {
    digitalWrite(relay, HIGH);
    Serial.println("Water level low: Pump ON");
  }
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(buzzer,OUTPUT);
  pinMode(light,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
}

void loop() {
  Blynk.run();
  gasSensorMonitor(); 
  waterLevelMonitor();
  delay(1000); 
}

BLYNK_WRITE(V1){
  bool value = param.asInt(); 
  if(value==0){
    digitalWrite(light,LOW);
  }
  else{
    digitalWrite(light,HIGH);
  }
}