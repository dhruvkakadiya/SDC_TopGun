/*
 * imu.hpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Akshay Vijaykumar
 */

#ifndef L4_IO_IMU_HPP_
#define L4_IO_IMU_HPP_

#include "uart3.hpp"
#include "file_logger.h"
#include "semphr.h"
#include "gpio.hpp"
#include "projdefs.h"
#include "string.h"
#include "scheduler_task.hpp"
#include "queue.h"

#define PIN_OFF false
#define PIN_ON true

#define IMU_WRITE_TIMEOUT 0 // Timeout of 0ms
#define IMU_READ_TIMEOUT 0  // portMAX_DELAY // Timeout of 20ms

#define IMU_SEM_TIMEOUT 0
#define IMU_SEM_APP_TIMEOUT 0


#define IMU_UART_RX_QUEUE_SIZE 16 // RX Queue Size 20 Bytes
#define IMU_UART_TX_QUEUE_SIZE 3 // TX Queue Size 3 Bytes

#define IMU_UART_BAUDRATE 9600   // BaudRate

#define IMU_BUFFER_SIZE IMU_UART_RX_QUEUE_SIZE // Internal Buffer Size

#define IMU_INTERNAL_QUEUE_LENGTH 1 // Elements in the internal Queue
#define IMU_INTERNAL_QUEUE_TIMEOUT 0 // Timeout for internal Queue
#define IMU_COMMAND_SIZE 2 // Size of Command sent to IMU

#define IMU_RESET_TIMEOUT 20 // Timeout for Reset

#define IMU_ERR_MAX_COUNT       32
#define IMU_ERR_BACKOFF_COUNT   64

#define IMU_ERR     5


#define IMUInterface imu::getInstance() // Singleton

class imu : public SingletonTemplate<imu>
{
    public:

        imu();

        inline unsigned int getBufferSize(void)
        {
            return sizeof(buffer);
        }

        // External Functions
        bool getValue(char * external_buffer, unsigned int external_buffer_size);
        void resetIMU(void);
        void clearBuffer(void);
        float getHeading( void );

        // Helper Functions
        bool getYawReadingString(void);
        bool sendYawReadCommand(const char * commandcode);

        friend class SingletonTemplate<imu>;
        friend class IMUTask;

    private:

        Uart3& imuUart; // Reference to the Uart Module used to communicate to IMU
        GPIO imuResetPin; // GPIO Connected to Reset Pin

        char buffer[IMU_BUFFER_SIZE]; // Character Array that would hold the Yaw Reading
        char readYawCommand[IMU_COMMAND_SIZE]; // Read Command to be sent to IMU
        float imu_old_heading;
        unsigned int err_count;
        QueueHandle_t queueHandle;
};

#define IMUTASK_DELAY 50 // 50ms Sleep for IMUTask; Get value every 50ms

/* IMU Task
 * This task runs before the periodic scheduler.
 * On creation, it initializes the IMU Power and Interface
 * */
class IMUTask : public scheduler_task
{
    public:
        IMUTask(unsigned int priority):
            scheduler_task("IMUTask", 3000, priority)
    {
            LOG_DEBUG("IMUTask : Initializing Task");
            // Power on the IMU Interface
            IMUInterface;

            LOG_DEBUG("IMUTask : Initiializing Task Done");
    }

    bool run(void *p);
};



#endif /* L4_IO_IMU_HPP_ */
