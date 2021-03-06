/*
 * 			vcnl4200.c
 *
 *   Created on:  March 26, 2018
 *  Last Edited:  April 20, 2018
 *       Author:  Nick Gorab
 *        Board:  MSP403FR5994
 * 
 *  This code is used to interface with the VCNL4200 distance sensor
 */

#include "system.h"

/*
 * Function: VCNL4200_Get_PS_Data
 * ------------------------------
 * Returns the proximity sensor data from the VCNL4200
 *
 * @param sensdat:	Struct is used as an argument so the
 *					function can have access to the I2C 
 *					received data buffer.
 * @param return:	Returns the prox_data variable found
 *					in the sensor data struct as an unsigned
 *					16-bit integer value.
 */

uint16_t VCNL4200_Get_PS_Data(void) {

    VCNL4200_Set_LED_I();

    char prox_buffer[2] = {0};
	i2c_UCB2_multiple_read(VCNL4200_ADDRESS, VCNL4200_PROXIMITY_REG, 2, prox_buffer);
	int prox_data = (prox_buffer[0] << 8 | prox_buffer[1]);

	return prox_data;
}



/*
 * Function: VCNL4200_Get_ALS_Data
 * -------------------------------
 * Returns the ambient light sensor data from the VCNL4200
 *
 * @param sensdat:	Struct is used as an argument so the
 *					function can have access to the I2C 
 *					received data buffer.
 * @param return:	Overwrites the prox data buffer and replaces
 *					it with the ambient light information. This 
 *					may need to be reworked in future code.
 */

uint16_t VCNL4200_Get_ALS_Data(void) {
    char als_buffer[2] = {0};

    i2c_UCB2_multiple_read(VCNL4200_ADDRESS, VCNL4200_AMBIENT_REG, 2, als_buffer);
    int als_data = (als_buffer[0] << 8 | als_buffer[1]);

    return als_data;
}

/*
 * Function: VCNL4200_Read_CONF_REG
 * --------------------------------
 * Function built specifically to read all of the 
 * available registers on the VCNL4200 so they can be
 * viewed on a logic analyzer for debugging purposes.
 */

void VCNL4200_Read_CONF_REG(void) {
    char i = 0;
    char data_buff[2] = {0};
    while(i < 0x0F) {
    i2c_UCB2_multiple_read(VCNL4200_ADDRESS, i, 2, data_buff);
    i++;
    }
}



/*
 * Function: VCNL4200_Get_ALS_Data
 * -------------------------------
 * Returns the device ID from the VCNL4200. This should 
 * always read 0x4810. (Needs to be confirmed.)
 *
 * @param sensdat:	Struct is used as an argument so the
 *					function can have access to the I2C 
 *					received data buffer.
 * @param return:	Returns the ID of the device as an
 *					unsigned 16-bit register.
 */

uint16_t VCNL4200_Get_ID(void) {
    char id_buffer[2] = {0};
	i2c_UCB2_multiple_read(VCNL4200_ADDRESS, VCNL4200_DeviceID_REG, 2, id_buffer);
	uint16_t vcnl4200_id = (id_buffer[0] << 8 | id_buffer[1]);

	return vcnl4200_id;
}



/*
 * Function: VCNL4200_Start_PS
 * ---------------------------
 * Starts the proximity sensor IRLED
 * by writing the last bit of the CONF1
 * register to zero (IRLED Power On).
 */

void VCNL4200_Start_PS(void) {
    char data[2] = {0x0A, 0x08};
    i2c_UCB2_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_CONF1_CONF2_REG, 2, data);
}




void VCNL4200_Set_LED_I(void) {
    char data[2] = {0x0C, 0x07};
    i2c_UCB2_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_CONF3_MS_REG, 2, data);
}

/* Auxiliary functions */

void VCNL4200_Start_CONF3_MS(void){	
    char data[2] = {0x00, 0x20};
    i2c_UCB2_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_CONF3_MS_REG, 2, data);
}


void VCNL4200_Start_ALS(void) {
    char data[1] = {0};
    i2c_UCB2_single_write(VCNL4200_ADDRESS, VCNL4200_ALS_CONF_REG, data);
}

void VCNL4200_Init(void) {
	char data[2] = {0x88, 0x13};
	i2c_UCB2_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_THDL_REG, 2, data);
	data[0] = 0xE0;
	data[1] = 0x2E;
	i2c_UCB2_multiple_write(VCNL4200_ADDRESS, VCNL4200_PS_THDH_REG, 2, data);

}
