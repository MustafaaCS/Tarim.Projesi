#define LCD_RS_PIN PIN_D0  //LCD NIN RS PININI D0 PINI OLARAK BELIRLEDIM
#define LCD_RW_PIN PIN_D1  //LCD NIN RW PININI D1 PINI OLARAK BELIRLEDIM
#define LCD_ENABLE_PIN PIN_D2  //LCD NIN ENABLE PININI D2 PINI OLARAK BELIRLEDIM
#define LCD_DATA4 PIN_D3  //LCD NIN DATA4 PININI D3 PINI OLARAK BELIRLEDIM
#define LCD_DATA5 PIN_D4  //LCD NIN DATA5 PININI D4 PINI OLARAK BELIRLEDIM
#define LCD_DATA6 PIN_D5  //LCD NIN DATA6 PININI D5 PINI OLARAK BELIRLEDIM
#define LCD_DATA7 PIN_D6  //LCD NIN DATA7 PININI D6 PINI OLARAK BELIRLEDIM

#include <16F877A.h> //16F877A KUTUPHANESINI EKLEDIIM
 #device adc=10 //ADC DEGERIMI AYARLADIM
#fuses xt,wdt,noprotect,nobrownout,nolvp,noput,nowrt,nodebug,nocpd   //DENETLEYICI KONFIGRASYON AYARLARINI YAPTIM   
#use delay(clock = 8000000) //GECIKME FONKSIYONU ICIN KULLANILICAK OSILATOR FREKANSINI AYARLADIM
#use rs232(baud=9600,parity=N,uart1,bits=8)    // RS232 protokolünün 9600 BIT/S BAUD HIZINDA OLACAGINI  PARITY BITININ OLMADIGINI AYARLADIM

#include <lcd.c>  //LCD.C DOSYASINI TANIMLADIM
#include <stdio.h> //STDIO KUTUPHANESINI TANIMLADIM
#include <string.h> //STRING KUTUPHANESINI TANIMLADIM
#use fast_io(B) //PORT YONLENDIRME KOMUTLARININ B ICIN GECERLI OLDUGUNU TANIMLADIM
#define HAVA_SICAKLIK_NEM PIN_B4  //HAVA SICAKLIK VE NEM SENSORUNUN PININI TANIMLADIM
 #define TOPRAK_SICAKLIK_NEM PIN_B5 //TOPRAK SICAKLIK VE NEM SENSORUNUN PININI TANIMLADIM
 #define ISITICI PIN_B2 //ISITICININ PININI TANIMLADIM

#define RTC_SCLK pin_c2 //RTC_SCLK PININI TANIMLADIM
#define RTC_IO   pin_c3 //RTC_IO PININI TANIMLADIM
#define RTC_RST  pin_c1 //RTC_RST PININI TANIMLADIM
#include "ds1302.c" //DS1302 RTC MI TANIMLADIM
 #include <math.h> //MATH KUTUPHANEMI TANIMLADIM

char isik_siddeti[]= "000 Kandela/m2"; //ISIK SIDDETINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char ruzgar_bilgi[]= "Ruzgar Degeri"; //SABIT BIR IP YE GONDERECEGIM RUZGAR DEGERI BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
char ruzgar_yon[]="Ruzgar Yonu"; //SABIT BIR IP YE GONDERECEGIM RUZGAR YONU BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
int32 adc_deger,adc_deger10; //adc_deger ve adc_deger degiskenimi tanimladim
char saatc[] = "  :  :  "; //SAATI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char tarihc[] = "  /  /20  "; //TARIHI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char haftanin_gunu[11]; //HAFTANIN GUNUNU KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char Hava_sicaklik[] = "SICAKLIK = 00.0 C  "; //HAVANIN SICAKLIGINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char Hava_nem[] = "NEM  = 00.0 %  "; //HAVANIN NEMINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char Toprak_sicaklik[] = "SICAKLIK = 00.0 C  "; //TOPRAGIN SICAKLIGINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char Toprak_nem[] = "NEM  = 00.0 %  "; //TOPRAGIN NEMINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
char haftanin_gunu1[]="GUN"; //SABIT BIR IP YE GONDERECEGIM GUN BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
char time1[] = "SAAT"; //SABIT BIR IP YE GONDERECEGIM SAAT BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
char calendar1[]="TARIH"; //SABIT BIR IP YE GONDERECEGIM TARIH BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
char hava1[]="HAVA"; //SABIT BIR IP YE GONDERECEGIM HAVA BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
char toprak1[]="TOPRAK"; //SABIT BIR IP YE GONDERECEGIM TOPRAK BILGISININ BILGILENDIRME MESAJINI TANIMLADIM
char bilgi[]="BILGILERI"; //SABIT BIR IP YE GONDERECEGIM BILGILENDIRME MESAJINI TANIMLADIM
char gaz_alarm[]="GAZ ALARMI"; //SABIT BIR IP YE GONDERECEGIM GAZ ALARMININ BILGILENDIRME MESAJINI TANIMLADIM
char darbe_alarm[]="DARBE ALARMI"; //SABIT BIR IP YE GONDERECEGIM DARBE ALARMININ BILGILENDIRME MESAJINI TANIMLADIM
short Hava_zaman_asimi,Toprak_zaman_asimi; //SENSORLERDEN GELECEK GERI DONUTE GORE ZAMAN ASIMI OLUP OLMAYACAGINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
unsigned long int gazl,deger,deger2; //ADC DEGERLERINI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
unsigned int8 Hava_bit1, Hava_bit2, Hava_nem_bit1, Hava_nem_bit2, Hava_kontrol,Toprak_bit1, Toprak_bit2, Toprak_nem_bit1, Toprak_nem_bit2, Toprak_Kontrol,Sicaklik_Kontrol_Hava,Sicaklik_Kontrol_Toprak; //SENSORLERDEN GELECEK BILGILERI KAYDEDECEGIM DEGISKENLERIMI TANIMLADIM
unsigned int saniye,saniyec, saniye10, dakika,dakikac, dakika10,saat, saat10, gun, gun10, ay, ay10,yil, yil10,tarih; //TARIH BILGILERINI KAYDEDECEGIM ISLEM DEGISKENLERINI TANIMLADIM
char gunaa[5],ayaa[5],yilaa[5],tarihaa[5],saataa[5],dakikaaa[5]; //GSM MODULUNDEN GELECEK TARIH SAATI KAYDEDECEGIM DEGISKENIMI TANIMLADIM
unsigned int guna,aya,yila,tariha,saata,dakikaa; //TARIH ISLEMLERINDE KULLANACAGIM DEGISKENLERIMI TANIMLADIM
unsigned int guna10,aya10,yila10,tariha10,saata10,dakikaa10; //TARIH ISLEMLERINDE KULLANACAGIM DEGISKENLERIMI TANIMLADIM
unsigned int guna1,aya1,yila1,saata1,dakikaa1; //TARIH ISLEMLERINDE KULLANACAGIM DEGISKENLERIMI TANIMLADIM
int kapi,k,a,ka,kk; //BILGI GELDIGINDE KAPININ ACILIP KAPANABILMESI ICIN GEREKLI DEGISKENLERI TANIMLADIM
char kapic; //GSM MODULUNDEN GELECEK BILGIYI TUTACAK DEGISKENI TANIMLADIM
char yon[10]; //YON DEGISKENIMI TANIMLADIM
char ruzgar_sid[]="      "; //RUZGAR SIDDETI DEGISKENIMI TANIMLADIM
int kontrol; //KONTROL DEGISKENIMI TANIMLADIM

