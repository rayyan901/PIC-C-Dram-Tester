
#include <18F4550.h> 
#include <stdlib.h>

#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,WDT2048
#use delay(clock=48000000)

#use rs232(baud=9600, xmit=PIN_A5, rcv=PIN_A4,stream=pc,errors)

#define  ras  PIN_B2
#define  cas  PIN_B3
#define  wr   PIN_B4
#define  rd   PIN_B5
//============


//=========================== 

set_data(int value) {

#bit data0 = value.0;
#bit data1 = value.1;
#bit data2 = value.2;
#bit data3 = value.3;

output_bit( PIN_A2, data0);
output_bit( PIN_A3, data1);
output_bit( PIN_C6, data2);
output_bit( PIN_C7, data3);

return 0;
}

int read_data() {

int data=0;


if(input(PIN_A2)){
bit_set(data,0);
} else {
bit_clear(data,0);
}

if(input(PIN_A3)){
bit_set(data,1);
} else {
bit_clear(data,1);
}

if(input(PIN_C6)){
bit_set(data,2);
} else {
bit_clear(data,2);
}

if(input(PIN_C7)){
bit_set(data,3);
} else {
bit_clear(data,3);
}


return(data);
}

    
//========================== 
void main() 
{ 
delay_ms(1000);

printf("\r\n\r\n Testing in progress...\r\n");

output_high(ras);
output_high(cas);
output_high(wr);
output_high(rd);
delay_us(200);

output_low(PIN_A0);
output_low(PIN_A1);

long addRow, addCol;
int data,rdata[10];
int init;
long long counter=0,bad_ram=0;
float percent;
//=======

addRow=0;
//====================
output_d(addRow);


for(init=0;init<9;init++) {
output_low(ras);
delay_cycles(2);
output_high(ras);
delay_cycles(2);
}
//===========

while(1) {


int i=0,baki,m=5;
int j=0,k=0,l;
int add10R,add10C;
long addRowR, addColR,count=0;



for(add10C=0;add10C<4;add10C++){

for(addCol=0;addCol<=255;addCol++) {

for(add10R=0;add10R<4;add10R++){

for(addRow=0;addRow<=255;addRow++) {


#bit dataR0 = add10R.0;
#bit dataR1 = add10R.1;
output_bit( PIN_A0, dataR0);
output_bit( PIN_A1, dataR1);

output_d(addRow);
output_low(ras);
delay_cycles(2);

#bit data0 = add10C.0;
#bit data1 = add10C.1;
output_bit( PIN_A0, data0);
output_bit( PIN_A1, data1);

output_d(addCol);
output_low(cas);
delay_cycles(2);


//set_data(k);
m=~m;
set_data(m);
output_low(wr);
delay_cycles(4);
output_high(wr);
output_high(ras);
output_high(cas);

delay_cycles(4);


//===read======
//k=k+2;
//#bit testbit = addRow.0;
baki=addRow%2;
if ((baki==0) && (addRow>0))
//if(testbit)
{
k=0;
//i=0;
addRowR=addRow-2;
addColR=addCol;
for(l=0;l<2;l++)
{


#bit rdataR0 = add10R.0;
#bit rdataR1 = add10R.1;
output_bit( PIN_A0, rdataR0);
output_bit( PIN_A1, rdataR1);

output_d(addRowR);
output_low(ras);
delay_cycles(2);

#bit rdata0 = add10C.0;
#bit rdata1 = add10C.1;
output_bit( PIN_A0, rdata0);
output_bit( PIN_A1, rdata1);


output_d(addColR);
output_low(cas);
delay_cycles(2);

output_low(rd);
delay_cycles(2);
rdata[l]=read_data();
output_high(rd);
output_high(ras);
output_high(cas);
delay_cycles(2);
addRowR++;
//addColR++;

}
percent=(counter/1048576.0)*100.0;
if(addCol==254){
printf("\r %u %Lu tested: %Lu  %f  bad RAM: %Lu d0:%u d1:%u",add10R,addRow,counter,percent,bad_ram,rdata[0],rdata[1]);
}
data=0;
for(l=0;l<2;l++) {
data = data + rdata[l];
//data=rdata[0] + rdata[1] + rdata[2] + rdata[3] + rdata[4];

}
count++;

if (data==15) {
output_high(PIN_B6);
delay_us(1);
output_low(PIN_B6);
delay_us(1);

//i++;
//!if(i<=5)
//!{
//!for(l=0;l<2;l++) {
//!
//!printf("%u: ",rdata[l]);
//!
//!}
//!printf("G \r\n");
//!}
} else {
output_high(PIN_B7);
delay_ms(500);
output_low(PIN_B7);
delay_ms(250);
bad_ram++;
i=0;
printf("%Lu : %u \r\n",count,data);
for(l=0;l<5;l++) {

printf("%u: ",rdata[l]);

}
printf("B\r\n");



}

}
//while(1);
//=====================================



//k++;
counter++;
}
}
}

}
output_high(PIN_B6);
output_high(PIN_B7);
percent=(counter/1048576.0)*100.0;
printf("\r %u %Lu tested: %Lu  %f  bad RAM: %Lu d0:%u d1:%u",add10R,addRow,counter,percent,bad_ram,rdata[0],rdata[1]);
if(bad_ram>0) {
for(i=0;i<bad_ram;i++){
output_high(PIN_B6);
output_low(PIN_B7);
delay_ms(250);
output_low(PIN_B6);
output_high(PIN_B7);
delay_ms(250);

}
}
while(1);

}

}
