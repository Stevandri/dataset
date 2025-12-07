#include <WiFi.h>          
#include <FirebaseESP32.h> 
#define WIFI_SSID "Stev"
#define WIFI_PASSWORD "12341234"

//KONFIGURASI FIREBASE
#define FIREBASE_HOST "kendali-led-28d92-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "3iOUUPZrSOnYYNlzhjHeFwj6TvH64ayvpQr2CvuW"

#define LED_MERAH 18
#define LED_BIRU  19
#define LED_KUNING 21

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long previousMillis = 0;
const long interval = 500; 
bool yellowState = false;

String command = "";

void setup() {
  Serial.begin(115200);

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_BIRU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  
  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_BIRU, LOW);
  digitalWrite(LED_KUNING, LOW);

  //Koneksi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Terhubung! IP: ");
  Serial.println(WiFi.localIP());

  //Setup Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.test_mode = true; 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.getString(firebaseData, "/status_led")) {
    
    if (firebaseData.dataType() == "string") {
      command = firebaseData.stringData();
      
      static String lastCommand = "";
      if (command != lastCommand) {
        Serial.println("Perintah Baru: " + command);
        lastCommand = command;
      }
      
      
      if (command == "merah") {
        digitalWrite(LED_MERAH, LOW); 
        digitalWrite(LED_BIRU, HIGH);
        digitalWrite(LED_KUNING, HIGH);
      } 
      else if (command == "biru") {
        digitalWrite(LED_MERAH, HIGH);
        digitalWrite(LED_BIRU, LOW); 
        digitalWrite(LED_KUNING, HIGH);
      }
      else if (command == "diam") {
        digitalWrite(LED_MERAH, HIGH);
        digitalWrite(LED_BIRU, HIGH);
        
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          yellowState = !yellowState; 
          digitalWrite(LED_KUNING, yellowState);
        }
      }
      else {
        digitalWrite(LED_MERAH, HIGH);
        digitalWrite(LED_BIRU, HIGH);
        digitalWrite(LED_KUNING, HIGH);
      }
    }
  } else {
    
  }
}