void gsm_saat(){  //GSM MODULUNU CALISTIRACAK FONKSIYONUMU TANIMLADIM
   restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
   gets(gunaa); //GUNAA BILGISINI CEKTIM
   gets(ayaa); //AYAA BILGISINI CEKTIM
   gets(yilaa); //YILAA BILGISINI CEKTIM
   gets(tarihaa); //TARIHAA BILGISINI CEKTIM
   restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
   gets(saataa); //SAATAA BILGISINI CEKTIM
   gets(dakikaaa); //DAKIKAAA BILGISINI CEKTIM
   if(strlen(gunaa)>1){   //GUNAA UZUNLUGUNA GORE RTC DE KULLANACAGIM GUNA YI BULDUM
   guna10=gunaa[1]-48; //GUNAA DEGISKENIMIN 2. ELEMANINI ASCII DEN CEVIRIP GUNA10 DEGISKENIME KAYDETTIM
   guna1=gunaa[0]-48;  //GUNAA DEGISKENIMIN 1. ELEMANINI ASCII DEN CEVIRIP GUNA1 DEGISKENIME KAYDETTIM
      guna=(guna1*10)+guna10; //GUNA1 DEGISKENIMI 10 ILE CARPIP VE GUNA10 ILE TOPLAYIP GUNA DEGISKENIME KAYDETTIM
      write_eeprom(0,guna);}  //EEPROM A GUNA BILGISINI YAZDIRDIM
   else{
   guna=gunaa[0]-48;  //GUNAA DEGISKENIMIN 1.ELEMANINI ASCII DEN CEVIRIP GUNA DEGISKENIME KAYDETTIM
   write_eeprom(0,guna);}  //EEPROM A GUNA BILGISINI YAZDIRDIM
   if(strlen(ayaa)>2){ //AYAA UZUNLUGUNA GORE RTC DE KULLANACAGIM AYA YI BULDUM
   aya10=ayaa[2]-48;  //AYAA DEGISKENIMIN 3.ELEMANINI ASCII DEN CEVIRIP AYA10 DEGISKENIME KAYDETTIM
   aya1=ayaa[1]-48;  //AYAA DEGISKENIMIN 2.ELEMANINI ASCII DEN CEVIRIP AYA1 DEGISKENIME KAYDETTIM
   aya=(aya1*10)+aya10;  //AYA1 DEGISKENIMI 10 ILE CARPTIM VE AYA10 ILE TOPLAYIP AYA DEGISKENIME KAYDETTIM
   write_eeprom(1,aya);}  //EEPROM A AYA BILGISINI YAZDIRDIM
   else{
      aya=ayaa[1]-48;  //AYAA DEGISKENIMIN 1.ELEMANINI ASCII DEN CEVIRIP AYA DEGISKENIME KAYDETTIM
      write_eeprom(1,aya);}  //EEPROM A AYA BILGISINI YAZDIRDIM
   yila10=yilaa[2]-48; //YILAA DEGISKENIMIN 3.ELEMANINI ASCII DEN CEVIRIP YILA10 DEGISKENIME KAYDETTIM
   yila1=yilaa[1]-48; //YILAA DEGISKENIMIN 2.ELEMANINI ASCII DEN CEVIRIP YILA1 DEGISKENIME KAYDETTIM
   yila=(yila1*10)+yila10; //YILA1 I 10 ILE CARPIP YILA10 DEGISKENIM ILE TOPLAYIP YILA DEGISKENIME KAYDETTIM
   write_eeprom(2,yila);  //EEPROM A YILA BILGISINI YAZDIRDIM
   tariha=tarihaa[1]-48; //TARIHAA DEGISKENIMIN 2. ELEMANINI ASCII DEN CEVIRIP TARIHA DEGISKENIME KAYDETTIM
   write_eeprom(3,tariha);  //EEPROM A TARIHA BILGISINI YAZDIRDIM
   if(strlen(saataa)>2){ //SAATAA UZUNLUGUNA GORE RTC DE KULLANACAGIM SAATA YI BULDUM
   saata10=saataa[2]-48; //SAATAA DEGISKENIMIN 3.ELEMANINI ASCII DEN CEVIRIP SAATA10 DEGISKENIME KAYDETTIM
   saata1=saataa[1]-48; //SAATAA DEGISKENIMIN 2.ELEMANINI ASCII DEN CEVIRIP SAAT1 DEGISKENIME KAYDETTIM
   saata=(saata1*10)+saata10; //SAAT1 I 10 ILE CARPIP SAAT10 ILE TOPLADIM SAATA DEGISKENIME KAYDETTIM
   write_eeprom(4,saata);}  //EEPROM A SAATA BILGISINI YAZDIRDIM
   else{
      saata=saataa[1]-48; //SAATAA DEGISKENIMIN 2. ELEMANINI SAATA DEGISKENIME KAYDETTIM
      write_eeprom(4,saata);}  //EEPROM A SAATA BILGISINI YAZDIRDIM
   if(strlen(dakikaaa)>2){ //DAKIKAAA UZUNLUGUNA GORE RTC DE KULLANACAGIM DAKIKAA YI BULDUM
   dakikaa10=dakikaaa[2]-48;  //DAKIKAAA DEGISKENIMIN 3. ELEMANINI ASCII DEN CEVIRIP DAKIKAA10 DEGISKENIME KAYDETTIM
   dakikaa1=dakikaaa[1]-48; //DAKIKAAA DEGISKENIMIN 2.ELEMANINI ASCII DEN CEVIRIP DAKIKAA1 DEGISKENIME KAYDETTIM
   dakikaa=(dakikaa1*10)+dakikaa10; //DAKIKAA1 I 10 ILE CARPIP DAKIKAA10 LA TOPLADIM VE DAKIKAA DEGISKENIME KAYDETTIM
   write_eeprom(5,dakikaa);}  //EEPROM A DAKIKAA BILGISINI YAZDIRDIM
   else{
      dakikaa=dakikaaa[1]-48; //DAKIKAAA DEGISKENIMIN 2.ELEMANINI DAKIKAA DEGISKENIME KAYDETTIM
      write_eeprom(5,dakikaa);}  //EEPROM A DAKIKAA BILGISINI YAZDIRDIM
   }
 
