#include<reg51.h>                                   //include 8051 header files
#define dataport P2                                 //lcd connected to port2
#define adc_input P1                                //adc connected to port1
sbit rd= P3^0;                                      //read pin of adc connected to p3.0
sbit wr= P3^1;                                      //write pin of adc connected to p3.1
sbit intr= P3^2;                                    //intr pin of adc connected to p3.2
sbit rs = P3^3;                                     //rs pin of lcd connected to p3.3
sbit rw = P3^4;                                     //rw pin of lcd connected to p3.4
sbit e = P3^5;                                      //enable pin of lcd connected to p3.5
sbit dc_fan=P3^6;                                   //dc fan is connected to p3.6
void delay(unsigned int);                           //to generate delay
void lcd_cmd(unsigned char);                        //lcd command write
void lcd_data(unsigned char);                       //lcd data write
void lcd_data_string(unsigned char*);               //to display string on lcd
void fan(unsigned int);                             //fan control
void lcd_init();                                    //lcd command initialisation
void adc_conv();                                    //adc start conversion
void adc_read();                                    //read adc output
unsigned char s1[]="fan is on";
unsigned char s2[]="fan is off";





void main()                                         //main function starts
{
    while(1)                                        //infinite loop
    {
        dataport=0x00;                              //lcd as output port
        adc_input=0xff;                             //adc as input port
        P3=0x00;                                    //port3 as output port
        lcd_init();
        adc_conv();
        adc_read();
        delay(50);
    }                                               //infinte loop ends
}                                                   //main ends

void lcd_init()
{
    lcd_cmd(0x01);                                  //clear lcd screen
    delay(1);
    lcd_cmd(0x06);                                  //entry mode set 
    delay(1);
    lcd_cmd(0x0e);                                  //display-cursor on-off
    delay(1);
    lcd_cmd(0x38);                                  //function set
    delay(1);
    lcd_cmd(0x80);                                  //start display in first line
    delay(1);
}

void delay(unsigned int sec )
{
    int i ,j ;
    for(i=0;i<sec;i++)
    for(j=0; j<1275; j++);
}

void lcd_cmd(unsigned char item)
{
    dataport=item;
    rs= 0;                                          //select command register
    rw=0;
    e=1;                                            //enable high to low pulse
    delay(1);
    e=0;
}

void lcd_data(unsigned char item) 
{
    dataport = item;
    rs= 1;                                          //select data register
    rw=0;
    e=1;                                            //enable high to low pulse
    delay(1);
    e=0;
}

void adc_conv()
{                      
    wr = 0;                                         //write low to high pulse to start conversion
    delay(2);                     
    wr = 1;                     
    while(intr);                                    //wait for intr to become 0
    delay(2);
    intr=1;                                         //set intr for next cycle
}

void adc_read()
{
    unsigned int value;                 
    rd = 0;                                         //read low to high to enable the outputs of adc
    delay(2);
    value=adc_input;                                //copy adc_input port to value
    delay(1);
    rd=1; 
    fan(value);                                     //call function to control fan 
}

void fan(unsigned int i)
{
    unsigned int p;
    lcd_init();                                     //lcd initialisation
    if(i<13)                                        //temperature is less than 25
    {
        dc_fan=0;                                   //fan is off
        delay(1000);
        lcd_data_string("fan is off");
        delay(1000);
    }
    else                                            //temperature is more than 25
    {  
        dc_fan=1;                                   //fan is on
        delay(1000);
        lcd_data_string("fan is on");
        delay(1000);
    }
}

void lcd_data_string(char *x)
{
    while(*x!='\0')
    {
        lcd_data(*x);
        delay(2);
        x++;
    }
}
