int rightWheelsPin1 = 2;     
int rightWheelsPin2 = 3;     
int leftWheelsPin1 = 4;      
int leftWheelsPin2 = 5;      
int speedRight = 9;          
int speedLeft = 10;          

struct DischargePoint {
  float voltage;
  float capacity[20]; 
};

const float cRates[20] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 
                         1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};

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
  pinMode(rightWheelsPin1, OUTPUT);  
  pinMode(rightWheelsPin2, OUTPUT);  
  pinMode(leftWheelsPin1, OUTPUT); 
  pinMode(leftWheelsPin2, OUTPUT); 
  pinMode(speedRight, OUTPUT);      
  pinMode(speedLeft, OUTPUT);
  Serial.begin(9600);               
}

void moveForward(int speed) {
  analogWrite(speedRight, speed);    
  analogWrite(speedLeft, speed);     
  digitalWrite(rightWheelsPin1, HIGH);  
  digitalWrite(rightWheelsPin2, LOW);   
  digitalWrite(leftWheelsPin1, HIGH);   
  digitalWrite(leftWheelsPin2, LOW);    
}

float adcToVoltage(int raw) {
  return raw * (5.0 / 1023.0);                         
}

float voltage_calculation() {
  int raw = analogRead(A5);  
  float measuredVoltage = adcToVoltage(raw);  
  return measuredVoltage;                                
}

float current_calculation() {
  int raw = analogRead(A4);          
  float vshunt = adcToVoltage(raw);  
  float current = vshunt / 0.1;      
  return current;                    
}

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

float discharge_rate_calculation(float discharge_current) {
  float discharge_rate = discharge_current / 2.9; 
  return discharge_rate;
}

float get_soc_from_voltage(float voltage, float c_rate) {
  int voltage_idx = 0;
  switch ((int)(voltage * 10)) {
    case 42: voltage_idx = 0; break; 
    case 41: voltage_idx = 1; break; 
    case 40: voltage_idx = 2; break; 
    case 39: voltage_idx = 3; break; 
    case 38: voltage_idx = 4; break; 
    case 37: voltage_idx = 5; break; 
    case 36: voltage_idx = 6; break; 
    case 35: voltage_idx = 7; break; 
    case 34: voltage_idx = 8; break; 
    case 33: voltage_idx = 9; break; 
    case 32: voltage_idx = 10; break; 
    case 31: voltage_idx = 11; break; 
    case 30: voltage_idx = 12; break; 
    case 29: voltage_idx = 13; break; 
    case 28: voltage_idx = 14; break; 
    case 27: voltage_idx = 15; break; 
    case 26: voltage_idx = 16; break; 
    case 25: voltage_idx = 17; break; 
    default: voltage_idx = 0; break; 
  }

  int c_rate_idx = 0;
  switch ((int)(c_rate * 10)) {
    case 1: c_rate_idx = 0; break; 
    case 2: c_rate_idx = 1; break; 
    case 3: c_rate_idx = 2; break; 
    case 4: c_rate_idx = 3; break; 
    case 5: c_rate_idx = 4; break; 
    case 6: c_rate_idx = 5; break; 
    case 7: c_rate_idx = 6; break; 
    case 8: c_rate_idx = 7; break; 
    case 9: c_rate_idx = 8; break; 
    case 10: c_rate_idx = 9; break; 
    case 11: c_rate_idx = 10; break; 
    case 12: c_rate_idx = 11; break; 
    case 13: c_rate_idx = 12; break; 
    case 14: c_rate_idx = 13; break; 
    case 15: c_rate_idx = 14; break; 
    case 16: c_rate_idx = 15; break; 
    case 17: c_rate_idx = 16; break; 
    case 18: c_rate_idx = 17; break; 
    case 19: c_rate_idx = 18; break; 
    case 20: c_rate_idx = 19; break; 
    default: c_rate_idx = 0; break; 
  }

  float discharge_capacity = curve[voltage_idx].capacity[c_rate_idx];
  
  float soc_c_rate = (discharge_capacity / 2900.0) * 100.0;
  
  return soc_c_rate;
}

float discharging_remaining_colomb_counting(float initial_soc, float sum_current) {
  const float nominal_capacity = 2.9; 
  float charge_consumed = sum_current / 3600.0; 
  float soc_drop = (charge_consumed / nominal_capacity) * 100.0;
  float soc_t = initial_soc - soc_drop;
  return soc_t;
}

void loop() {
  static int cycle_count = 0; 
  static float initial_remaining_soc = 100.0; 
  static float voltage_rounded = 0.0;
  static float discharge_rate_rounded = 0.0;
  static float initial_discharged_soc = 0.0;
  static float remaining_capacity = 2900.0;
  static float current_discharge_capacity = 0.0;
  static float Battery_remaining_time = 0.0;
  static float Battery_remaining_time_minutes = 0.0;
  static float soc_remaining_coloumb_counting_for_10s = 100.0;
  static float soc_discharged_coloumb_counting_over_10s = 0.0;
  static float current_integral = 0.0; 
  static int measurement_count = 0; 

  moveForward(255); 
  
  float voltage = voltage_calculation();
  float current = current_calculation();

  current_integral += current * 1.0; 
  measurement_count++;

  if (cycle_count == 0) {
    float discharge_rate = discharge_rate_calculation(current);
    voltage_rounded = smartRound1Decimal(voltage);
    discharge_rate_rounded = smartRound1Decimal(discharge_rate);
    initial_discharged_soc = get_soc_from_voltage(voltage_rounded, discharge_rate_rounded);
    initial_remaining_soc = 100.0 - initial_discharged_soc;
    remaining_capacity = (initial_remaining_soc / 100.0) * 2900.0;
    current_discharge_capacity = current * 1.0 * 1000.0;
    Battery_remaining_time = remaining_capacity / current;
    Battery_remaining_time_minutes = Battery_remaining_time * 60.0;
  }

  if (cycle_count == 9) {
    soc_remaining_coloumb_counting_for_10s = discharging_remaining_colomb_counting(initial_remaining_soc, current_integral);
    soc_discharged_coloumb_counting_over_10s = 100.0 - soc_remaining_coloumb_counting_for_10s;
    current_integral = 0.0;
    measurement_count = 0;
  }

  if (cycle_count == 0) {
    Serial.print("Initial Discharged SoC(Voltage):");
    Serial.print(initial_discharged_soc,2);
    Serial.print("% \t\tInitial Remaining SoC(Voltage):");
    Serial.print(initial_remaining_soc,2);
    Serial.println();
  } else if (cycle_count == 9) {
    float drop = soc_discharged_coloumb_counting_over_10s - initial_discharged_soc;
    Serial.print("SoC Discharged over 10sec(Coulomb):");
    Serial.print(soc_discharged_coloumb_counting_over_10s,2);
    Serial.print("\tSoC Remaining after 10sec(Coulomb):");
    Serial.print(soc_remaining_coloumb_counting_for_10s,2);
    Serial.print("\tSoC Drop:");
    Serial.print(drop,2);
    Serial.println();
    Serial.println();
  }
  

  cycle_count++;

  if (cycle_count >= 10) {
    cycle_count = 0;
  }

  delay(1000); 
}