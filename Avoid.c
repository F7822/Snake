/*
加速运动？


*/
#include "Avoid.h"

//
//在0.5m内没有障碍物
void Go_Straight(){

}
//0.5m检测到障碍物，先向右转45度，以45度为中心的60度角内检测，如果没有障碍物则前进
uint8_t Turn_Right();
//如果右边有障碍物，则向左
uint8_t Turn_Left();