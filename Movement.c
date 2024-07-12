#include <stdio.h>
#include <math.h>
#define PI 3.1415926

void ServoCtrl(int num, int angle, int speed){
    //angle_300: 转换为0-300的角度。
    ;
}

void Delay_1ms(int t){
    ;
}

double GetAngle_i(double FullAngle, double LinearAngle, int time, 
                  int AngFreq, double phase, int ServoNum){
    return LinearAngle * sin((PI/180) * (AngFreq * time + ServoNum * phase)) + FullAngle;
}

int Detect(){
    ;
    return 0;
    //return 1;
}

//启动，由线型开始直线运动。理想状态下刷新率为每100毫秒一次。
int Serpenoid(int start, double phase, int AngFreq){
    int Detected = 0;//是否检测到障碍
    int time = 0;//时间
    double Angle_last = 0.0;
    double Angle_buff[6] = 0.0;//单位：角度
    double Speed_buff[6] = 0.0;//单位：弧度每秒

    while(1){
        for(int ServoNum = 0; ServoNum < 6; ServoNum++){
            Angle_last = Angle_buff[ServoNum];
            Angle_buff[ServoNum] = GetAngle_i(0, 30, time, 10, 60, ServoNum);
            if(start){//如果是刚启动，需要乘上指数函数。
                Angle_buff[ServoNum] *= (1 - exp(-10*time));
            }
            Speed_buff[ServoNum] = ((PI/180) * (Angle_buff[ServoNum] - Angle_last) * (1000/100));
            ServoCtrl(ServoNum, (int)(150 + (300/720) * Angle_buff[ServoNum]), (int)Speed_buff[ServoNum]);
        }//这里假定所有舵机接受指令的过程可以瞬间完成，接受指令后可以同时工作。

        if(Detect) {//若有障碍则中止运行
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