# battey_monitor
Dual track battery pack voltage monitoring
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