#int_RDA  //KESMEMI KULLANDIM
void gsm_kapi(void){ //KAPI BILGISINI CEKECEK FONKSIYONUMU TANIMLADIM
restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
kapic = getc();}

void Hava_sensor_ac(){ //HAVA NEM VE SICAKLIK SENSORUNU AKTIFLESTIRECEK FONKSIYONUMU TANIMLADIM
  output_drive(HAVA_SICAKLIK_NEM);    //HAVA SICAKLIK VE NEM SENSORUNUN PININE CIKIS VERDIM
  output_low(HAVA_SICAKLIK_NEM);      //HAVA SICAKLIK VE NEM SENSORUNUN PININE LOW DEGERI VERDIM
  delay_ms(25); //25MS BEKLEDIM
  output_high(HAVA_SICAKLIK_NEM);     //HAVA SICAKLIK VE NEM SENSORUNUN PININE HIGH DEGERI VERDIM
  delay_us(30);//30MS BEKLEDIM
  output_float(HAVA_SICAKLIK_NEM);}  //HAVA SICAKLIK VE NEM SENSORUNU PININE GIRIS VERDIM
short Hava_sensor_kontrol(){ //HAVA NEM VE SICAKLIK SENSORUNUN DURUMUNU KONTROL ETTIM
  delay_us(40);//40MS BEKLEDIM
  if(!input(HAVA_SICAKLIK_NEM)){      //HAVA SICAKLIK VE NEM SENSORUNUN GIRISINI KONTROL ETTIM
    delay_us(80);//80MS BEKLEDIM
    if(input(HAVA_SICAKLIK_NEM)){   //HAVA SICAKLIK VE NEM SENSORUNUN GIRISINI KONTROL ETTIM    
      delay_us(50);//50MS BEKLEDIM
      return 1;
    }
  }
}
unsigned int8 Hava_sensor_okuma(){  //HAVA NEM VE SICAKLIK SENSORUNDEN BILGI OKUDUM
  unsigned int8 ih, kh, _hdata = 0;       
  if(Hava_zaman_asimi)
    break;
  for(ih = 0; ih < 8; ih++){
    kh= 0;
    while(!input(HAVA_SICAKLIK_NEM)){       //HAVA SICAKLIK VE NEM SENSORUNUN GIRISINI KONTROL ETTIM        
      kh++;
      if(kh > 100){
        Hava_zaman_asimi = 1;
        break;
      }
      delay_us(1);//1MS BEKLEDIM
    }
    delay_us(30);
    if(!input(HAVA_SICAKLIK_NEM))  //HAVA SICAKLIK VE NEM SENSORUNUN GIRISINI KONTROL ETTIM    
      bit_clear(_hdata, (7 - ih));        
    else{
      bit_set(_hdata, (7 - ih));          
      while(input(HAVA_SICAKLIK_NEM)){        //HAVA SICAKLIK VE NEM SENSORUNUN GIRISINI KONTROL ETTIM      
        kh++;
        if(kh > 100){
        Hava_zaman_asimi = 1;
        break;
      }
      delay_us(1);} //1MS BEKLEDIM
    }
  }
  return _hdata;
}
void hava_func(){ //HAVA NEM VE SICAKLIK DEGERINI BULACAGIM FONKSIYONUMU TANIMLADIM
       Hava_zaman_asimi = 0;
    Hava_sensor_ac(); //SENSORU ACMAK ICIN GEREKLI FONKSIYONUMU CAGIRDIM
    if(Hava_sensor_kontrol()){      //HAVA SICAKLIK VE NEM SENSORUNUN GIRISINI KONTROL ETTIM                     
      Hava_nem_bit1 = Hava_sensor_okuma();       //HAVA NEMINI HAVA SENSORUNDEN OKUDUM         
      Hava_nem_bit2 = Hava_sensor_okuma();       //HAVA NEMINI HAVA SENSORUNDEN OKUDUM               
      Hava_bit1 = Hava_sensor_okuma();                 //HAVA SICAKLIGINI HAVA SENSORUNDEN OKUDUM
      Hava_bit2 = Hava_sensor_okuma();                 //HAVA SICAKLIGINI HAVA SENSORUNDEN OKUDUM 
      Hava_kontrol = Hava_sensor_okuma();            //HAVA SENSORUNU KONTROL ETTIM         
      if(Hava_zaman_asimi){    //HAVA SENSORU ZAMAN ASIMINI KONTRIK ETTIM                      
        lcd_putc('\f');  //EKRANI TEMIZLEDIM                     
        lcd_gotoxy(5, 1);   //BESINCI SUTUNA GITTIM                 
        lcd_putc("ZamanAsimi!");  //EGER ZAMAN ASIMI OLDUYSA LCD YE ZAMANASIMI YAZDIRDIM
      }
      else{
       if(Hava_kontrol == ((Hava_nem_bit1 + Hava_nem_bit2 + Hava_bit1 + Hava_bit2) & 0xFF)){  //HAVA SENSORUNDEN KONTROL ETTIGIM DEGER ILE ALDIGIM DEGER AYNI ISE
         Hava_sicaklik[11]  = Hava_bit1/10  + 48;  //HAVA SICAKLIGININ ONLAR BASAMAGINI KAYDETTIM
         Hava_sicaklik[12]  = Hava_bit1%10  + 48;  //HAVA SICAKLIGININ BIRLER BASAMAGINI KAYDETTIM
         Hava_sicaklik[14] = Hava_bit2/10  + 48;  //HAVA SICAKLIGININ . DAN SONRASINI KAYDETTIM
         Hava_nem[7]  = Hava_nem_bit1/10 + 48;  //HAVA NEMININ ONLAR BASAMAGINI KAYDETTIM
         Hava_nem[8]  = Hava_nem_bit1%10 + 48; //HAVA NEMININ BIRLER BASAMAGINI KAYDETTIM 
         Hava_nem[10] = Hava_nem_bit2/10 + 48;  //HAVA NEMININ . DAN SONRASINI KAYDETTIM
         Hava_sicaklik[15] = 223;   //¯ DERECE SEMBOLUMU KOYDUM                
    Sicaklik_Kontrol_Hava=Hava_bit1; //HAVA SICAKLIGINI SICAKLIK KONTROLE KAYDETTIM
       }
       else{   //EGER DEGERLER FARKLI ISE SENSOR HATASI OLARAK KULLANICIYA GERI BILDIRIMDE BULUNDUM KOYDUM
         lcd_putc('\f');   //LCD YI TEMIZLE                    
         lcd_gotoxy(1, 1);      //BIRINCI SATIRA GIT
         lcd_putc("SensorHatasi!");  //SENSOR HATASI YAZ
       }
      }
    }
    else {  //EGER SENSORDEN CEVAP ALINAMAZSA KULLANICIYA GERI BILDIRIMDE BULUNDUM
      lcd_putc('\f');        //LCD YI YEMIZLE                 
      lcd_gotoxy(3, 1);     //UCUNCU SUTUNA GIT                   
      lcd_putc("Sensorden"); //SENSORDEN YAZ
      lcd_gotoxy(1, 2);   //IKINCI SATIRA GIT                     
      lcd_putc("Cevap Alinamiyor");  //CEVAP ALINAMIYOR YAZ
      }
 }
 void Toprak_sensor_ac(){ //TOPRAK NEM VE SICAKLIK SENSORUNU AKTIFLESTIRDIM
  output_drive(TOPRAK_SICAKLIK_NEM);   //TOPRAK SICAKLIK NEM SENSORUNUN PININI CIKIS OLARAK AYARLA
  output_low(TOPRAK_SICAKLIK_NEM);      //TOPRAK SICAKLIK NEM SENSORUNUN PININE LOW VERDIM
  delay_ms(25); //25 MS BEKLEDIM
  output_high(TOPRAK_SICAKLIK_NEM);  //TOPRAK SICAKLIK NEM SENSORUNUN PININE HIGH VERDIM   
  delay_us(30); //30 MS BEKLEDIM
  output_float(TOPRAK_SICAKLIK_NEM); //TOPRAK SICAKLIK NEM SENSORUNU GIRIS OLARAK AYARLADIM    
}
short Toprak_sensor_kontrol(){ //TOPRAK SICAKLIK VE NEM SENSORUNUN DURUMUNU KONTROL ETTIM
  delay_us(40); //40MS BEKLEDIM
  if(!input(TOPRAK_SICAKLIK_NEM)){  //TOPRAK SICAKLIK VE NEM SENSORUNU KONTROL ETTIM     
    delay_us(80); //80 MS BEKLEDIM
    if(input(TOPRAK_SICAKLIK_NEM)){  //TOPRAK SICAKLIK VE NEM SENSORUNU KONTROL ETTIM
      delay_us(50); //50 MS BEKLEDIM
      return 1;
    }
  }
}
unsigned int8 Toprak_sensor_okuma(){ //TOPRAK  NEM VE SICAKLIK SENSORUNDEN GELEN BILGILERI OKUDUM 
  unsigned int8 it, kt, _tdata = 0;       
  if(Toprak_zaman_asimi) //TOPRAK ZAMAN ASIMINI KONTROL ETTIM
    break;
  for(it = 0; it < 8; it++){
    kt = 0;
    while(!input(TOPRAK_SICAKLIK_NEM)){     //TOPRAK SICAKLIK VE NEM SENSORUNU KONTROL ETTIM
      kt++;
      if(kt > 100){
        Toprak_zaman_asimi = 1;
        break;
      }
      delay_us(1); //1 MS BEKLEDIM
    }
    delay_us(30); //30MS BEKLEDIM
    if(!input(TOPRAK_SICAKLIK_NEM))  //TOPRAK SICAKLIK VE NEM SENSORUNU KONTROL ETTIM
      bit_clear(_tdata, (7 - it));       
    else{
      bit_set(_tdata, (7 - it));          
      while(input(TOPRAK_SICAKLIK_NEM)){   //TOPRAK SICAKLIK VE NEM SENSORUNU KONTROL ETTIM       
        kt++;
        if(kt > 100){
        Toprak_zaman_asimi = 1;
        break;
      }
      delay_us(1);}  //1 MS BEKLEDIM
    }
  }
  return _tdata;
}
void toprak_func(){ //TOPRAK NEM VE SICAKLIK SENSORUNUN DEGERINI BULACAGIM FONKSIYONUMU TANIMLADIM 
       Toprak_zaman_asimi = 0;
    Toprak_sensor_ac();  //SENSORU ACMAK ICIN GEREKLI FONKSIYONUMU CAGIRDIM
    if(Toprak_sensor_kontrol()){  //TOPRAK SICAKLIK NEM SENSORUNU KONTROL ETTIM                   
      Toprak_nem_bit1 = Toprak_sensor_okuma();      //TOPRAK NEMINI OKUDUM
      Toprak_nem_bit2 = Toprak_sensor_okuma();      //TOPRAK NEMINI OKUDUM          
      Toprak_bit1 = Toprak_sensor_okuma();               //TOPRAK SICAKLIGINI OKUDUM   
      Toprak_bit2 = Toprak_sensor_okuma();               //TOPRAK SICAKLIGINI OKUDUM   
      Toprak_Kontrol = Toprak_sensor_okuma();        //TOPRAK SICAKLIK VE NEM SENSORUNU KONTROL ETTIM         
      if(Toprak_zaman_asimi){     // TOPRAK SENSORUNU OKURKEN ZAMAN ASIMI OLDUYSA                     
        lcd_putc('\f');     //EKRANI TEMIZLEDIM                  
        lcd_gotoxy(5, 1);    //BESINCI SUTUNA GITTIM                 
        lcd_putc("ZamanAsimi!"); //KULLANICIYA GERI BILDIRIM OLARAK EKRANA ZAMAN ASIMI YAZDIRDIM
      }
      else{
       if(Toprak_Kontrol == ((Toprak_nem_bit1 + Toprak_nem_bit2 + Toprak_bit1 + Toprak_bit2) & 0xFF)){ //TOPRAK SENSORU ILE KAYDETTIGIM DEGERLERI KARSILASTIRDIM
         Toprak_sicaklik[11]  = Toprak_bit1/10  + 48;  //TOPRAK SICAKLIGININ ONLAR BASAMAGINI KAYDETTIM
         Toprak_sicaklik[12]  = Toprak_bit1%10  + 48;  //TOPRAK SICAKLIGININ BIRLER BASAMAGINI KAYDETTIM
         Toprak_sicaklik[14] = Toprak_bit2/10  + 48;  //TOPRAK SICAKLIGININ . DAN SONRA OLAN KISMINI KAYDETTIM
         Toprak_nem[7]  = Toprak_nem_bit1/10 + 48;  //TOPRAK NEMININ ONLAR BASAMAGINI KAYDETTIM
         Toprak_nem[8]  = Toprak_nem_bit1%10 + 48;  //TOPRAK NEMININ BIRLER BASAMAGINI KAYDETTIM
         Toprak_nem[10] = Toprak_nem_bit2/10 + 48;  //TOPRAK NEMININ . DAN SONRA OLAN KISMINI KAYDETTIM
         Toprak_sicaklik[15] = 223;    //TOPRAK SICAKLIGINA ¯ DERECE SEMBOLU EKLEDIM               
    Sicaklik_Kontrol_Toprak=Toprak_bit1; //TOPRAK SICAKLIK KONTROLUNE TOPRAK SICAKLIGINI KAYDETTIM
       }
       else{ //DEGERLER UYUSMUYORSA 
         lcd_putc('\f');    //EKRANI TEMIZLEDIM                   
         lcd_gotoxy(1, 1);  //BIRINCI SATIRA GITTIM                    
         lcd_putc("SensorHatasi!"); //KULLANICIYA GERI BILDIRIM OLARAK LCD YE SENSORHATASI YAZDIRDIM 
       }
      }
    }
    else { //SENSORDEN CEVAP ALINAMAZSA
      lcd_putc('\f');     //EKRANI TEMIZLEDIM                     
      lcd_gotoxy(3, 1);   //UCUNCU SUTUNA GITTIM                     
      lcd_putc("Sensorden");  //SENSORDEN YAZDIRDIM
      lcd_gotoxy(1, 2);    //IKINCI SATIRA GECTIM                    
      lcd_putc("Cevap Alinamiyor");  //LCD YE CEVAP ALINAMIYOR YAZDIRDIM
      }
 }
