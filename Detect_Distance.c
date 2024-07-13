#include "project.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#define BUFFER_LEN              64

#define CLK2_FREQ               4

#define ROW_0                   0
#define ROW_1                   1
#define COLUMN_0                0 
#define COLUMN_8                8
#define COLUMN_12               12

#define K_DIST                  1.2

uint32 period;

uint32 GetCount()
{
    uint32 count[6];
    uint32 avcount;
    //uint32 s_2;
    int i=0;
    for(;i<4;i++)
    {
        count[i] = period;
        while(!Echo_Read())
        {};
        while(Echo_Read())
        {};
        count[i] -= Timer_2_ReadCounter();
    }
    //Get 4 distances, use the average
    avcount=(count[0]+count[1]+count[2]+count[3])/4;//+count[4]+count[5])/6;
    /*
    s_2=(((count[0]-avcount)*(count[0]-avcount)
         +(count[1]-avcount)*(count[1]-avcount)
         +(count[2]-avcount)*(count[2]-avcount)
         +(count[3]-avcount)*(count[3]-avcount))/4);
    //   +(count[4]-avcount)*(count[4]-avcount)
    //   +(count[5]-avcount)*(count[5]-avcount))/6);
    
    if(sqrt(s_2)*30 > avcount)
    {
        return UINT32_MAX;
    }*/
    return avcount;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Clock_2_Start();
    PWM_1_Start();
    Timer_2_Start();
    LCD_Start(); 
    LED3_Write(0);
    UART_1_Start();
    Timer_2_SetCaptureMode(Timer_2__B_TIMER__CM_RISINGEDGE);
    USBUART_1_Start(0u, USBUART_1_3V_OPERATION);
    
    period = Timer_2_ReadPeriod();    
    uint32 count = 0;
    uint16 mmdist;
    char buff[BUFFER_LEN];
    uint8  flag = 0;
    //uint16 buff = 0;
    const uint8 yaoyaoling = 0x6;
    
    LCD_Position(ROW_0,COLUMN_8);
    LCD_PrintString("                ");
    LCD_Position(ROW_0,COLUMN_8);
    LCD_PrintString("=count");
    LCD_Position(ROW_1,COLUMN_8);
    LCD_PrintString("                ");
    LCD_Position(ROW_1,COLUMN_8);
    LCD_PrintString("=mmdist");

    for(;;)
    {
        /* Place your application code here. */
        do{
            count=GetCount();
        }
        while(count==UINT32_MAX);
        
        mmdist= K_DIST * count * 0.17 / CLK2_FREQ;
        //mmdist=(mmdist>10000? 10000 : mmdist);
        if(mmdist > 10000)continue;
        LCD_Position(ROW_0,COLUMN_0);
        LCD_PrintString("        ");
        LCD_Position(ROW_0,COLUMN_0);
        LCD_PrintU32Number(count);
        LCD_Position(ROW_1,COLUMN_0);
        LCD_PrintString("        ");
        LCD_Position(ROW_1,COLUMN_0);
        LCD_PrintU32Number(mmdist);
        
        if(mmdist<200){
            flag+=(250-mmdist)/50;
        }
        else if(flag>0){
            flag--;
        }
        
        if(flag>=3){
            flag=3;
            LED3_Write(1);
            if(USBUART_1_CDCIsReady() != 0u){
                USBUART_1_PutData(&yaoyaoling,1);            
            }
            UART_1_PutChar('!');
        }
        else {
            if(USBUART_1_CDCIsReady() != 0u){
                sprintf(buff,"Distance: %d mm\n",mmdist);
                USBUART_1_PutString(buff); 
            }
            if(flag==0){
                LED3_Write(0);
            }
            UART_1_PutChar('=');
        }
    }
}

/* [] END OF FILE */
