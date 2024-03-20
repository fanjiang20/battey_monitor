
#include <Arduino.h>
#include <U8g2lib.h>
#include <IRremote.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//适用于RC5红外协议13bit marantz Remote.或者其他遥控32Bit.
#define DECODE_RC5
#define OFF    0x1C7B        //      白板按键--定义关机键
#define Coax   0x1462        //A     按键定义同轴输入
#define STLink 0x1468        //B     按键定义光纤输入
#define loadSW 0x1478        //C     按键定义负载断开控制信号
#define Power  0x1477        //D     按键定义开机
//#define LK     0x00


//液晶屏型号IO连接信息 
U8G2_SSD1322_NHD_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); // Enable U8G2_16BIT in u8g2.h

//按键功能定义
 const int LK    = 6;  //左键
 const int RK    = 4;  //右键
 const int PLAY  = 7;  //播放键
 const int STOP  = 5;  //停止键

 //ADC模拟量采集IO
 const int ADCP = 18;   //  正压电池组电压监控ADC0  Range:0~5v  5V=+16.8v
 const int ADCN = 19;   //  负压电池组电压监控ADC1  Range:0~5v  5V=-16.8V 
 
 //遥控接受头IO
 const int recvPin              = 12;     // 红外遥控接收头Pin

 //输出端口
 const int Mute_sig             = 20;     // 禁音电路,或者开机启动静音.静音红灯亮起时表示静音在工作 逻辑电平：1=静音，0=不动作
 const int Total_POWER_Signal   = 21;     // 低通部分数字电源部分，V2.0PWE板子关机信号信号（芯片shutdown控制-有效高电平，板上绿灯亮0=OFF;1=ON）
 const int LOAD_Relay           = 22;     // 脱负载继电器信号，断开或接通负载；   高有效，逻辑电平1=ON;0=OFF
 const int ST_SET               = 23;     // RCA和ST光纤选 择信号。0=RCA输入，1=ST光纤输入。
 


IRrecv recv(recvPin);  // 初始化
decode_results res;    // 保存读取到的数据


