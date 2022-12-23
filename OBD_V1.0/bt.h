#define bt_key  12

boolean DEBUG_IS = true;
String data_obd_hex[6];
int data_PID[10];
char dataAB[100];
long int A, B;
String data_bt = " ";
String response = "";

String getValue(String data_to_parse, char separator, int index); // definisikan fungsi ini
void obd_bus_init(String command, int time_delay, boolean DEBUG);
void send_to_bt(String command, int time_delay, boolean DEBUG);
void read_pid_param(String command, int time_delay, boolean DEBUG);

void init_bt() {
  pinMode(bt_key, OUTPUT);
  digitalWrite(bt_key, HIGH);
  delay(1000);
}

void obd_bus_init(String command, int time_delay, boolean DEBUG) {
  data_bt = "";
  if (DEBUG) Serial.println(command);
  Serial3.println(command);
  Serial3.flush();
  delay(time_delay);
  while (Serial3.available() <= 0);
  char s = "";
  while (s != '>') {
    if (Serial3.available()) {
      char c = Serial3.read();
      s=c;
      Serial.print(s);
      data_bt += c; 
    }
  }
  if (DEBUG)Serial.println(data_bt);
}

void send_to_bt(String command, int time_delay, boolean  DEBUG) {
  data_bt = "";
  if (DEBUG) Serial.println(command);
  Serial3.println(command);
  Serial3.flush();
  delay(time_delay);
  while (Serial3.available() <= 0);
  char s = "";
  while (s != '\r')
  {
    if (Serial3.available()) {
      char c = Serial3.read();
      s = c;
      data_bt += c; 
    }
  }
  if (DEBUG)Serial.println(data_bt);
}


void read_pid_param(String command, int time_delay, boolean  DEBUG) {
  String data_obd = "";
  int  data_ready = 0;
  A = 0, B = 0;
  if (DEBUG)Serial.println(command);
  Serial3.println(command);
  Serial3.flush();
  delay(time_delay);
  while (Serial3.available() <= 0);
  char c= "";
  while (c != '>')
  {
    c = Serial3.read();
    data_obd += c;
  }
  if (DEBUG) {
    Serial.print(" data pid =  ");
    Serial.print(data_obd);
    Serial.print(" panjang data = ");
    Serial.println(data_obd.length());
  }

  data_obd_hex[0] = getValue(data_obd, ' ', 0);
  data_obd_hex[1] = getValue(data_obd, ' ', 1);
  data_obd_hex[2] = getValue(data_obd, ' ', 2);
  data_obd_hex[3] = getValue(data_obd, ' ', 3);
  data_obd_hex[4] = getValue(data_obd, ' ', 4);
  //data_obd_hex[5] = getValue(data_obd, ' ', 5);

  A =  strtol(data_obd_hex[4].c_str(), NULL, 16);
  if (data_obd.length() > 15) {
    data_obd_hex[5] = getValue(data_obd, ' ', 5);
    B = strtol(data_obd_hex[5].c_str(), NULL, 16);
  }
  if (DEBUG) {
    Serial.print(" data hex 1 = ");
    Serial.print(data_obd_hex[1]);
    Serial.print(" data hex 2 = ");
    Serial.print(data_obd_hex[2]);
    Serial.print(" data hex 3 = ");
    Serial.print(data_obd_hex[3]);
    Serial.print(" data hex 4 = ");
    Serial.print(data_obd_hex[4]);
    Serial.print(" data hex 5 = ");
    //Serial.print(data_obd_hex[5]);
    //Serial.print(" data hex 6 = ");
    Serial.println(data_obd_hex[5]);
  }
}

String getValue(String data_to_parse, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data_to_parse.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data_to_parse.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data_to_parse.substring(strIndex[0], strIndex[1]) : "";
}