void time_func(){ //RTC DEN BILGI ALMAK ICIN GEREKLI FONKSIYONUMU KURDUM
      restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
      rtc_get_time(saat,dakika,saniye); //RTC DEN SAAT DAKIKA SANIYE BILGILERINI OKUDUM
      rtc_get_date(gun,ay,yil,tarih); //RTC DEN GUN AY YIL TARIH BILGILERINI OKUDUM
      dakikac = dakika; //DAKIKAYI DAKIKAC DEGISKENIME KAYDETTIM
      saniyec = saniye;  //SANIYEYI SANIYEC DEGISKENIME KAYDETTIM
      saniye10  = saniye%10;  //SANIYEMIN BIRLER BASAMAGINI BULDUM
      saniye = (saniye-saniye10)/10;  //SANIYEMIN ONLAR BASAMAGINI BULDUM
      dakika10  = dakika%10 ;  //DAKIKAMIN BIRLER BASAMAGINI BULDUM
      dakika = (dakika-dakika10)/10;  //DAKIKAMIN ONLAR BASAMAGINI BULDUM
      saat10  = saat%10;  //SAATIMIN BIRLER BASAMAGINI BULDUM
      saat = (saat-saat10)/10;  //SAATIMIN ONLAR BASAMAGINI BULDUM
      gun10  = gun%10;  //GUNUMUN BIRLER BASAMAGINI BULDUM
      gun = (gun-gun10)/10;  //GUNUMUN ONLAR BASAMAGINI BULDUM
      ay10  =  ay%10;  //AYIMIN BIRLER BASAMAGINI BULDUM
      ay = (ay-ay10)/10;  //AYIMIN ONLAR BASAMAGINI BULDUM
      yil10  =  yil%10;  //YILIMIN BIRLER BASAMAGINI BULDUM
      yil = (yil-yil10)/10;  //YILIMIN ONLAR BASAMAGINI BULDUM
      saatc[6]  = saniye  + 48; //SAATIMIN SANIYESININ ONLAR BASAMAGINI SAATC DEGISKENIME YAZDIRDIM
      saatc[7]  = saniye10  + 48;  //SAATIMIN SANIYESININ BIRLER BASAMAGINI SAATC DEGISKENIME YAZDIRDIM
      saatc[3]  = dakika  + 48;  //SAATIMIN DAKIKASININ ONLAR BASAMAGINI SAATC DEGISKENIME YAZDIRDIM
      saatc[4]  = dakika10  + 48;  //SAATIMIN DAKIKASININ BIRLER BASAMAGINI SAATC DEGISKENIME YAZDIRDIM
      saatc[0]  = saat  + 48;  //SAATIMIN SAATININ ONLAR BASAMAGINI SAATC DEGISKENIME YAZDIRDIM
      saatc[1]  = saat10  + 48;  //SAATIMIN SAATININ BIRLER BASAMAGINI SAATC DEGISKENIME YAZDIRDIM
      tarihc[8]  = yil  + 48;  //TARIHIMIN YILININ ONLAR BASAMAGINI TARIHC DEGISKENIME YAZDIRDIM
      tarihc[9]  = yil10  + 48;   //TARIHIMIN YILININ BIRLER BASAMAGINI TARIHC DEGISKENIME YAZDIRDIM  
      tarihc[3]  = ay + 48;    //TARIHIMIN AYININ ONLAR BASAMAGINI TARIHC DEGISKENIME YAZDIRDIM
      tarihc[4]  = ay10 + 48;   //TARIHIMIN AYININ BIRLER BASAMAGINI TARIHC DEGISKENIME YAZDIRDIM
      tarihc[0]  = gun + 48;    //TARIHIMIN GUNUNUN ONLAR BASAMAGINI TARIHC DEGISKENIME YAZDIRDIM
      tarihc[1]  = gun10 + 48;   //TARIHIMIN GUNUNUN BIRLER BASAMAGINI TARIHC DEGISKENIME YAZDIRDIM
   switch(tarih){ //RTC DEN GELEN BILGI ILE TARIH DEGISKENINE BAKIP GUNU BELIRLEDIM
      case 1:{ //1 ISE PAZAR
      haftanin_gunu[0] = 'P';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'Z';
      haftanin_gunu[3] = 'A';
      haftanin_gunu[4]= 'R';
      break;}
      case 2:{ //2 ISE PAZARTESI
      haftanin_gunu[0] = 'P';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'Z';
      haftanin_gunu[3] = 'A';
      haftanin_gunu[4]= 'R';
      haftanin_gunu[5]= 'T';
      haftanin_gunu[6] = 'E';
      haftanin_gunu[7]= 'S';
      haftanin_gunu[8]= 'I';
      break;}
      case 3:{ //3 ISE SALI
      haftanin_gunu[0] ='S';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'L';
      haftanin_gunu[3]= 'I';
      break;}
      case 4:{ //4 ISE CARSAMBA
      haftanin_gunu[0] = 'C';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'R';
      haftanin_gunu[3] = 'S';
      haftanin_gunu[4]= 'A';
      haftanin_gunu[5]= 'M';
      haftanin_gunu[6] = 'B';
      haftanin_gunu[7]= 'A';
      break;}
      case 5:{ //5 ISE PERSEMBE
      haftanin_gunu[0] = 'P';
      haftanin_gunu[1]= 'E';
      haftanin_gunu[2]= 'R';
      haftanin_gunu[3] = 'S';
      haftanin_gunu[4]= 'E';
      haftanin_gunu[5]= 'M';
      haftanin_gunu[6] = 'B';
      haftanin_gunu[7]= 'E';
      break;}
      case 6:{ //6 ISE CUMA
      haftanin_gunu[0] = 'C';
      haftanin_gunu[1]= 'U';
      haftanin_gunu[2]= 'M';
      haftanin_gunu[3]= 'A';
      break;}
      case 7:{ //7 ISE CUMARTESI
      haftanin_gunu[0] = 'C';
      haftanin_gunu[1]= 'U';
      haftanin_gunu[2]= 'M';
      haftanin_gunu[3] = 'A';
      haftanin_gunu[4]= 'R';
      haftanin_gunu[5]= 'T';
      haftanin_gunu[6] = 'E';
      haftanin_gunu[7]= 'S';
      haftanin_gunu[8]= 'I';
      break;}}
      lcd_putc('\f'); //LCD EKRANINI TEMIZLEDIM                         
      lcd_gotoxy(1, 1); // LCD NIN BIRINCI SATIRINA GECTIM                      
      printf(lcd_putc,time1); //LCD YE SAAT BILGIMI BELIRTECEK DEGISKENIMI YAZDIRDIM
      lcd_gotoxy(1, 2); //LCD NIN IKINCI SATIRINA GECTIM                     
      printf(lcd_putc,saatc);  //SAATI LCD YE YAZDIRDIM
}
void gas_func(){  //ADC AYARLADIM VE GAZ SENSORUNU OKUDUM
   set_adc_channel(3); //ADC CHANNEL INI 3 OLARAK AYARLADIM
   gazl = read_adc();  //ADC DEGERINI OKUYUP GAZL DEGISKENIME KAYDETTIM 
      delay_ms(10); //10MS BEKLEDIM
      if(gazl>=750){ //GAZ DEGERI 750 YE ESIT YADA BUYUKSE
    while(gazl>=750){ //750 YE ESIT YADA BUYUK OLDUGU SURE BOYUNCA
       gazl = read_adc(); //ADC DEGERINI OKUYUP GAZL DEGISKENIME KAYDETTIM
       output_high(pin_b1); //B1 PININI HIGH YAPTIM
       delay_ms(250);//250 MS BEKLEDIM         
       output_low(pin_b1); //B1 PININI LOW YAPTIM
       delay_ms(250);//250 MS BEKLEDIM
       puts(saatc); //SABIT IP YE SAATI YOLLADIM
       puts(gaz_alarm);} //SABIT IP YE GAZ ALARMINI YOLLADIM
    }
   else{
      output_low(pin_b1);} //B1 PININI LOW YAPTIM
   }
