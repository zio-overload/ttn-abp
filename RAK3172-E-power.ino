
#define ON_SEN1_PIN PA7
#define ON_SEN2_PIN PB7
#define ON_SEN3_PIN PA5
#define ON_SEN4_PIN PB2
#define ON_SEN5_PIN PA4

#define ON_RGB_PIN PA15
#define DIN_RGB_PIN PB4

#define INT_SEN1_PIN PA1
#define INT_SEN2_PIN PB6
#define INT_SEN3_PIN PA6
#define INT_SEN4_PIN PB5
#define INT_SEN5_PIN PA8

#define ADC_BAT_PIN PA10
#define VM_BAT_PIN PA9
#define WKUP_PIN PA0

#define RX2_PIN PA3
#define TX2_PIN PA2

void setup()
{
  Serial.begin(115200);
  Serial.println("RAKwireless RAK3172-E Example");
  Serial.println("------------------------------------------------------");
  pinMode(ON_SEN1_PIN, OUTPUT);
  pinMode(ON_SEN2_PIN, OUTPUT);
  pinMode(ON_SEN3_PIN, OUTPUT);
  pinMode(ON_SEN4_PIN, OUTPUT);
  pinMode(ON_SEN5_PIN, OUTPUT);

  pinMode(DIN_RGB_PIN, OUTPUT);

  pinMode(INT_SEN1_PIN, INPUT);
  pinMode(INT_SEN2_PIN, INPUT);
  pinMode(INT_SEN3_PIN, INPUT);
  pinMode(INT_SEN4_PIN, INPUT);
  pinMode(INT_SEN5_PIN, INPUT);

  pinMode(ON_RGB_PIN, OUTPUT);

  pinMode(VM_BAT_PIN, INPUT);
  // pinMode(ADC_BAT_PIN, INPUT_PULLUP);
  analogRead(ADC_BAT_PIN);
  pinMode(WKUP_PIN, INPUT);

 /*
  pinMode(VM_BAT_PIN, OUTPUT);
  pinMode(ADC_BAT_PIN, OUTPUT);
  pinMode(WKUP_PIN, OUTPUT);
*/
 
  digitalWrite(ON_SEN1_PIN, LOW);
  digitalWrite(ON_SEN2_PIN, LOW);
  digitalWrite(ON_SEN3_PIN, LOW);
  digitalWrite(ON_SEN4_PIN, LOW);
  digitalWrite(ON_SEN5_PIN, LOW);

  digitalWrite(ON_RGB_PIN, HIGH);

  digitalWrite(DIN_RGB_PIN, LOW);

  // api.system.sleep.cpu(6000);
 // Serial.end();
 // pinMode(RX2_PIN, INPUT);
 // pinMode(TX2_PIN, INPUT);
}

void loop()
{
  /* Destroy this busy loop and use timer to do what you want instead,
   * so that the system thread can auto enter low power mode by api.system.lpm.set(1); */
  api.system.scheduler.task.destroy();
  //api.system.sleep.all(2000);
}
