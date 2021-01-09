//LCD PINLERINI TANIMLADIM
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_ENABLE_PIN PIN_D2
#define LCD_DATA4 PIN_D3
#define LCD_DATA5 PIN_D4
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D6

#include <16F877A.h> //16F877A KUTUPHANESINI EKLEDIIM
 #device adc=10 //ADC DEGERIMI AYARLADIM
#fuses xt,nowdt,noprotect,nobrownout,nolvp,noput,nowrt,nodebug,nocpd   //DENETLEYICI KONFIGRASYON AYARLARINI YAPTIM   
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
  gets(gunaa); //GUNAA BILGISINI CEKTIM
   gets(ayaa); //AYAA BILGISINI CEKTIM
   gets(yilaa); //YILAA BILGISINI CEKTIM
   gets(tarihaa); //TARIHAA BILGISINI CEKTIM
   gets(saataa); //SAATAA BILGISINI CEKTIM
   gets(dakikaaa); //DAKIKAAA BILGISINI CEKTIM
   if(strlen(gunaa)>1){   //GUNAA UZUNLUGUNA GORE RTC DE KULLANACAGIM GUNA YI BULDUM
   guna10=gunaa[1]-48;
   guna1=gunaa[0]-48;
      guna=(guna1*10)+guna10;
      write_eeprom(0,guna);}
   else{
   guna=gunaa[0]-48;
   write_eeprom(0,guna);}
   if(strlen(ayaa)>2){ //AYAA UZUNLUGUNA GORE RTC DE KULLANACAGIM AYA YI BULDUM
   aya10=ayaa[2]-48;
   aya1=ayaa[1]-48;
   aya=(aya1*10)+aya10;
   write_eeprom(1,aya);}
   else{
      aya=ayaa[1]-48;
      write_eeprom(1,aya);}
   yila10=yilaa[2]-48;
   yila1=yilaa[1]-48;
   yila=(yila1*10)+yila10;
   write_eeprom(2,yila);
   tariha=tarihaa[1]-48;
   write_eeprom(3,tariha);
   if(strlen(saataa)>2){ //SAATAA UZUNLUGUNA GORE RTC DE KULLANACAGIM SAATA YI BULDUM
   saata10=saataa[2]-48;
   saata1=saataa[1]-48;
   saata=(saata1*10)+saata10;
   write_eeprom(4,saata);}
   else{
      saata=saataa[1]-48;
      write_eeprom(4,saata);}
   if(strlen(dakikaaa)>2){ //DAKIKAAA UZUNLUGUNA GORE RTC DE KULLANACAGIM DAKIKAA YI BULDUM
   dakikaa10=dakikaaa[2]-48;
   dakikaa1=dakikaaa[1]-48;
   dakikaa=(dakikaa1*10)+dakikaa10;
   write_eeprom(5,dakikaa);}
   else{
      dakikaa=dakikaaa[1]-48;
      write_eeprom(5,dakikaa);}
   }
 
#int_RDA  //KESMEMI KULLANDIM
void gsm_kapi(void){ //KAPI BILGISINI CEKECEK FONKSIYONUMU TANIMLADIM
delay_ms(200);   
kapic = getc();
delay_ms(200);}

