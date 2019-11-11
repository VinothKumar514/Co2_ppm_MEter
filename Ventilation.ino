int gassensor=A0;
int sensorvalue=-0;

void setup() {
  pinMode(gassensor,INPUT);
  Serial.begin(9600);
}
  
void loop() {
sensorvalue=analogRead(gassensor);
//Serial.println("The Gas Value is=");
Serial.println(sensorvalue);
delay(1000);

}
