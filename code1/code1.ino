
#define KALIBRASI_PH false
#define NILAI_PH_KALIBRASI  4.01
#define SAMPLING_KALIBRASI  200

#define SensorPin A0
#define Offset   41.30785714 
#define k        -18.46428571                 
#define LED       13
#define samplingInterval  20
#define printInterval     800
#define ArrayLenth        40    // times of collection
#define TAMPIL_SERIAL     true  // tampilkan hasil di serial monitor
#define v_ear   0.0908 
#define ph_ear   0.16 


int pHArray[ArrayLenth];   // Menyimpan nilai rata2 hasil pembacaan sensor
int pHArrayIndex = 0;

void setup(void) {
  pinMode(LED, OUTPUT);
#if TAMPIL_SERIAL
  Serial.begin(9600);
  Serial.println("STARTED");
#endif

  
}

void loop(void) {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static double pHValue, voltage;

#if KALIBRASI_PH  // MODE KALIBRASI
  double voltKalibrasi = 0;
  uint16_t samplingKalibrasi = SAMPLING_KALIBRASI;
#if TAMPIL_SERIAL
  
#endif
  
  for (uint16_t i = 0; i < samplingKalibrasi; i++) {
    voltage = analogRead(SensorPin) * 5.0 / 1024;
    voltKalibrasi += voltage;
    delay(samplingInterval);
#if TAMPIL_SERIAL
   
#endif
  }
#if TAMPIL_SERIAL
  
#endif
  voltKalibrasi /= samplingKalibrasi;
  delay(2000);
#if TAMPIL_SERIAL
  
#endif
  
  while (1) {};
  #else // MODE EKSEKUSI
  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0;

    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    voltage = voltage + v_ear;
    pHValue = k * voltage + Offset;
    pHValue = pHValue + ph_ear;
    samplingTime = millis();
  }

  // Setiap 800 mili detik, print a hasil pembacaan, ubah nyala kondisi LED
  if (millis() - printTime > printInterval) {
#if TAMPIL_SERIAL
    Serial.print(" pH value: ");
    Serial.println(pHValue, 2);
#endif
    digitalWrite(LED, digitalRead(LED) ^ 1);
    printTime = millis();
  }
#endif
}

double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
#if TAMPIL_SERIAL
    //Serial.println("Error number for the array to avraging!/n");
#endif
    return 0;
  }

  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;      //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
}
