#include <16F877A.h>
#fuses xt,nowdt,noprotect,nobrownout,nolvp,noput,nowrt,nodebug,nocpd            
#use delay(clock = 8000000)
#use rs232(baud=9600,xmit=pin_d0,rcv=pin_d1,parity=N,stop=1) 

char gunaa[5];
char aya[5];
char yila[5];
char tariha[5];
char saata[5];
char dakikaa[5];
char buttonc;
int gun=05;
int ay=01;
int yil=21;
int tarih=3;
int saat=07;
int dakika=59;

int main (void){ 
    sprintf(gunaa,"%d",gun);
    sprintf(aya,"%d",ay);
    sprintf(yila,"%d",yil);
    sprintf(tariha,"%d",tarih);
    sprintf(saata,"%d",saat);
    sprintf(dakikaa,"%d",dakika);
    delay_ms(500);
   puts(gunaa);
    delay_ms(500);
   puts(aya);
    delay_ms(500);
   puts(yila);
    delay_ms(500);
   puts(tariha);
    delay_ms(500);
   puts(saata);
    delay_ms(500);
   puts(dakikaa);
    delay_ms(500);
    output_float(PIN_C5);    
    while(true){
      if(input(PIN_C5) == 1){
    if(input(PIN_C5) == 1){
       putc('a');
        delay_ms(500);}}
      if(input(PIN_C5) == 0){
    if(input(PIN_C5) == 0){
       putc('k');
        delay_ms(500);}}

 }
 }