void gsm_func(){   //SABIT BIR IP YE GONDERECEGIM BILGILERIMI SAATLIK OLARAK AYARLADIM
      restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
      if(dakikac==00&&saniyec==0){   //SAAT BASLARINDA 
      puts(saatc); //SAATI YOLLADIM
      puts(tarihc); //TARIHI YOLLADIM
      puts(haftanin_gunu); //HAFTANIN GUNUNU YOLLADIM
      puts(hava1); //HAVA DEGISKENIMI BELLI EDECEK YAZIMI YOLLADIM
      puts(Hava_sicaklik); //HAVANIN SICAKLIGINI YOLLADIM
      puts(Hava_nem); //HAVANIN NEMINI YOLLADIM
      puts(toprak1); //TOPRAK DEGISKENIMI BELLI EDECEK YAZIMI YOLLADIM
      puts(Toprak_sicaklik); //TOPRAK SICAKLIGINI YOLLADIM
      puts(Toprak_nem); //TOPRAK NEMINI YOLLADIM
      puts(isik_siddeti); //ISIK SIDDETINI YOLLADIM
      puts(ruzgar_bilgi); //RUZGAR SIDDETI DEGISKENIMI BELLI EDECEK YAZIMI YOLLADIM
      puts(ruzgar_sid); //RUZGAR SIDDETI DEGERIMI YOLLADIM
      puts("\n"); //ALT SATIRA GECTIM
      puts(ruzgar_yon); //RUZGAR YON BILGIMI BELLI EDECEK DEGISKENIMI YOLLADIM
      puts(yon);}//RUZGAR YONUNU YOLLADIM
   }

