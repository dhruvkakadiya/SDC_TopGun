/*
 * motor_controller.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Dhruv
 */
#include "motor_controller.hpp"

QueueHandle_t motor_task= xQueueCreate(10,sizeof(motor_direction));

Motor motors;
PWM pwmServo(PWM::pwm1,50); //As per waveform and real testing(percent range - 6.0(right) - 7.5(center) - 9.3(left))
PWM pwmDC(PWM::pwm2,50);   //As per waveform only(percent range - 5.5(forward) - 8.5(stop) - 10.5(backward))

void set_motors_pwm(void) {
// Waiting here in Queue till CAN Rx sends structure pointer in Queue.
    motor_direction *md1;
    if(!xQueueReceive(motor_task, md1, 1)) {
        puts("Failed to receive item within 1000 ms");
        return;
       }

    if(md1->turn==0) {             // turn left
               pwmServo.set(7.5);      // Keep servo straight
               printf("Motor Task : Turn left \n");
           }
    else if(md1->turn==1) {             // turn slight left
               pwmServo.set(7.5);      // Keep servo straight
               printf("Motor Task : Turn Slight left \n");
           }
    else if(md1->turn==2) {             // keep straight
                pwmServo.set(7.5);      // Keep servo straight
                printf("Motor Task : Keep Straight \n");
            }
    else if(md1->turn==3) {             // turn slight right
                pwmServo.set(7.5);      // Keep servo straight
                printf("Motor Task : Turn Slight Right \n");
            }
    else if(md1->turn==4) {             // turn right
                   pwmServo.set(7.5);      // Keep servo straight
                   printf("Motor Task : Turn Right \n");
               }

    //--------------------------- Speed of DC Motor ------------------------
    if(md1->speed==0) {             // Slow Speed
                      pwmDC.set(7.5);         // As per real testing -> Keep rotate motor in forward direction
                      printf("Motor Task : Slow Speed \n");
                  }
    else if(md1->speed==1) {             // Normal Speed
                         pwmDC.set(7.0);         // As per real testing -> Keep rotate motor in forward direction
                         printf("Motor Task : Normal Speed \n");
                     }
    else if(md1->speed==2) {             // Fast Speed
                         pwmDC.set(6.5);         // As per real testing -> Keep rotate motor in forward direction
                         printf("Motor Task : Fast Speed \n");
                     }
    else if(md1->speed==3) {             // Turbo Speed
                         pwmDC.set(6.0);         // As per real testing -> Keep rotate motor in forward direction
                         printf("Motor Task : Turbo Speed \n");
                     }
    else if(md1->speed==4) {             // Stop -> Zero Speed
                           pwmDC.set(8.0);
                           printf("Motor Task : Stop the car.\n");
                       }

}

void can_motor_rx_task(void) {
    can_msg_t motor_msg;
    motor_msg.data.qword = 0;
    if(receive_data()){
        if(motor_msg.msg_id==0x0D) {
               motor_direction *m1;
               m1 =(motor_direction *) &motor_msg.data.qword;

               // Sending queue to motor task with structure.
               if(!xQueueSend(motor_task, &m1, 1)) {
                           puts("Failed to send item to queue within 2ms");
                       }

/*
            if(message2.data.bytes[0]==0x00){
                s_mode=slow;
                md->speed=s_mode;
                printf("CAN Rx: Slow down the speed \n");
            }
            else if(message2.data.bytes[0]==0x01){
                s_mode=normal;
                md->speed=s_mode;
                printf("CAN Rx: Normal Speed \n");
            }
            else if(message2.data.bytes[0]==0x02){
                s_mode=fast;
                md->speed=s_mode;
                 printf("Can Rx: Fast Speed\n");
              }
            else if(message2.data.bytes[0]==0x03){
                s_mode=turbo;
                md->speed=s_mode;
                 printf("CAN Rx: Turbo speed\n");
              }
            else if(message2.data.bytes[0]==0x04){
                           s_mode=stop;
                           md->speed=s_mode;
                            printf("CAN Rx: Stop the car.\n");
                         }
            //left,s_left,straight,s_right,right
            if(message2.data.bytes[1]==0x00){
              motor_dir_t=left;
              md->turn=motor_dir_t;
              printf("CAN Rx: Turn Left\n");
                }
            else if(message2.data.bytes[1]==0x01){
                motor_dir_t=s_left;
                 md->turn=motor_dir_t;
                 printf("CAN Rx: Turn slight Left\n");
              }
            else if(message2.data.bytes[1]==0x02){
                motor_dir_t=straight;
                md->turn=motor_dir_t;
                printf("CAN Rx: Be Straight\n");
              }
            else if(message2.data.bytes[1]==0x03){
              motor_dir_t=s_right;
              md->turn=motor_dir_t;
              printf("CAN Rx: Turn slight right \n");
              }
            else if(message2.data.bytes[1]==0x04){
               motor_dir_t=right;
               md->turn=motor_dir_t;
               printf("CAN Rx: Turn Right\n");
             }

        */
        }
    }
}


