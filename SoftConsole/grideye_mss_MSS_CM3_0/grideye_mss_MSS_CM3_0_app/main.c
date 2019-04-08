#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"

/*
    Authored by:
    Darshin Patel
    Matthew Stachowiak
    Michael Manceor
    Nathan Vollbrecht
*/

// GridEYE address is 0x69
#define gridEYE_ADDR 0b1101001

// Perform a write to a register on the gridEYE
// reg_addr: 1-byte array containing address of register to read
// recieved_data: 1-byte array containing input data
void gridEYE_write(uint8_t* reg_addr, uint8_t* data){

	uint8_t signal[] = {*reg_addr, *data};

	MSS_I2C_write
		(
				&g_mss_i2c1,
				gridEYE_ADDR,
				signal,
				sizeof(signal),
				MSS_I2C_RELEASE_BUS

		);

	MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
}

// Read from a specific register on the gridEYE
// reg_addr: 1-byte array containing address of register to read
// recieved_data: 1-byte array where received data will be placed
void gridEYE_read(uint8_t* reg_addr, uint8_t* recieved_data ){

	MSS_I2C_write_read
		(
				&g_mss_i2c1,
				gridEYE_ADDR,
				reg_addr,
				1,
				recieved_data,
				1,
				MSS_I2C_RELEASE_BUS
		);

	MSS_I2C_wait_complete(&g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
}

int main()
{

	// Initialize with a clock freq of ~ 400kHz
	MSS_I2C_init(&g_mss_i2c1 , gridEYE_ADDR, MSS_I2C_PCLK_DIV_256 );

	uint8_t resAddr[] = {0x1};
	uint8_t resVal[] = {0x3f};

	// Reset Device
	gridEYE_write(resAddr, resVal);

	uint8_t powAddr[] = {0x0};
	uint8_t powVal[] = {0x0};

	// Set normal power mode
	gridEYE_write(powAddr, powVal);

	uint8_t pAddr[] = {0x80};
	uint8_t data[] = {0};

	// The gridEYE uses a write_read I2C communication
	// for read transactions.
	// The write-data is the address of the register,
	// the read data is a 1-byte data array that the slave modifies.

	while (1) {
		gridEYE_read(pAddr, data);
		printf("Pixel 0 Data: %u\r\n", data[0]);

	}


	return 0;
}
