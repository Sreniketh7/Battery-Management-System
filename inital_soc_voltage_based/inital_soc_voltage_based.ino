// === Motor Driver Pin Assignments ===
int rightWheelsPin1 = 2;     // OUT1: Controls front right and rear right wheels
int rightWheelsPin2 = 3;     // OUT2: Controls front right and rear right wheels
int leftWheelsPin1 = 4;      // OUT3: Controls front left and rear left wheels
int leftWheelsPin2 = 5;      // OUT4: Controls front left and rear left wheels
int speedRight = 9;          // ENA: PWM speed control for front right and rear right wheels
int speedLeft = 10;          // ENB: PWM speed control for front left and rear left wheels

// === Discharge Curve Table Structure ===
struct DischargePoint {
  float voltage;
  float capacity[20]; // Capacities for C-rates from 0.1C to 2.0C
};

// Discharge rates corresponding to the table columns (0.1C to 2.0C)
const float cRates[20] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 
                         1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};

// Full discharge curve table
const DischargePoint curve[] = {
  {4.2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {4.1, {300, 250, 225, 212, 200, 175, 162, 138, 125, 150, 138, 125, 112, 106, 125, 113, 107, 104, 102, 100}},
  {4.0, {700, 600, 550, 525, 500, 475, 450, 425, 413, 400, 388, 375, 362, 350, 375, 362, 350, 344, 338, 300}},
  {3.9, {1100, 950, 875, 838, 800, 775, 750, 725, 688, 650, 613, 600, 587, 575, 575, 563, 550, 530, 515, 500}},
  {3.8, {1500, 1300, 1200, 1150, 1100, 1050, 1000, 950, 925, 900, 875, 850, 825, 813, 800, 775, 750, 725, 712, 700}},
  {3.7, {1900, 1650, 1525, 1463, 1400, 1325, 1263, 1200, 1175, 1150, 1125, 1100, 1075, 1025, 900, 900, 900, 900, 900, 900}},
  {3.6, {2300, 2000, 1850, 1775, 1700, 1550, 1475, 1400, 1375, 1400, 1325, 1250, 1200, 1150, 1100, 1075, 1050, 1025, 1013, 1100}},
  {3.5, {2650, 2300, 2125, 2038, 1950, 1800, 1725, 1650, 1600, 1650, 1475, 1400, 1350, 1325, 1300, 1275, 1250, 1225, 1212, 1300}},
  {3.4, {2900, 2550, 2375, 2288, 2200, 2075, 1963, 1850, 1725, 1850, 1650, 1550, 1500, 1475, 1450, 1425, 1400, 1375, 1362, 1450}},
  {3.3, {2900, 2750, 2575, 2488, 2400, 2275, 2175, 2075, 1837, 2000, 1800, 1700, 1650, 1625, 1600, 1575, 1550, 1525, 1512, 1600}},
  {3.2, {2900, 2850, 2700, 2625, 2550, 2438, 2338, 2238, 2025, 2150, 1950, 1850, 1800, 1775, 1750, 1725, 1700, 1675, 1662, 1700}},
  {3.1, {2900, 2900, 2800, 2750, 2700, 2588, 2488, 2388, 2194, 2300, 2100, 2000, 1950, 1925, 1900, 1875, 1850, 1825, 1812, 1800}},
  {3.0, {2900, 2900, 2850, 2825, 2800, 2675, 2575, 2475, 2262, 2450, 2225, 2125, 2075, 2050, 2025, 2000, 1975, 1950, 1937, 1900}},
  {2.9, {2900, 2900, 2875, 2863, 2850, 2713, 2613, 2513, 2294, 2550, 2300, 2200, 2150, 2125, 2100, 2075, 2050, 2025, 2012, 2000}},
  {2.8, {2900, 2900, 2875, 2863, 2850, 2738, 2644, 2550, 2325, 2650, 2375, 2275, 2225, 2200, 2175, 2150, 2125, 2100, 2087, 2100}},
  {2.7, {2900, 2900, 2875, 2863, 2850, 2763, 2675, 2588, 2369, 2750, 2450, 2350, 2300, 2275, 2250, 2225, 2200, 2175, 2162, 2200}},
  {2.6, {2900, 2900, 2875, 2863, 2850, 2788, 2706, 2625, 2412, 2850, 2525, 2425, 2375, 2350, 2325, 2300, 2275, 2250, 2237, 2300}},
  {2.5, {2900, 2900, 2875, 2863, 2850, 2800, 2712, 2625, 2438, 2900, 2575, 2475, 2425, 2400, 2375, 2350, 2325, 2300, 2287, 2350}}
};

void setup() {
  // Configure motor driver pins as outputs
  pinMode(rightWheelsPin1, OUTPUT);  // OUT1: Set front right and rear right wheel pin as output
  pinMode(rightWheelsPin2, OUTPUT);  // OUT2: Set front right and rear right wheel pin as output
  pinMode(leftWheelsPin1, OUTPUT); 
  pinMode(leftWheelsPin2, OUTPUT); 
  pinMode(speedRight, OUTPUT);      
  pinMode(speedLeft, OUTPUT);
  Serial.begin(9600);               
}

// Function to move all wheels forward continuously
void moveForward(int speed) {
  analogWrite(speedRight, speed);    // ENA: Set speed for right wheels
  analogWrite(speedLeft, speed);     // ENB: Set speed for left wheels
  digitalWrite(rightWheelsPin1, HIGH);  // OUT1 HIGH: Right wheels forward
  digitalWrite(rightWheelsPin2, LOW);   // OUT2 LOW: Right wheels forward
  digitalWrite(leftWheelsPin1, HIGH);   // OUT3 HIGH: Left wheels forward
  digitalWrite(leftWheelsPin2, LOW);    // OUT4 LOW: Left wheels forward
}

// === Function: Convert ADC reading to voltage ===
float adcToVoltage(int raw) {
  return raw * (5.0 / 1023.0);                         
}

// === Function: Voltage Calculation ===
float voltage_calculation() {
  int raw = analogRead(A5);  // Read ADC
  float measuredVoltage = adcToVoltage(raw);  // Convert to Vref scale
  return measuredVoltage;                                // Return calculated battery voltage
}

// === Function: Current Calculation ===
float current_calculation() {
  int raw = analogRead(A4);          // Read raw ADC value from current pin
  float vshunt = adcToVoltage(raw);  // Convert ADC to voltage across shunt resistor
  float current = vshunt / 0.1;      // Calculate current using Ohm’s law (I = V/R)
  return current;                    // Return calculated current
}

// === Function: Smart Round to 1 Decimal Place ===
float smartRound1Decimal(float value) {
  float scaled = value * 10.0;
  int base = (int)scaled;
  float fractional = scaled - base;

  if (fractional <= 0.5) {
    return base / 10.0;
  } else {
    return (base + 1) / 10.0;
  }
}

// === Function: Discharge Rate Calculation ===
float discharge_rate_calculation(float discharge_current) {
  float discharge_rate = discharge_current / 2.9; // Convert current to C-rate (assuming 2900mAh nominal capacity)
  return discharge_rate;
}

// === Function: Get SoC from Voltage and C-rate ===
float get_soc_from_voltage(float voltage, float c_rate) {
  // Find index of voltage using switch-case
  int voltage_idx = 0;
  switch ((int)(voltage * 10)) {
    case 42: voltage_idx = 0; break; // 4.2V
    case 41: voltage_idx = 1; break; // 4.1V
    case 40: voltage_idx = 2; break; // 4.0V
    case 39: voltage_idx = 3; break; // 3.9V
    case 38: voltage_idx = 4; break; // 3.8V
    case 37: voltage_idx = 5; break; // 3.7V
    case 36: voltage_idx = 6; break; // 3.6V
    case 35: voltage_idx = 7; break; // 3.5V
    case 34: voltage_idx = 8; break; // 3.4V
    case 33: voltage_idx = 9; break; // 3.3V
    case 32: voltage_idx = 10; break; // 3.2V
    case 31: voltage_idx = 11; break; // 3.1V
    case 30: voltage_idx = 12; break; // 3.0V
    case 29: voltage_idx = 13; break; // 2.9V
    case 28: voltage_idx = 14; break; // 2.8V
    case 27: voltage_idx = 15; break; // 2.7V
    case 26: voltage_idx = 16; break; // 2.6V
    case 25: voltage_idx = 17; break; // 2.5V
    default: voltage_idx = 0; break; // Default to 4.2V if no match
  }

  // Find index of C-rate using switch-case
  int c_rate_idx = 0;
  switch ((int)(c_rate * 10)) {
    case 1: c_rate_idx = 0; break; // 0.1C
    case 2: c_rate_idx = 1; break; // 0.2C
    case 3: c_rate_idx = 2; break; // 0.3C
    case 4: c_rate_idx = 3; break; // 0.4C
    case 5: c_rate_idx = 4; break; // 0.5C
    case 6: c_rate_idx = 5; break; // 0.6C
    case 7: c_rate_idx = 6; break; // 0.7C
    case 8: c_rate_idx = 7; break; // 0.8C
    case 9: c_rate_idx = 8; break; // 0.9C
    case 10: c_rate_idx = 9; break; // 1.0C
    case 11: c_rate_idx = 10; break; // 1.1C
    case 12: c_rate_idx = 11; break; // 1.2C
    case 13: c_rate_idx = 12; break; // 1.3C
    case 14: c_rate_idx = 13; break; // 1.4C
    case 15: c_rate_idx = 14; break; // 1.5C
    case 16: c_rate_idx = 15; break; // 1.6C
    case 17: c_rate_idx = 16; break; // 1.7C
    case 18: c_rate_idx = 17; break; // 1.8C
    case 19: c_rate_idx = 18; break; // 1.9C
    case 20: c_rate_idx = 19; break; // 2.0C
    default: c_rate_idx = 0; break; // Default to 0.1C if no match
  }

  // Get discharge capacity from table
  float discharge_capacity = curve[voltage_idx].capacity[c_rate_idx];
  
  // Calculate SoC based on C-rate: (Discharge_capacity / 2900) * 100
  float soc_c_rate = (discharge_capacity / 2900.0) * 100.0;
  
  return soc_c_rate;
}

void loop() {
  // Motor control: Active option
  moveForward(255);                          // Move forward continuously at full speed
  
  float voltage = voltage_calculation();     // Calculate battery voltage
  float current = current_calculation();     // Calculate current draw
  float discharge_rate = discharge_rate_calculation(current); // Calculate raw C-rate
  float voltage_rounded = smartRound1Decimal(voltage); // Round voltage to 1 decimal place
  float discharge_rate_rounded = smartRound1Decimal(discharge_rate); // Round C-rate to 1 decimal place
  float soc_discharged_voltage_based = get_soc_from_voltage(voltage_rounded, discharge_rate_rounded); // Calculate discharged SoC
  float soc_remaining_voltage_based = 100.0 - soc_discharged_voltage_based; // Calculate remaining SoC
  float remaining_capacity = (soc_remaining_voltage_based / 100.0) * 2900.0; // Calculate remaining capacity in mAh
  float current_discharge_capacity = current * 1.0 * 1000.0; // Capacity discharged in 1 hour (mAh)
  float Battery_remaining_time = remaining_capacity / current; // Time in hours
  float Battery_remaining_time_minutes = Battery_remaining_time * 60.0; // Convert to minutes

  Serial.print("\tVoltage: ");     // Print voltage label with tab
  Serial.print(voltage*2, 2); // Print rounded voltage with 2 decimal places
  Serial.print(" V\t");            // Print unit with tab
  Serial.print("\tCurrent: ");     // Print current label with tab
  Serial.print(current, 2);        // Print current with 2 decimal places
  Serial.print(" A\t");            // Print unit with tab
 // Serial.print("\tC-rate: ");      // Print C-rate label with tab
  //Serial.print(discharge_rate_rounded, 2); // Print rounded C-rate with 2 decimal places
  Serial.print("\tSoC Discharged: "); // Print discharged SoC label with tab
  Serial.print(soc_discharged_voltage_based, 2); // Print discharged SoC with 2 decimal places
  Serial.print(" %\t");            // Print unit with tab
  Serial.print("\tSoC Remaining: "); // Print remaining SoC label with tab
  Serial.print(soc_remaining_voltage_based, 2); // Print remaining SoC with 2 decimal places
  Serial.print(" %\t");            // Print unit with tab
  //Serial.print("\tRemaining Capacity: "); // Print remaining capacity label with tab
  //Serial.print(remaining_capacity, 2); // Print remaining capacity with 2 decimal places
 // Serial.print(" mAh\t");          // Print unit with tab
  Serial.print("\tRemaining Time: "); // Print remaining time label with tab
  Serial.print(Battery_remaining_time_minutes, 2); // Print remaining time in minutes with 2 decimal places
  Serial.println(" min");          // Print unit with newline
  delay(1000);
}