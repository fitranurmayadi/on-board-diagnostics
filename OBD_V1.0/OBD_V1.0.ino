#include "bt.h"
#include "gsm.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define pinBaterai A0
int ready_to_send;
char data_to_serial[50];
char data_to_lcd_0[16];
char data_to_lcd_1[16];

void setup() {
  lcd.init();
  lcd.backlight();
  int battVoltage = analogRead(pinBaterai);
  if(battVoltage <100){
    lcd_print("CATU DAYA SIM", "BELUM DINYALAKAN ");
    delay(2000);
    lcd_print("Nyalakan Saklar", "Disamping");
  }
  while(battVoltage < 100){
    delay(100);
    battVoltage = analogRead(pinBaterai);
  }
  delay(500);
  lcd_print("Daya Telah", "  Mengalir");
  delay(2000);
  lcd_print("BT INIT ....", " ");
  init_bt();
  Serial.begin(38400);
  Serial1.begin(38400);  //Serial untuk gps
  Serial2.begin(38400);  //Serial untuk gsm
  Serial3.begin(38400);  //Serial untuk bluetooth


  delay(5000);
  lcd_print("CEK GSM", "MENUNGGU........");
  cek_gsmdata_available();

  delay(1000);
  
  lcd_print("INIT GPRS", "MENUNGGU........");
  
  init_gprs_gsm();

  lcd_print("CONNECT TO OBD", "MENUNGGU........");
  send_to_bt("at+init", 1000, DEBUG_IS);
  lcd_print("INITIALITING",data_bt);
  send_to_bt("at+disc", 5000, DEBUG_IS);
  send_to_bt("at+pair=0,0,111111,10", 2000, DEBUG_IS);
  lcd_print("PAIRING",data_bt);
  send_to_bt("at+bind=0,0,111111", 1000, DEBUG_IS);
  lcd_print("BINDING",data_bt);
  send_to_bt("at+link=0,0,111111", 3000, DEBUG_IS);
  lcd_print("LINKING",data_bt);
  
  obd_bus_init("ATZ", 1000, DEBUG_IS);
  obd_bus_init("ATSP0", 1000, DEBUG_IS);
  obd_bus_init("ATE0", 1000, DEBUG_IS);
  lcd_print("BUS INIT",data_bt);
  //obd_bus_init("0100", 1000, DEBUG_IS);
  lcd_print("BUS INIT",data_bt);
  lcd_print("BT IS OK", " ");
  ready_to_send = 0; 

}

void loop() {
  read_pid_param("0104", 1000, DEBUG_IS );
  data_PID[0] = (A * 100) / 255;
  read_pid_param("0105", 1000, DEBUG_IS );
  data_PID[1] = A - 40;
  read_pid_param("010B", 1000, DEBUG_IS );
  data_PID[2] = A;
  read_pid_param("010C", 1000, DEBUG_IS );
  data_PID[3] = ((256 * A) + B) / 4 ;
  read_pid_param("010D", 1000, DEBUG_IS );
  data_PID[4] = A ;
  read_pid_param("010E", 1000, DEBUG_IS );
  data_PID[5] = (A / 2) - 64 ;
  read_pid_param("010F", 1000, DEBUG_IS );
  data_PID[6] = A - 40;
  read_pid_param("0111", 1000, DEBUG_IS );
  data_PID[7] = (A * 100) / 255;
  sprintf(data_to_serial, "data : %d %d %d %d %d %d %d %d", data_PID[0], data_PID[1], data_PID[2], data_PID[3], data_PID[4], data_PID[5], data_PID[6], data_PID[7]);
  Serial.println(data_to_serial);
  sprintf(data_to_lcd_0, "DATA: SUHU: %d", data_PID[1]);
  sprintf(data_to_lcd_1, "S: %d RPM : %d", ready_to_send ,data_PID[3]);
  lcd_print(data_to_lcd_0, data_to_lcd_1);

  if (ready_to_send >= 3) {
    send_to_internet(data_PID[0], data_PID[1], data_PID[2], data_PID[3], data_PID[4], data_PID[5], data_PID[6], data_PID[7]);
  } else {
    ready_to_send++;
  }
}


void lcd_print(String baris1, String baris2) {
  delay(50);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(baris1);
  lcd.setCursor(0, 1); lcd.print(baris2);
}
