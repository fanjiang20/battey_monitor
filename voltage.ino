// 监控电池组电压项目battery voltage monitor alarm (BVMA V1.0)
// to 16x2 LCD via I2C
//MCU参考电压:4.096V，由MCP1541 提供
//AD8275数据表 Rev.B Page 13 of 16  Figure 37. shard Refrernce  电路应用，+VS=5.0V, REF1=REF2=0V.
//测量范围：最大输入电压+24.5V to 0.2V(根据参考电压4.096v限制测量范围+0.2~+20.2v)，A0 High:4.95v Swing:4.95v Low:0.045V。
//版本：BVMA1.0
//项目名称代号：电池低电压报警提示 battery voltage monitor alarm (BVMA V1.0)
//功能描述：锂电池组由4S（4串3200mA/h）监控主锂电池电压状态，当低于12V时D13点亮提示需要充电。
//锂电组：4S 3200mA/h，4串，放电电压范围：10.8~16.8V
// 2022/9/12 pm 22:14



#include <LedDisplay.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //LCD1602液晶地址0x27.


#define led_pin 13 //定义D13为欠压指示.


int ADCA = A0; //指定A0端口读取AD8275 负电压.
int ADCB = A1; //指定A1端口读取AD8275 正电压.

float Bin; //存储AD8275电压.
float Bvoltage; //存储电压测量结果.
float Ain; //存储AD8275电压.
float Avoltage; //存储电压测量结果.


#define dataPin 2              // connects to the display's data in
#define registerSelect 3       // the display's register select pin
#define clockPin 4             // the display's clock pin
#define enable 5               // the display's chip enable pin
#define reset 6               // the display's reset pin

#define displayLength 8        // number of characters in the display

LedDisplay myDisplay = LedDisplay(dataPin, registerSelect, clockPin,
                                  enable, reset, displayLength);

int brightness = 10;        // screen brightness





void setup()     //主函数、仅执行一次.
{
lcd.init ();        //初始化LCD.
lcd.home ();    //首页.
lcd.backlight(); //初始化背光.
Serial.begin(9600); //初始化串口. 

lcd.init ();        //初始化LCD.

myDisplay.begin();
myDisplay.clear();
myDisplay.setBrightness(brightness);



  
analogReference(EXTERNAL);    //使用外部参考电压 ，参考电压4.096V接Ref pin.
pinMode(led_pin, OUTPUT);       // D13为输出.
digitalWrite(led_pin, LOW);        //初始化写入D13为LOW电平.
pinMode(ADCA, INPUT);       // A0
pinMode(ADCB, INPUT);       // A1
}




void loop()
{
  printVoltsA();          //读取ADC电压子函数
  delay(500);
  printVoltsB(); 
  //myDisplay.print("{000000]");
  //myDisplay.print(voltage);  

  if(Avoltage<12.00,Bvoltage<12.00){digitalWrite(led_pin, HIGH); }  //判断A0,A1任意一个电压低于12V将点亮D13，提示电压不平衡，急需充电
  else 
  {digitalWrite(led_pin, LOW); }
}


 
 void printVoltsA()     //ADC采集电压子程序
{
 Ain = analogRead(ADCA) *4.096/1024; //计算出ADC0的电压，单位为V（re参考电压4.096V）转换每级0.016V.
 Avoltage =20.44-(Ain *5.01); //将A0电压4.1(max)要转换成4.829mV 如显f示负压使用：(Ain *5.01)-20.44
 Serial.print("-VEE= "); //在串口监视器输出结果
Serial.print(Avoltage);
 Serial.println("V");
delay(100);
lcd.setCursor(0,0);
lcd.print("vol = ");
lcd.print(Avoltage); //print the voltage to LCD
lcd.print("V");
 



         if(Avoltage<=16.80 && Avoltage>15.00){myDisplay.setCursor(0);myDisplay.print("<>>|"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
    else if(Avoltage<=15.00 && Avoltage>14.00){myDisplay.setCursor(0);myDisplay.print(" >>|"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
    else if(Avoltage<=14.00 && Avoltage>13.00){myDisplay.setCursor(0);myDisplay.print("  >|"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
    else if(Avoltage<=13.00 && Avoltage>12.00){myDisplay.setCursor(0);myDisplay.print("   |"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
else { 
myDisplay.setCursor(0);myDisplay.print(" ");
myDisplay.setCursor(1);myDisplay.print(" ");
myDisplay.setCursor(2);myDisplay.print(" ");
myDisplay.setCursor(3);myDisplay.print(" "); 

lcd.setCursor(0,1);
lcd.print("Low BAT");
}




}

void printVoltsB()     //ADC采集电压子程序
{
 Bin = analogRead(ADCB) *4.096/1024; //计算出ADC0的电压，单位为V（ref参考电压4.096V）转换每级0.016V.
 Bvoltage = Bin*5.01; //将A0电压4.1(max)要转换成4.829mV
 Serial.print("+VCC="); //在串口监视器输出结果
 Serial.print(Bvoltage);
 Serial.println(" *V");
delay(100);
lcd.setCursor(1,1);
lcd.print("Bol = ");
lcd.print(Bvoltage); //print the voltage to LCD
lcd.print("V");
 



         if(Bvoltage<=16.80 && Bvoltage>15.00){myDisplay.setCursor(4);myDisplay.print("<>>+"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
    else if(Bvoltage<=15.00 && Bvoltage>14.00){myDisplay.setCursor(4);myDisplay.print(" >>+"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
    else if(Bvoltage<=14.00 && Bvoltage>13.00){myDisplay.setCursor(4);myDisplay.print("  >+"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
    else if(Bvoltage<=13.00 && Bvoltage>12.00){myDisplay.setCursor(4);myDisplay.print("   +"); lcd.setCursor(0,1);lcd.print("~Full  battery");}
else { 
myDisplay.setCursor(4);myDisplay.print(" ");
myDisplay.setCursor(5);myDisplay.print(" ");
myDisplay.setCursor(6);myDisplay.print(" ");
myDisplay.setCursor(7);myDisplay.print(" "); 

lcd.setCursor(0,1);
lcd.print("Low  battery");
}




}