void setup(void) {

//设定输入IO

  pinMode(LK,    INPUT);
  pinMode(RK,    INPUT);
  pinMode(PLAY,  INPUT);
  pinMode(STOP,  INPUT);

//设定输出IO
  pinMode(Mute_sig,            OUTPUT);  //
  pinMode(Total_POWER_Signal,  OUTPUT);  //
  pinMode(LOAD_Relay,          OUTPUT);  //
  pinMode(ST_SET ,             OUTPUT);  //


//初始化输出IO
     digitalWrite(Mute_sig,           LOW);  //开机静音
     digitalWrite(Total_POWER_Signal, LOW);  //数字信号控制电源板上LDO shutdown 关机
     digitalWrite(LOAD_Relay,         LOW);  //脱载信号=断开或者接通负载。
     digitalWrite(ST_SET,             LOW);  //同轴、光纤选择信号 LOW=同轴输入使能；HIGH=ST光纤信号使能

 Serial.begin(9600);    // 初始化串口设置波特率9600
 recv.enableIRIn();     // 启用IR红外遥控数据接收
 u8g2.begin();          // U8G2 begin 初始化
 u8g2.firstPage();      //  首页初始化
  
  /*************************************************************开机主页显示内容******************************************************************************/
  do {
       u8g2.setFont(u8g2_font_ncenB14_tr);           //设置为14号字体
       u8g2.drawStr(60,20,"Nangua R-2R DAC");        //第一行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);    //设置为10号字体
       u8g2.drawStr(60,30,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
       
       u8g2.setFont(u8g2_font_courR10_tr);           //设置为10号字体
       u8g2.drawStr(65,40,"24Bit 384KHz HDCD");      //第3行显示内容
       
       u8g2.setFont(u8g2_font_finderskeepers_tr);        //设置为10号字体
       u8g2.drawStr(90,50,"8xPCM1704uK PMD200");         //第4行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);         //设置为10号字体
       u8g2.drawStr(65,60,"->Battery supply is not turned ON"); //第5行显示内容
      } 
      while( u8g2.nextPage() );
 
    }



void loop(void) {

   
if(recv.decode(&res))
      {
         switch(res.value)
      {
/*************************************************************power按下*********************************************************************/    
case OFF: //白板按键
     Serial.print("irCode 0x:");            // 串口打印IRcode
     Serial.print(res.value,HEX);           // 16进制映射收到的编码的值
     Serial.print(",bits:");                // 显示映射字节数量
     Serial.println(res.bits);              // 
   
     digitalWrite(Mute_sig,           HIGH);  //静音
     digitalWrite(Total_POWER_Signal, LOW);  //数字信号控制电源板上LDO shutdown 关机
     digitalWrite(LOAD_Relay,         LOW);  //脱载信号=断开负载。
     digitalWrite(ST_SET,             LOW);  //同轴、光纤选择信号 LOW=同轴输入使能；HIGH=ST光纤信号使能

     u8g2.firstPage();
     
  do {
       u8g2.setFont(u8g2_font_ncenB14_tr);           //设置为14号字体
       u8g2.drawStr(70,20,"Power off");        //第一行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);    //设置为10号字体
       u8g2.drawStr(5,30,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 

       
       
       u8g2.setFont(u8g2_font_courR10_tr);           //设置为10号字体
       u8g2.drawStr(10,40,"Now turn off your DAC");      //第二行显示内容
       
       
      
     } 
        while(u8g2.nextPage() );
         break;

case Coax:  //按键:A 同轴信号输入
     Serial.print("irCode 0x:");            // 串口打印IRcode
     Serial.print(res.value,HEX);           // 16进制映射RES的值
     Serial.print(",bits:");                // 显示映射字节数量
     Serial.println(res.bits);              //  

     digitalWrite(Mute_sig,           LOW);  //放音
     digitalWrite(Total_POWER_Signal, HIGH);  //数字信号控制电源板上电。
     digitalWrite(LOAD_Relay,         HIGH);  //脱载信号接通负载。
     digitalWrite(ST_SET,             LOW);  // LOW=同轴输入使能。
     u8g2.firstPage();
  do {
       u8g2.setFont(u8g2_font_ncenB14_tr);           //设置为14号字体
       u8g2.drawStr(60,20,"Nangua R-2R DAC");        //第一行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);    //设置为10号字体
       u8g2.drawStr(60,30,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
       
       u8g2.setFont(u8g2_font_courR10_tr);             //设置为10号字体
       u8g2.drawStr(65,40,"24Bit 384KHz HDCD");         //第3行显示内容
       
       u8g2.setFont(u8g2_font_finderskeepers_tr);        //设置为10号字体
       u8g2.drawStr(90,50,"8xPCM1704uK PMD200");         //第4行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);         //设置为10号字体
       u8g2.drawStr(90,60,"->Coax S/Pdif input");     //第5行显示内容
      
  } 
      while( u8g2.nextPage() );
      break; 

case STLink:  //按键:B 同轴信号输入
     Serial.print("irCode 0x:");            // 串口打印IRcode
     Serial.print(res.value,HEX);           // 16进制映射RES的值
     Serial.print(",bits:");                // 显示映射字节数量
     Serial.println(res.bits);              //  

     digitalWrite(Mute_sig,           LOW);  //放音
     digitalWrite(Total_POWER_Signal, HIGH);  //数字信号控制电源板上电。
     digitalWrite(LOAD_Relay,         HIGH);  //脱载信号接通负载。
     digitalWrite(ST_SET,             HIGH);  // LOW=同轴输入使能。
     u8g2.firstPage();
  do {
       u8g2.setFont(u8g2_font_ncenB14_tr);           //设置为14号字体
       u8g2.drawStr(60,20,"Nangua R-2R DAC");        //第一行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);    //设置为10号字体
       u8g2.drawStr(60,30,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
       
       u8g2.setFont(u8g2_font_courR10_tr);             //设置为10号字体
       u8g2.drawStr(65,40,"24Bit 384KHz HDCD");         //第3行显示内容
       
       u8g2.setFont(u8g2_font_finderskeepers_tr);        //设置为10号字体
       u8g2.drawStr(90,50,"8xPCM1704uK PMD200");         //第4行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);         //设置为10号字体
       u8g2.drawStr(90,60,"->ST-link S/Pdif input");     //第5行显示内容
      
  } 
      while( u8g2.nextPage() );
      break; 

case loadSW:  //按键:C 同轴信号输入
     Serial.print("irCode 0x:");            // 串口打印IRcode
     Serial.print(res.value,HEX);           // 16进制映射RES的值
     Serial.print(",bits:");                // 显示映射字节数量
     Serial.println(res.bits);              //  

     digitalWrite(Mute_sig,           HIGH);  //放音
     digitalWrite(Total_POWER_Signal, LOW);  //数字信号控制电源板上电。
     digitalWrite(LOAD_Relay,         LOW);  //脱载信号接通负载。
     digitalWrite(ST_SET,             LOW);  // LOW=同轴输入使能。
     u8g2.firstPage();
  do {
       u8g2.setFont(u8g2_font_ncenB14_tr);           //设置为14号字体
       u8g2.drawStr(60,20,"Nangua R-2R DAC");        //第一行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);    //设置为10号字体
       u8g2.drawStr(60,30,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
       
       u8g2.setFont(u8g2_font_courR10_tr);             //设置为10号字体
       u8g2.drawStr(65,40,"24Bit 384KHz HDCD");         //第3行显示内容
       
       u8g2.setFont(u8g2_font_finderskeepers_tr);        //设置为10号字体
       u8g2.drawStr(90,50,"8xPCM1704uK PMD200");         //第4行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);         //设置为10号字体
       u8g2.drawStr(90,60,"LOAD POWER DISABLE");     //第5行显示内容
      
  } 
      while( u8g2.nextPage() );
      break;


case Power:  //按键:D 同轴信号输入
     Serial.print("irCode 0x:");            // 串口打印IRcode
     Serial.print(res.value,HEX);           // 16进制映射RES的值
     Serial.print(",bits:");                // 显示映射字节数量
     Serial.println(res.bits);              //  

     digitalWrite(Mute_sig,           LOW);  //放音
     digitalWrite(Total_POWER_Signal, HIGH);  //数字信号控制电源板上电。
     digitalWrite(LOAD_Relay,         HIGH);  //脱载信号接通负载。
     digitalWrite(ST_SET,             LOW);  // LOW=同轴输入使能。
     u8g2.firstPage();
  do {
       u8g2.setFont(u8g2_font_ncenB14_tr);           //设置为14号字体
       u8g2.drawStr(60,20,"Nangua R-2R DAC");        //第一行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);    //设置为10号字体
       u8g2.drawStr(60,30,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
       
       u8g2.setFont(u8g2_font_courR10_tr);             //设置为10号字体
       u8g2.drawStr(65,40,"24Bit 384KHz HDCD");         //第3行显示内容
       
       u8g2.setFont(u8g2_font_finderskeepers_tr);        //设置为10号字体
       u8g2.drawStr(90,50,"8xPCM1704uK PMD200");         //第4行显示内容
       u8g2.setFont(u8g2_font_finderskeepers_tr);         //设置为10号字体
       u8g2.drawStr(90,60,"->All power supply turn ON");     //第5行显示内容
      
  } 
      while( u8g2.nextPage() );
      break;


 
     default:
     ;
     break;
      }
 
      recv.resume();

    }

 }
  
