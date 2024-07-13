/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h> 
#include <stdio.h>
#include <math.h>
#define PI 3.1415926
#include <string.h>
 
#if defined (__GNUC__) 
    /* Add an explicit reference to the floating point printf library */     
    /* to allow the usage of floating point conversion specifiers. */     
    /* This is not linked in by default with the newlib-nano library. */     
    asm (".global _printf_float"); 
#endif 

void ServoCtrl(int num, int angle, int speed){
    //angle_300: 转换为0-300的角度。
    //速度用不着
    int angle_1024 = angle * 1024 / 300;
    uint8 angle_lo = angle_1024 & 0xff;
    uint8 angle_hi = (angle_1024 / 256) & 0xff;
    speed = speed;
    int check = 0xff - (0x2a + num + angle_lo + angle_hi);
    while(check<0){
        check += 255;
    }
    check = check & 0xff;
    char8 buff[11];
    sprintf(buff, "%c%c%c%c%c%c%c%c%c%c%c",0xff,0xff,num,0x07,0x04,0x1e,angle_lo,angle_hi,0x00,0x01,check);
    UART_1_PutArray((const uint8*)buff,11);//FF FF xx 07 04 1E lo hi 00 01 check
}

int Servo_do(){//发送"FF FF FE 02 05 FA"
    char8 buff[6];
    sprintf(buff, "%c%c%c%c%c%c",0xff,0xff,0xfe,0x02,0x05,0xfa);
    UART_1_PutArray((const uint8*)buff,6);
    return 0;
}

void Delay_1ms(int t){
    CyDelay(t);
}

double GetAngle_i(double FullAngle, double LinearAngle, int time, 
                  int AngFreq, double phase, int ServoNum){
    return LinearAngle * sin(((double)(PI/180)) * (AngFreq * time + ServoNum * phase)) + FullAngle;
}

int Detect(){
    ;
    return 0;
    //return 1;
}

//启动，由线型开始直线运动。理想状态下刷新率为每100毫秒一次。
int Serpenoid(int start){
    int Detected = 0;//是否检测到障碍
    int time = 0;//时间
    double Angle_last = 0.0;
    double Angle_buff[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//单位：角度
    double Speed_buff[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//单位：弧度每秒

    while(1)
    {
        for(int ServoNum = 0; ServoNum < 6; ServoNum++){
            Angle_last = Angle_buff[ServoNum];
            Angle_buff[ServoNum] = GetAngle_i(0, 30, time, 10, 60, ServoNum);
            if(start){//如果是刚启动，需要乘上指数函数。
                Angle_buff[ServoNum] *= (1 - exp(-10*time));
            }
            Speed_buff[ServoNum] = ((PI/180) * (Angle_buff[ServoNum] - Angle_last) * (1000/100));
            ServoCtrl(ServoNum, (int)(150 + (300/720) * 90), (int)Speed_buff[ServoNum]);
        }//这里假定所有舵机接受指令的过程可以瞬间完成，接受指令后可以同时工作。
        Servo_do();//广播执行
        time++;
        
        if(Detect()) {//若有障碍则中止运行
            Detected = 1;
            break;
        }
        Delay_1ms(100);//可能不需要这一行，因为Detect的过程本身就消耗时间。
    }
    return Detected;
}

int Serpenoid_curve(){


    return 0;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start(); 
    //UART_1_Init(); 

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    //for(;;)
    {
        Serpenoid(0);
        CyDelay(1000);
/* Place your application code here. */
    }
}

/* [] END OF FILE */
