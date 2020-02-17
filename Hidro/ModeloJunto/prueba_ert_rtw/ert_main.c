/*
 * File: ert_main.c
 *
 * Code generated for Simulink model 'prueba'.
 *
 * Model version                  : 1.21
 * Simulink Coder version         : 8.13 (R2017b) 24-Jul-2017
 * C/C++ source code generated on : Mon Feb 17 11:16:36 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: AMD->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "prueba.h"                    /* Model's header file */
#include "rtwtypes.h"
#include <stdlib.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <stdint.h>
#include <sched.h>
#include <math.h>
#include <stdbool.h>
#include <semaphore.h>
#include <wiringPiSPI.h>



/*variables para funcionamiento*/
#define NSEC_PER_SEC    1000000000
/*Entradas*/
double Res =20;
/*Salidas*/
double Vd=0;
double Vq=0;
double Md=0;
double Mq=0;
double Mabc=0;

/*Serial*/
int fd;
int bits= 0,bits2= 0,bits3= 0;
char buffer5[24];


/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */

/*funciones para tiempo real*/
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
        __const struct timespec *__req,
        struct timespec *__rem);


static inline void tsnorm(struct timespec *ts)
{
    while (ts->tv_nsec >= NSEC_PER_SEC) {
        ts->tv_nsec -= NSEC_PER_SEC;
        ts->tv_sec++;
    }
}

/*funciones para tiempo real*/

void rt_OneStep(void);
void rt_OneStep(void)
{
    static boolean_T OverrunFlag = false;
    
    /* Disable interrupts here */
    
    /* Check for overrun */
    if (OverrunFlag) {
        rtmSetErrorStatus(rtM, "Overrun");
        return;
    }
    
    OverrunFlag = true;
    
    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */
    
    
    //entradas
    set_R(Res);
    
    
    
    /* Step the model for base rate */
    prueba_step();
    
    /* Get model outputs here */
    
    /*  ------Salidas--------*/
    Vd=get_Vd();
    Vq=get_Vq();
    Md=get_Md();
    Mq=get_Mq();
    Mabc=get_Mabc();
    
    printf("El Vd: %3.2f \n",Vd);
    printf("La Vq: %3.2f \n",Vq);
    printf("La Md: %3.2f \n", Md);
    printf("La Mq: %3.2f \n",Mq);
    printf("La Mabc: %3.2f \n", Mabc);
    
    
    /*Serial*/
    
    /*Comuniacion Serial-- envio datos arduino*/
    
    bits =(int) (((Md+1)*1000));//4095 para DAC
    bits2=(int) (((Mq+1)*1000));//4095 para DAC
    bits3=(int) (((1.5+2)*1000));//4095 para DAC
    
    memset(buffer5,0,sizeof(buffer5));
    
    sprintf(buffer5,"v%07dw%07dz%07d\n",bits,bits2,bits3);
    serialPuts(fd,buffer5);
    serialFlush(fd);
    tcflush(fd,TCIOFLUSH);
    
    /* Indicate task complete */
    OverrunFlag = false;
    
    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
}

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific.  This example
 * illustrates how you do this relative to initializing the model.
 */
int_T main(int_T argc, const char *argv[])
{
    /* Unused arguments */
    (void)(argc);
    (void)(argv);
    
    /*Time*/
    struct timespec t;
    struct sched_param param;
    int interval=100000000; //en nanoseg--4s--1000000000-->-->1 ms pero o se observa bien debido a la comunicacion
    
    
    if(argc>=2 && atoi(argv[1])>0)
    {
        printf("using realtime, priority: %d\n",atoi(argv[1]));
        param.sched_priority = atoi(argv[1]);
        /* enable realtime fifo scheduling */
        if(sched_setscheduler(0, SCHED_FIFO, &param)==-1){
            perror("sched_setscheduler failed");
            exit(-1);
        }
    }
    if(argc>=3)
        interval=atoi(argv[2]);
    
    if (wiringPiSetup () == -1)
        return 1 ;
    pinMode (2, OUTPUT) ;
    
    
    
    fd=serialOpen("/dev/ttyACM0",115200);
    serialClose(fd);
    fd=serialOpen("/dev/ttyACM0",115200);
    sleep(1);
    
    /* get current time */
    clock_gettime(0,&t);
    
    /* start after one second */
    t.tv_sec++;
    
    /* Initialize model */
    prueba_initialize();
    
    /* Attach rt_OneStep to a timer or interrupt service routine with
     * period 0.0001 seconds (the model's base sample time) here.  The
     * call syntax for rt_OneStep is
     *
     *  rt_OneStep();
     */
    printf("Warning: The simulation will run forever. "
            "Generated ERT main won't simulate model step behavior. "
            "To change this behavior select the 'MAT-file logging' option.\n");
    fflush((NULL));
    while (rtmGetErrorStatus(rtM) == (NULL)) {
        /*  Perform other application tasks here */
        /* wait untill next shot */
        clock_nanosleep(0, TIMER_ABSTIME, &t, NULL);
        /* do the stuff */
        
        if(estado==0){
            estado=1;
        }
        else{
            estado=0;
        }
        digitalWrite (2, estado) ;
        
        rt_OneStep();
    }
    
    t.tv_nsec+=interval;
    tsnorm(&t);
    
    /* Disable rt_OneStep() here */
    return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