void ruzgar_func(){ //RUZGAR SENSORUNDEN BILGI OKUMAK ICIN GEREKLI FONKSIYONUMU KURDUM
   set_adc_channel(0); //ADC KANALINI 0 OLARAK AYARLADIM
   deger=read_adc(); //DEGER DEGISKENIMI ADC DEN OKUDUGUM DEGER ILE DEGISTIRDIM
   delay_ms(1000); //1SN BEKLEDIM
   if(deger>999){ //ADC DEN OKUDUGUMUZ DEGER 999 DAN BUYUK ISE
      ruzgar_sid[4]=(deger%10)+48; //RUZGAR SIDDETI DEGISKENIMIN BIRLER BASAMAGINA ADC DEN OKUDUGUMUZ BIRLER BASAMAGINI KAYDETTIM
      ruzgar_sid[3]=((deger%100-deger%10)/10)+48; //RUZGAR SIDDETI DEGISKENIMIN ONLAR BASAMAGINA ADC DEN OKUDUGUMUZ ONLAR BASAMAGINI KAYDETTIM
      ruzgar_sid[2]=((deger%1000-deger%100-deger%10)/100)+48; //RUZGAR SIDDETI DEGISKENIMIN YUZLER BASAMAGINA ADC DEN OKUDUGUMUZ YUZLER BASAMAGINI KAYDETTIM
      ruzgar_sid[1]=((deger-((deger%1000-deger%100-deger%10)/100))/1000)+48;} //RUZGAR SIDDETI DEGISKENIMIN BINLER BASAMAGINA ADC DEN OKUDUGUMUZ BINLER BASAMAGINI KAYDETTIM
   if(deger>99 && deger<=999){ //ADC DEN OKUDUGUMUZ DEGER 99 DAN BUYUK VE 999 A KUCUK YADA ESITSE
      ruzgar_sid[3]=(deger%10)+48; //RUZGAR SIDDETI DEGISKENIMIN BIRLER BASAMAGINA ADC DEN OKUDUGUMUZ BIRLER BASAMAGINI KAYDETTIM
      ruzgar_sid[2]=((deger%100-deger%10)/10)+48; //RUZGAR SIDDETI DEGISKENIMIN ONLAR BASAMAGINA ADC DEN OKUDUGUMUZ ONLAR BASAMAGINI KAYDETTIM
      ruzgar_sid[1]=((deger%1000-deger%100-deger%10)/100)+48;} //RUZGAR SIDDETI DEGISKENIMIN YUZLER BASAMAGINA ADC DEN OKUDUGUMUZ YUZLER BASAMAGINI KAYDETTIM
   if(deger<=99){ //ADC DEN OKUDUGUMUZ DEGER 99 A ESIT YADA KUCUKSE
      ruzgar_sid[2]=(deger%10)+48; //RUZGAR SIDDETI DEGISKENIMIN BIRLER BASAMAGINA ADC DEN OKUDUGUMUZ BIRLER BASAMAGINI KAYDETTIM
      ruzgar_sid[1]=((deger%100-deger%10)/10)+48;}   //RUZGAR SIDDETI DEGISKENIMIN ONLAR BASAMAGINA ADC DEN OKUDUGUMUZ ONLAR BASAMAGINI KAYDETTIM
   if(input(pin_c4)==1){    //C4 PININE BAGLI BUTONA BASILDI ISE
    yon="Kuzey";} //YON DEGISKENINI KUZEY OLARAK KAYDETTIM
   if(input(pin_b3)==1){   //B3 PININE BAGLI BUTONA BASILDI ISE
    yon="Guney";} //YON DEGISKENINI GUNEY OLARAK KAYDETTIM
   if(input(pin_b6)==1){   //B6 PININE BAGLI BUTONA BASILDI ISE
    yon="Dogu";}   //YON DEGISKENINI DOGU OLARAK KAYDETTIM
   if(input(pin_b7)==1){  //B7 PININE BAGLI BUTONA BASILDI ISE
    yon="Bati";} //YOK DEGISKENINI BATI OLARAK KAYDETTIM
   if(input(pin_c4)==0 && input(pin_b3)==0 && input(pin_b6)==0 && input(pin_b7)==0 ){  //HICBIR TUSA BASILMADI ISE
    yon="Yon Yok";} //YON DEGISKENINI YON YOK OLARAK KAYDETTIM
    
   }
   
 void gunes_func(){ //GUNES SENSORUNDEN BILGI OKUMAK ICIN GEREKLI FONKSIYONUMU KURDUM
   delay_ms(100);//100 MS BEKLEDIM
   set_adc_channel(1); //ADC KANALINI 1 OLARAK AYARLADIM
   adc_deger = read_adc(); //ADC DEGERINI ADC_DEGER DEGISKENIME KAYDETTIM
if(adc_deger>=100){ //ADC DEGERI 100 E ESIT YADA BUYUKSE 
   adc_deger10=adc_deger%100; //ADC DEGERININ ONLAR VE BIRLER BASAMAGINI BULDUM
   isik_siddeti[2]=(adc_deger%10)+48; //ISIK SIDDETINE ADC DEN OKUDUGUM DEGERIN BIRLER BASAMAGINI KAYDETTIM
   isik_siddeti[1]=((adc_deger10-(adc_deger%10))/10)+48; //ISIK SIDDETINE ADC DEN OKUDUGUM DEGERIN ONLAR BASAMAGINI KAYDETTIM
   isik_siddeti[0]=((adc_deger-adc_deger10)/100)+48;} //ISIK SIDDETINE ADC DEN OKUDUGUM DEGERIN YUZLER BASAMAGINI KAYDETTIM
else{ //100 DEN KUCUKSE 
   isik_siddeti[2]=(adc_deger%10)+48; //ISIK SIDDETINE ADC DEN OKUDUGUM DEGERIN BIRLER BASAMAGINI KAYDETTIM
   isik_siddeti[1]=((adc_deger-(adc_deger%10))/10)+48; //ISIK SIDDETINE ADC DEN OKUDUGUM DEGERIN OBLAR BASAMAGINI KAYDETTIM
   isik_siddeti[0]=' ';} //ISIK SIDDETININ BASINA BOSLUK KOYDUM
    }
      
