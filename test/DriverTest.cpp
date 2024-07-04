#include <stdio.h>
#include <stdlib.h> // exit 함수 사용을 위해 추가
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#define WRITE_BUF_SIZE 10
#define MOTOR_OFF '0'
#define MOTOR_FORWARD '1'
#define MOTOR_BACKWARD '2'

char buf_to_GlassMotor[WRITE_BUF_SIZE] ={'9'};
char buf_to_ShieldMotor[WRITE_BUF_SIZE] = {'9'};

int main() {
    char option;
    int GlassDriver;
    int ShieldDriver;

    GlassDriver = open("/dev/GlassMotorDriver", O_RDWR);
    ShieldDriver = open("/dev/ShieldMotorDriver", O_RDWR);
    
    //Check Driver Connection is Valid
    if(GlassDriver == -1) {
        printf("Cannot Open Motor Driver File\n");
        return 0;
    }


    if(ShieldDriver == -1) {
        printf("Cannot Open Motor Driver File\n");
        return 0;
    }

    




    while(1) {
        printf("--------------------------\n");
        printf("      Select Option       \n");
        printf("      1. Glass Motor Off        \n");
        printf("      2. Glass Motor Forward    \n");
        printf("      3. Glass Motor Backward   \n");
        printf("      4. Shield Motor off        \n");
        printf("      5. Shield Motor Forward        \n");
        printf("      6. Shield Motor Backward        \n");
        printf("      7. Stop Both Motor        \n");
        printf("      8. End Test        \n");

        printf("--------------------------\n");

        scanf(" %c", &option);
        getchar(); // 개행 문자 소비

        switch(option) {
            case '1':
                buf_to_GlassMotor[0] = MOTOR_OFF;
                break;
            case '2':
                buf_to_GlassMotor[0] = MOTOR_FORWARD;
                break;
            case '3':
                buf_to_GlassMotor[0] = MOTOR_BACKWARD;
                break;

            case '4':
                buf_to_ShieldMotor[0] = MOTOR_OFF;
                break;
            
            case '5':
                buf_to_ShieldMotor[0] = MOTOR_FORWARD;
                break;
            
            case '6':
                buf_to_ShieldMotor[0] = MOTOR_BACKWARD;
                break;
            
            case '7':
                buf_to_ShieldMotor[0] = MOTOR_OFF;
                buf_to_GlassMotor[0] = MOTOR_OFF;
                break;

            case '8':
                buf_to_GlassMotor[0] = MOTOR_OFF;
                buf_to_ShieldMotor[0] = MOTOR_OFF;
                printf("Test Off\n");
                write(GlassDriver, buf_to_GlassMotor, 1);
                write(ShieldDriver,  buf_to_ShieldMotor, 1);
                close(GlassDriver);
                close(ShieldDriver);

                exit(1);
            default:
                printf("Enter Valid Option\n");
                continue; 
        }

        //Only Send MSB to Driver ( buf_to_GlassMotor [0])
        if(write(GlassDriver, buf_to_GlassMotor, 1) < 1) {
            printf("Error in Glass Motor Control\n");
            close(GlassDriver);
            return 0;
        }

        //Only Send MSB to Driver ( buf_to_ShieldMotor [0])
        if(write(ShieldDriver, buf_to_ShieldMotor, 1) < 1) {
            printf("Error in Sheild Motor Control\n");
            close(ShieldDriver);
            return 0;
        }



        printf("Command sent: %c\n", option);
    }

     close(GlassDriver);
     close(ShieldDriver);
    return 0;
}
