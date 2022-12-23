String data_gsm;

void send_to_gsm(String command, int timedelay) {
  data_gsm = "";
  Serial.println(command);
  Serial2.println(command);
  Serial2.flush();
  delay(timedelay);
  while (Serial2.available() < 0);
  char c ="";
  while (Serial2.available())
  {
    c = Serial2.read();
    data_gsm += c;
  }
  Serial.println(data_gsm);
}

void cek_gsmdata_available() {
  send_to_gsm("AT+CSQ", 1000);
  send_to_gsm("AT+COPS?", 1000);
}

void init_gprs_gsm() {
  Serial.println("Mempersiapkan GPRS");
  send_to_gsm("AT+CGATT=1", 1000);
  send_to_gsm("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", 1000);
  send_to_gsm("AT+SAPBR=3,1,\"APN\",\"internet\"", 1000); //set apn provider yang digunakan
  send_to_gsm("AT+SAPBR=1,1", 1000);
  send_to_gsm("AT+HTTPINIT", 1000);
}

void send_to_internet(int para1, int para2, int para3, int para4, int para5, int para6, int para7, int para8) {

  String url;
  url = "AT+HTTPPARA=\"URL\",\"http://iotcarmonitor.id/insert.php?idmobil=98";
  url += "&04=";
  url += para1;
  url += "&05=";
  url += para2;
  url += "&0B=";
  url += para3;
  url += "&0C=";
  url += para4;
  url += "&0D=";
  url += para5;
  url += "&0E=";
  url += para6;
  url += "&0F=";
  url += para7;
  url += "&11=";
  url += para8;
  url += "\"\r\n";
  send_to_gsm(url, 1000);

  send_to_gsm("AT+HTTPACTION=0", 1000); //pada http action: 0=GET, 1=POST, 2=HEAD
  send_to_gsm("AT+HTTPREAD", 1000);     //baca respon dari server
  //send_to_gsm("AT+HTTPTERM", 1000);     // akhiri sambungan http
}