void Hava_sensor_ac(){ //HAVA NEM VE SICAKLIK SENSORUNU AKTIFLESTIRECEK FONKSIYONUMU TANIMLADIM
  output_drive(HAVA_SICAKLIK_NEM);    
  output_low(HAVA_SICAKLIK_NEM);      
  delay_ms(25);
  output_high(HAVA_SICAKLIK_NEM);     
  delay_us(30);
  output_float(HAVA_SICAKLIK_NEM);    
}
short Hava_sensor_kontrol(){ //HAVA NEM VE SICAKLIK SENSORUNUN DURUMUNU KONTROL ETTIM
  delay_us(40);
  if(!input(HAVA_SICAKLIK_NEM)){      
    delay_us(80);
    if(input(HAVA_SICAKLIK_NEM)){     
      delay_us(50);
      return 1;
    }
  }
}
unsigned int8 Hava_sensor_okuma(){ //HAVA NEM VE SICAKLIK SENSORUNDEN BILGI OKUDUM
  unsigned int8 ih, kh, _hdata = 0;       
  if(Hava_zaman_asimi)
    break;
  for(ih = 0; ih < 8; ih++){
    kh= 0;
    while(!input(HAVA_SICAKLIK_NEM)){          
      kh++;
      if(kh > 100){
        Hava_zaman_asimi = 1;
        break;
      }
      delay_us(1);
    }
    delay_us(30);
    if(!input(HAVA_SICAKLIK_NEM))
      bit_clear(_hdata, (7 - ih));        
    else{
      bit_set(_hdata, (7 - ih));          
      while(input(HAVA_SICAKLIK_NEM)){          
        kh++;
        if(kh > 100){
        Hava_zaman_asimi = 1;
        break;
      }
      delay_us(1);}
    }
  }
  return _hdata;
}
void hava_func(){ //HAVA NEM VE SICAKLIK DEGERINI BULACAGIM FONKSIYONUMU TANIMLADIM
       Hava_zaman_asimi = 0;
    Hava_sensor_ac();
    if(Hava_sensor_kontrol()){                     
      Hava_nem_bit1 = Hava_sensor_okuma();                
      Hava_nem_bit2 = Hava_sensor_okuma();                
      Hava_bit1 = Hava_sensor_okuma();                 
      Hava_bit2 = Hava_sensor_okuma();                  
      Hava_kontrol = Hava_sensor_okuma();                
      if(Hava_zaman_asimi){                          
        lcd_putc('\f');                       
        lcd_gotoxy(5, 1);                    
        lcd_putc("ZamanAsimi!");
      }
      else{
       if(Hava_kontrol == ((Hava_nem_bit1 + Hava_nem_bit2 + Hava_bit1 + Hava_bit2) & 0xFF)){
         Hava_sicaklik[11]  = Hava_bit1/10  + 48;
         Hava_sicaklik[12]  = Hava_bit1%10  + 48;
         Hava_sicaklik[14] = Hava_bit2/10  + 48;
         Hava_nem[7]  = Hava_nem_bit1/10 + 48;
         Hava_nem[8]  = Hava_nem_bit1%10 + 48;
         Hava_nem[10] = Hava_nem_bit2/10 + 48;
         Hava_sicaklik[15] = 223;                   
    Sicaklik_Kontrol_Hava=Hava_bit1;
       }
       else{
         lcd_putc('\f');                       
         lcd_gotoxy(1, 1);                     
         lcd_putc("SensorHatasi!");
       }
      }
    }
    else {
      lcd_putc('\f');                         
      lcd_gotoxy(3, 1);                        
      lcd_putc("Sensorden");
      lcd_gotoxy(1, 2);                        
      lcd_putc("Cevap Alinamiyor");
      }
 }
 void Toprak_sensor_ac(){ //TOPRAK NEM VE SICAKLIK SENSORUNU AKTIFLESTIRDIM
  output_drive(TOPRAK_SICAKLIK_NEM);   
  output_low(TOPRAK_SICAKLIK_NEM);     
  delay_ms(25);
  output_high(TOPRAK_SICAKLIK_NEM);    
  delay_us(30);
  output_float(TOPRAK_SICAKLIK_NEM);    
}
short Toprak_sensor_kontrol(){ //TOPRAK NEM VE SICAKLIK SENSORUNUN DURUMUNU KONTROL ETTIM
  delay_us(40);
  if(!input(TOPRAK_SICAKLIK_NEM)){      
    delay_us(80);
    if(input(TOPRAK_SICAKLIK_NEM)){    
      delay_us(50);
      return 1;
    }
  }
}
unsigned int8 Toprak_sensor_okuma(){ //TOPRAK  NEM VE SICAKLIK SENSORUNDEN GELEN BILGILERI OKUDUM 
  unsigned int8 it, kt, _tdata = 0;       
  if(Toprak_zaman_asimi)
    break;
  for(it = 0; it < 8; it++){
    kt = 0;
    while(!input(TOPRAK_SICAKLIK_NEM)){           
      kt++;
      if(kt > 100){
        Toprak_zaman_asimi = 1;
        break;
      }
      delay_us(1);
    }
    delay_us(30);
    if(!input(TOPRAK_SICAKLIK_NEM))
      bit_clear(_tdata, (7 - it));       
    else{
      bit_set(_tdata, (7 - it));          
      while(input(TOPRAK_SICAKLIK_NEM)){          
        kt++;
        if(kt > 100){
        Toprak_zaman_asimi = 1;
        break;
      }
      delay_us(1);}
    }
  }
  return _tdata;
}
void toprak_func(){ //TOPRAK NEM VE SICAKLIK SENSORUNUN DEGERINI BULACAGIM FONKSIYONUMU TANIMLADIM 
       Toprak_zaman_asimi = 0;
    Toprak_sensor_ac();
    if(Toprak_sensor_kontrol()){                     
      Toprak_nem_bit1 = Toprak_sensor_okuma();                 
      Toprak_nem_bit2 = Toprak_sensor_okuma();                
      Toprak_bit1 = Toprak_sensor_okuma();                  
      Toprak_bit2 = Toprak_sensor_okuma();                  
      Toprak_Kontrol = Toprak_sensor_okuma();                 
      if(Toprak_zaman_asimi){                           
        lcd_putc('\f');                       
        lcd_gotoxy(5, 1);                     
        lcd_putc("ZamanAsimi!");
      }
      else{
       if(Toprak_Kontrol == ((Toprak_nem_bit1 + Toprak_nem_bit2 + Toprak_bit1 + Toprak_bit2) & 0xFF)){
         Toprak_sicaklik[11]  = Toprak_bit1/10  + 48;
         Toprak_sicaklik[12]  = Toprak_bit1%10  + 48;
         Toprak_sicaklik[14] = Toprak_bit2/10  + 48;
         Toprak_nem[7]  = Toprak_nem_bit1/10 + 48;
         Toprak_nem[8]  = Toprak_nem_bit1%10 + 48;
         Toprak_nem[10] = Toprak_nem_bit2/10 + 48;
         Toprak_sicaklik[15] = 223;                   
    Sicaklik_Kontrol_Toprak=Toprak_bit1;
       }
       else{
         lcd_putc('\f');                       
         lcd_gotoxy(1, 1);                     
         lcd_putc("SensorHatasi!");
       }
      }
    }
    else {
      lcd_putc('\f');                          
      lcd_gotoxy(3, 1);                        
      lcd_putc("Sensorden");
      lcd_gotoxy(1, 2);                        
      lcd_putc("Cevap Alinamiyor");
      }
 }
