/*
 * imu.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Akshay Vijaykumar
 */

#include <stdlib.h>             // strtof()
#include "imu.hpp"
#include "io.hpp"
#include "file_logger.h"
#include "geo_controller.hpp"
#include <stdio.h>
#include "stdlib.h"


/* TO DO:
 * 1. Current Queue size is 1. The producer queues 1 value every 50 ms
 * while the consumer de-queues every 100ms. Modify the size so that every read
 * by the consumer returns the most recent data.
 * */

/* Constructor
 * Initializes UART used to interface with the IMU
 */
imu::imu():
    imuUart(Uart3::getInstance()),
    imuResetPin(P2_2),
    readYawCommand{'#', 'f'},
    imu_old_heading(0.0),
        err_count(0),
    queueHandle(xQueueCreate(IMU_INTERNAL_QUEUE_LENGTH, IMU_BUFFER_SIZE))

{
    // Load Command String to Array

    CUSTOM_DEBUG("IMU : readYawCommand = %c%c", readYawCommand[0], readYawCommand[1]);

    // Initialize Uart3
    if( !(imuUart.init(IMU_UART_BAUDRATE, IMU_UART_RX_QUEUE_SIZE, IMU_UART_TX_QUEUE_SIZE)) )
    {
      //  LOG_ERROR("IMU Uart Init Failed");
    }
    else
    {
        imuUart.setReady(true);
    }

    CUSTOM_DEBUG("IMU : Uart Init Done");

    // Initialize GPIO as output pin for Power
    imuResetPin.setAsOutput();
    imuResetPin.set(PIN_ON);

    CUSTOM_DEBUG("IMU : Reseting IMU Done");

    // Clearning Internal Buffer;
    clearBuffer();

    CUSTOM_DEBUG("IMU : Internal Buffer Cleared");

}

void imu::resetIMU()
{
    imuResetPin.set(PIN_OFF);

    vTaskDelay(IMU_RESET_TIMEOUT);

    imuResetPin.set(PIN_ON);

    return;
}

/* sendYawReadCommand
 * Issues a Read Command to the IMU
 * */
bool imu::sendYawReadCommand(const char * commandcode)
{
    bool retval = true;

    // Sends a 'Command' over Uart3 to IMU.
    if(imuUart.put(commandcode, IMU_WRITE_TIMEOUT))
    {
        CUSTOM_DEBUG("Sending Command Success");
    }
    else
    {
        CUSTOM_DEBUG("Sending Command Failed");
    }

    return retval;
}

/* getYaeReadingString
 * This function reads the Uart Rx Queue for messages received from IMU.
 * The message is posted to a local queue.
 * */
bool imu::getYawReadingString(void)
{
    bool retval = true;

    // Flush Uart
    imuUart.flush();

    // Read contents of UART Rx Queue
    if(imuUart.gets(buffer, IMU_BUFFER_SIZE, IMU_READ_TIMEOUT))
    {
        CUSTOM_DEBUG("IMU : %s", buffer);
        // Write to Local Queue for Producer Consumer Mechanism
        if(xQueueSend(queueHandle, buffer, IMU_INTERNAL_QUEUE_TIMEOUT) == pdTRUE)
        {
            CUSTOM_DEBUG("IMU : Written Data to Internal Queue");

        }
        else
        {
            CUSTOM_DEBUG("IMU : Failed to Write Data to Internal Queue");
        }
    }
    else
    {
        CUSTOM_DEBUG("IMU Read Failed\n");
        retval = false;
    }

    return retval;
}

/* getValue
 * This function would be called by a consumer
 * */
bool imu::getValue(char * external_buffer, unsigned int external_buffer_size)
{
    bool retval = false;

    /* TO DO : Change Queue Size
     * */

    if((xQueueReceive(queueHandle, external_buffer, IMU_INTERNAL_QUEUE_TIMEOUT)) == pdTRUE)
    {
        CUSTOM_DEBUG("getValue : Read from Queue was Successful");
        retval = true;
    }
    else
    {
        CUSTOM_DEBUG("getValue : Could not read from Queue");
    }

    return retval;
}

/* clearBuffer
 * Clears the contents of the internal Buffer
 */
void imu::clearBuffer(void)
{
    memset(buffer, 0, sizeof(buffer));
}

float imu::getHeading( void )
{
    char IMU_str_reading[IMU_BUFFER_SIZE];  // IMU readings in string format
    bool status = false;


    float imu_heading = 0;

    // Get readings from IMU every 10Hz
    status = getValue( IMU_str_reading, sizeof(IMU_str_reading));
    if( !status )
    {
        if( err_count > IMU_ERR_BACKOFF_COUNT )
        {
         //  LOG_ERROR("CRITICAL ERROR!!!! IMU not responsive\n");
            return -IMU_ERR;
        }

       // LOG_ERROR("ERROR!!! Cannot get IMU value\n");
        err_count++;

        // If the error_count exceeds threshold, report externally
        if( err_count > IMU_ERR_MAX_COUNT)
        {
            LE.on(1);
         //   LOG_ERROR("IMU not responsive. Requesting reset\n");
            resetIMU();
        }

        // Return old value
        return imu_old_heading;
    }

    // All is well and error count is within BACKOFF_COUNT
    else
    {
        if( err_count <= IMU_ERR_BACKOFF_COUNT )
        {
            err_count = 0;
            LE.off(1);
        }

        // We have IMU data as a string, convert to float
        imu_heading = strtof( (IMU_str_reading+3), NULL );  // Skip #Y=
        imu_old_heading = imu_heading;

    }
  //  printf("%f------>\n", imu_heading);
    return imu_heading;
}


/* IMUTask
 * This task sends a read command and writes the received message on a local queue.
 * This runs every 50ms, updating the value in the local queue.
 * */
bool IMUTask::run(void *p)
{
    // Send Read Command to IMU


    if(IMUInterface.sendYawReadCommand(IMUInterface.readYawCommand))
    {
        // Waits for a response from IMU
        if(IMUInterface.getYawReadingString())
        {
            CUSTOM_DEBUG("IMUTask : Read success");
        }
        else
        {
            CUSTOM_DEBUG("IMUTask : Read fail");
        }
    }
    uint16_t imu_heading = 0;       // 2-byte angle between 0 and 360. Compromise with precision
    imu_heading = static_cast<uint16_t>(IMUInterface.getHeading());

#if 0
        if(imu_heading  > (360 - HEADING_OFFSET) && imu_heading <= 360)
        {
            imu_heading = HEADING_OFFSET - (360 - (imu_heading));
        }
        else
        {
            imu_heading = imu_heading + HEADING_OFFSET;
        }
#endif

        //  printf("\nIMU: %d", imu_heading);
    // This task should now sleep for 50ms.
    vTaskDelayMs(IMUTASK_DELAY);

    // Always returning true
    return true;
}