void darbe_func(){ //IVME SENSORUNDEN BILGI OKUMAK ICIN GEREKLI FONKSIYONUMU KURDUM
   set_adc_channel(2); //ADC KANALINI 2 OLARAK AYARLADIM
   deger2=read_adc(); //ADC DEGERINI DEGER2 DEGISKENINE KAYDETTIM
   if(deger2 >=  450){ //ADC DEGERI 450 DEN BUYUK ISE
    lcd_putc('\f'); //LCD YI TEMIZLEDIM
    lcd_gotoxy(1, 1);  //LCD NIN ILK SATIRINA GITTIM
    printf(lcd_putc,darbe_alarm); //DARBE ALARMI BILGINISI YAZDIRDIM
      while(deger2>=450){ //ADC DEGERI 450 DEN BUYUK OLDUGU SURE BOYUNCA
    deger2=read_adc(); //ADC DEGERINI OKUDUM
    output_high(PIN_B1); //B1 PININI HIGH YAPTIM
    delay_ms(500);//0.5SN BEKLEDIM
    output_low(PIN_B1); //B1 PININI LOW YAPTIM
    delay_ms(500);}}//0.5SN BEKLEDIM
   } 
  
float kapi_kontrol(void){  //10 SANIYE BOYUNCA 1 SN ACIK 1 SN SONUK OLACAK LEDIMI VE KAPIMI AYARLAYACAGIM FONKSIYONUMU TANIMLADIM
      restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
      if(kapic == 'a'){  //GSM DEN KAPI ICIN A DEGERI GELDIGINDE KAPIYI ACTIM
    k++;
    ka++;
    a=0;
    if(k<6){ //LEDI 10 SN DE 1 KERE YAKIP 1 KERE SONDURDUM
       delay_ms(1000);//1SN BEKLEDIM
       output_high(pin_c5);//C5 PININI HIGH YAPTIM
       delay_ms(1000);//1SN BEKLEDIM
       output_low(pin_c5);//C5 PININI LOW YAPTIM
    if(ka<2) //KAPIYI FULL ACIK BIRAKTIM
         output_high(PIN_a5);}//C5 PININI HIGH YAPTIM
       else  //ISIGIMI FULL ACIK BIRAKTIM
          output_high(pin_c5);} //C5 PININI HIGH YAPTIM
    restart_wdt(); //WATCHDOG TIMER I SIFIRLADIM
    if(kapic =='k'){ //GSM DEN KAPI ICIN K DEGERI GELDIGINDE KAPIYI KAPATTIM
    a++;
    kk++;
    k=0;
    if(a<6){ //LEDI 10 SN DE 1 KERE YAKIP 1 KERE SONDURDUM
       delay_ms(1000);//1SN BEKLEDIM
       output_high(pin_c5);//C5 PININI HIGH YAPTIM
       delay_ms(1000);//1SN BEKLEDIM
       output_low(pin_c5);//C5 PININI LOW YAPTIM
    if(kk<2) //KAPIYI FULL KAPATTIM
         output_low(PIN_a5);}//C5 PININI LOW YAPTIM
       else //ISIGI FULL SONDURDUM
          output_low(pin_c5);} //C5 PININI LOW YAPTIM
    restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
       }