void time_func(){ //RTC DEN BILGI ALMAK ICIN GEREKLI FONKSIYONUMU KURDUM
      rtc_get_time(saat,dakika,saniye);
      rtc_get_date(gun,ay,yil,tarih);
      dakikac = dakika;
      saniyec = saniye;
      saniye10  = saniye%10;
      saniye = (saniye-saniye10)/10;
      dakika10  = dakika%10 ;
      dakika = (dakika-dakika10)/10;
      saat10  = saat%10;
      saat = (saat-saat10)/10;
      gun10  = gun%10;
      gun = (gun-gun10)/10;
      ay10  =  ay%10;
      ay = (ay-ay10)/10;
      yil10  =  yil%10;
      yil = (yil-yil10)/10;
      saatc[6]  = saniye  + 48;
      saatc[7]  = saniye10  + 48;
      saatc[3]  = dakika  + 48;
      saatc[4]  = dakika10  + 48;
      saatc[0]  = saat  + 48;
      saatc[1]  = saat10  + 48;
      tarihc[8]  = yil  + 48;
      tarihc[9]  = yil10  + 48;
      tarihc[3]  = ay + 48;
      tarihc[4]  = ay10 + 48;
      tarihc[0]  = gun + 48;
      tarihc[1]  = gun10 + 48;  
   switch(tarih){
      case 1:{
      haftanin_gunu[0] = 'P';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'Z';
      haftanin_gunu[3] = 'A';
      haftanin_gunu[4]= 'R';
      break;}
      case 2:{
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
      case 3:{
      haftanin_gunu[0] ='S';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'L';
      haftanin_gunu[3]= 'I';
      break;}
      case 4:{
      haftanin_gunu[0] = 'C';
      haftanin_gunu[1]= 'A';
      haftanin_gunu[2]= 'R';
      haftanin_gunu[3] = 'S';
      haftanin_gunu[4]= 'A';
      haftanin_gunu[5]= 'M';
      haftanin_gunu[6] = 'B';
      haftanin_gunu[7]= 'A';
      break;}
      case 5:{
      haftanin_gunu[0] = 'P';
      haftanin_gunu[1]= 'E';
      haftanin_gunu[2]= 'R';
      haftanin_gunu[3] = 'S';
      haftanin_gunu[4]= 'E';
      haftanin_gunu[5]= 'M';
      haftanin_gunu[6] = 'B';
      haftanin_gunu[7]= 'E';
      break;}
      case 6:{
      haftanin_gunu[0] = 'C';
      haftanin_gunu[1]= 'U';
      haftanin_gunu[2]= 'M';
      haftanin_gunu[3]= 'A';
      break;}
      case 7:{
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
      lcd_putc('\f');                          
      lcd_gotoxy(1, 1);                        
      printf(lcd_putc,time1);
      lcd_gotoxy(1, 2);                        
      printf(lcd_putc,saatc);     
}
void gas_func(){
   set_adc_channel(3);
   gazl = read_adc(); 
      delay_ms(10);
      if(gazl>=750){
    while(gazl>=750){
       gazl = read_adc();
       output_high(pin_b1);
       delay_ms(250);         
       output_low(pin_b1);
       delay_ms(250);
       puts(gaz_alarm);}
    }
   else{
      output_low(pin_b1);}
   }
void gsm_func(){   //SABIT BIR IP YE GONDERECEGIM BILGILERIMI SAATLIK OLARAK AYARLADIM
      if(dakikac==00&&saniyec==0){      
      puts(saatc);
      puts(tarihc);
      puts(haftanin_gunu);
      puts(hava1);
      puts(Hava_sicaklik);
      puts(Hava_nem);
      puts(toprak1);
      puts(Toprak_sicaklik);
      puts(Toprak_nem);
      puts(isik_siddeti);
      puts(ruzgar_bilgi);
      puts(ruzgar_sid);
      puts("\n");
      puts(ruzgar_yon);
      puts(yon);}
   }

void ruzgar_func(){ //RUZGAR SENSORUNDEN BILGI OKUMAK ICIN GEREKLI FONKSIYONUMU KURDUM
   set_adc_channel(0);
   deger=read_adc();
   delay_ms(1000);
   if(deger>999){
      ruzgar_sid[4]=(deger%10)+48;
      ruzgar_sid[3]=((deger%100-deger%10)/10)+48;
      ruzgar_sid[2]=((deger%1000-deger%100-deger%10)/100)+48;
      ruzgar_sid[1]=((deger-((deger%1000-deger%100-deger%10)/100))/1000)+48;}
   if(deger>99 && deger<=999){
      ruzgar_sid[3]=(deger%10)+48;
      ruzgar_sid[2]=((deger%100-deger%10)/10)+48;
      ruzgar_sid[1]=((deger%1000-deger%100-deger%10)/100)+48;}
   if(deger<=99){
      ruzgar_sid[2]=(deger%10)+48;
      ruzgar_sid[1]=((deger%100-deger%10)/10)+48;}      
   if(input(pin_b0)==1){        
    yon="Kuzey";}
   if(input(pin_b3)==1){        
    yon="Guney";}
   if(input(pin_b6)==1){       
    yon="Dogu";}   
   if(input(pin_b7)==1){        
    yon="Bati";}
   if(input(pin_b0)==0 && input(pin_b3)==0 && input(pin_b6)==0 && input(pin_b7)==0 ){  
    yon="Yon Yok";}
    
   }
   
 void gunes_func(){ //GUNES SENSORUNDEN BILGI OKUMAK ICIN GEREKLI FONKSIYONUMU KURDUM
   delay_ms(100);
   set_adc_channel(1);
   adc_deger = read_adc();
if(adc_deger>=100){
   adc_deger10=adc_deger%100;
   isik_siddeti[2]=(adc_deger%10)+48;
   isik_siddeti[1]=((adc_deger10-(adc_deger%10))/10)+48;
   isik_siddeti[0]=((adc_deger-adc_deger10)/100)+48;}
else{
   isik_siddeti[2]=(adc_deger%10)+48;
   isik_siddeti[1]=((adc_deger-(adc_deger%10))/10)+48;
   isik_siddeti[0]=' ';}
    }
      
void darbe_func(){ //IVME SENSORUNDEN BILGI OKUMAK ICIN GEREKLI FONKSIYONUMU KURDUM
   set_adc_channel(2);
   deger2=read_adc();
   if(deger2 >=  450){
    lcd_putc('\f');
    lcd_gotoxy(1, 1); 
    printf(lcd_putc,darbe_alarm);
      while(deger2>=450){
    deger2=read_adc();
    output_high(PIN_B1);
    delay_ms(500);
    output_low(PIN_B1);
    delay_ms(500);}}
   } 
  
float kapi_kontrol(void){  //10 SANIYE BOYUNCA 1 SN ACIK 1 SN SONUK OLACAK LEDIMI VE KAPIMI AYARLAYACAGIM FONKSIYONUMU TANIMLADIM
      if(kapic == 'a'){
    k++;
    ka++;
    a=0;
    if(k<6){
       delay_ms(1000);
       output_high(pin_c5);
       delay_ms(1000);
       output_low(pin_c5);
    if(ka<2)
         output_high(PIN_a5);}
       else
          output_high(pin_c5);}
    if(kapic =='k'){
    a++;
    kk++;
    k=0;
    ka=0;
    if(a<6){
       delay_ms(1000);
       output_high(pin_c5);
       delay_ms(1000);
       output_low(pin_c5);
    if(kk<2)
         output_low(PIN_a5);}
       else
          output_low(pin_c5);}
       }
void main(void){ //MAIN FONKSIYONUMU TANIMLADIM
gsm_saat();
delay_ms(1000);
setup_adc_ports(00001111);  
setup_adc(ADC_CLOCK_INTERNAL);
output_drive(PIN_A5);
output_float(PIN_B0);
output_float(PIN_B3);
output_float(PIN_B6);
output_float(PIN_B7);
output_drive(PIN_B1);
output_drive(PIN_C5);
output_low(pin_C5);
setup_psp(psp_disabled);
setup_timer_1(T1_DISABLED);
setup_timer_2(T2_DISABLED,0,1);
setup_ccp1(CCP_OFF);
setup_ccp2(CCP_OFF);
  lcd_init();                                 
  lcd_putc('\f');                             
  rtc_init();  
  rtc_set_datetime(guna,aya,yila,tariha,saata,dakikaa);
   output_drive(ISITICI);
   output_drive(pin_e0);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
  while(TRUE){ //SONSUZ DONGUMU KURDUM
     gsm_kapi();
     time_func();
     kapi_kontrol();
     darbe_func();
     gas_func();
     hava_func();
     toprak_func();
     ruzgar_func();
     If(Sicaklik_Kontrol_Hava <=0 || Sicaklik_Kontrol_Toprak <=0){
   output_low(ISITICI);
   }
   else
   output_high(ISITICI);
   gunes_func();
   gsm_func();
     }
  }

