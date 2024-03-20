int analogread_res = 12;          // ADC bit depth (see datasheet)
float aref = 20.14;                // ADC reference voltage (see datasheet) 6

// positive rail supply:
int pin_voltage_pos = A1;         // ADC pin
int voltage_devider_R1_pos = 110; // in kOhm R5
int voltage_devider_R2_pos = 47;  // in kOhm  R4
int analogread_value_pos;
float V_out_pos;
float V_in_pos;

// negative rail supply:
float V_in_neg;

// total supply voltage:
int pin_voltage_total = A0;         // ADC pin
int voltage_devider_R1_total = 110; // in kOhm   R7
int voltage_devider_R2_total = 110;  // in kOhm  R6
int analogread_value_total;
float V_out_total;
float V_in_total;

void setup() {
  Serial.begin(115200);
  int x = analogRead(pin_voltage_pos);  //discard this reading
  int y = analogRead(pin_voltage_total); //discard this reading
}

void loop() {
  analogread_value_pos = analogRead(pin_voltage_pos);
  V_out_pos = (analogread_value_pos / pow(2, analogread_res)) * aref;
  V_in_pos = V_out_pos / voltage_devider_R2_pos * (voltage_devider_R1_pos + voltage_devider_R2_pos);

  analogread_value_total = analogRead(pin_voltage_total);
  V_out_total = (analogread_value_total / pow(2, analogread_res)) * aref;
  V_in_total = V_out_total / voltage_devider_R2_total * (voltage_devider_R1_total + voltage_devider_R2_total);

  V_in_neg = V_in_pos - V_in_total;

  Serial.print("positive voltage: ");
  Serial.print(V_in_pos, 3);
  Serial.print("   negative voltage: ");
  Serial.println(V_in_neg, 3);

  delay(100);
}
