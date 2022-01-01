
#ifndef DEVICE_MODEL_SPI_H
#define DEVICE_MODEL_SPI_H

#include "device.h"
#include <stdint.h>


enum spi_bitrate {
	spi_bitrate_256kb,
	spi_bitrate_512kb,
	spi_bitrate_1Mb,
	spi_bitrate_2Mb,
	spi_bitrate_8Mb,
	spi_bitrate_16Mb,
	spi_bitrate_32Mb,
	spi_bitrate_64Mb,

	spi_bitrate_max,
};

enum spi_bus {
	spi_bus_0,
	spi_bus_1,
	spi_bus_2,
	spi_bus_3,
};

enum spi_mode {
	spi_mode_0, /**< sck active H, leading edge*/
	spi_mode_1, /**< sck active H, trailing edge */
	spi_mode_2, /**< sck active L, leading edge */
	spi_mode_3, /**< sck actvie L, trailing edge */
};

enum spi_bit_order {
	spi_bit_order_msb_first,
	spi_bit_order_lsb_first,
};

struct spi_bus_conf {
	enum spi_bus bus_id;
	int gpio_cs; /**< chip select */
};

struct spi_xfer {
	const uint8_t* tx;
	size_t tx_len;
	uint8_t* rx;
	size_t rx_len;
};

struct spi_api {
	int (*xfer)(struct device* dev, const struct spi_bus_conf* conf, const struct spi_xfer* xfer);
};

#endif//DEVICE_MODEL_SPI_H