void sleep_mode(){  //B0 PININE BAGLI BUTONA BASILDIGINDA SLEEP MODE AKTIF EDILDI
   if(input(pin_b0)==1) //B0 A BASILI ISE
      sleep(); //UYKU MODUNA ALDIM
}
       
void main(void){ //MAIN FONKSIYONUMU TANIMLADIM
   setup_WDT(WDT_2304MS); //2SN DE BIR WATCHDOG TIMER I AKTIFLESTIRDIM
   read_eeprom(guna); //GUNA DEGISKENIMI EEPROM DAN OKUDUM
   if(guna=='\0') //EGER GUNA OKUNMADIYSA GSM_SAAT FONKSIYONUMU AKTIFLESTIRDIM
   gsm_saat(); //GSM SAAT FONKSIYONUMU CALISTIRDIM
delay_ms(1000); //1SN BEKLEDIM
setup_adc_ports(00001111);  //ADC UYGULANACAK PINLERI BELIRLEDIM
setup_adc(ADC_CLOCK_INTERNAL); //ADC CLOCK FREKANSINI BELIRLEDIM
output_float(PIN_C4); //C4 PINININ GIRIS OLDUGUNU BELIRTTIM
output_float(PIN_D7); //D7 PINININ GIRIS OLDUGUNU BELIRTTIM
output_float(PIN_B0); //B0 PINININ GIRIS OLDUGUNU BELIRTTIM
output_float(PIN_B3); //B3 PINININ GIRIS OLDUGUNU BELIRTTIM
output_float(PIN_B6); //B6 PINININ GIRIS OLDUGUNU BELIRTTIM
output_float(PIN_B7); //B7 PINININ GIRIS OLDUGUNU BELIRTTIM
output_drive(PIN_A5);  //A5 PINININ CIKIS OLDUGUNU BELIRTTIM
output_drive(PIN_B1); //B1 PINININ CIKIS OLDUGUNU BELIRTTIM
output_drive(PIN_C5); //C5 PINININ CIKIS OLDUGUNU BELIRTTIM
output_low(pin_C5); //C5 PINININ LOW OLDUGUNU BELIRTTIM
output_drive(ISITICI); //ISITICI PINININ CIKIS OLDUGUNU BELIRTTIM
output_drive(pin_e0); //E0 PINININ CIKIS OLDUGUNU BELIRTTIM
setup_psp(psp_disabled); //PSP BIRIMI DEVRE DISI
setup_timer_1(T1_DISABLED); //T1 ZAMANLAYICISI DEVRE DISI
setup_timer_2(T2_DISABLED,0,1); //T2 ZAMANLAYICISI DEVRE DISI
setup_ccp1(CCP_OFF); //CCP1 BIRIMI DEVRE DISI
setup_ccp2(CCP_OFF); //CCP2 BIRIMI DEVRE DISI
  lcd_init(); //LCD  HAZIRLANDI                                
  lcd_putc('\f');  //LCD EKRANI TEMIZLENDI                            
  rtc_init();  //RTC HAZIRLANDI
  rtc_set_datetime(guna,aya,yila,tariha,saata,dakikaa);  //RTC DEGERLERI GSM FONK CEKILDI VE AYARLANDI
   enable_interrupts(INT_RDA); //RDA KESMESINI AKTIFLESTIRDIM
   enable_interrupts(GLOBAL); //AKTIF EDILEN KESMELERE IZIN VERDIM
  while(TRUE){ //SONSUZ DONGUMU KURDUM
     sleep_mode(); //SLEEPMODE TUSUNA BASTIGIMDA SLEEP MODE A ALDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     gsm_kapi(); //GSM MODULE DEN GELEN KAPI BILGISINI ALDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     time_func(); //ZAMAN SAYICIMI AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     kapi_kontrol(); //KAPI ACIP KAPAMAYI AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     darbe_func(); //IVME SENSORUNU AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     gas_func(); //GAZ SENSORUNU AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     hava_func(); //HAVA SICAKLIK VE NEM SENSORUNU AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     toprak_func(); //TOPRAK SICAKLIK VE NEM SENSORUNU AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     ruzgar_func(); //RUZGAR SENSORUNU AKTIFLESTIRDIM
     restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
     If(Sicaklik_Kontrol_Hava <=0 || Sicaklik_Kontrol_Toprak <=0){ //SICAKLIK 0 IN ALTINA DUSTUGUNDE ISITICIYI CALISTIRDIM
   output_low(ISITICI);  //ISITICIYI CALISTIRDIM
   }
   else
   output_high(ISITICI); //ISITICI KAPATTIM
   restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
   gunes_func(); //GUNES SENSORUNU AKTIFLESTIRDIM
   restart_wdt(); //WATCHDOG TIMER'I SIFIRLADIM
   gsm_func(); //SAATLIK SABIT BIR IP YE VERI GONDERDIM
     }
  }

