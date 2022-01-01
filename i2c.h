#ifndef _I2C_H
#define _I2C_H
#include "device.h"
#include <stddef.h>
#include <stdint.h>

enum i2c_freq {
	i2c_freq_100KHz,
	i2c_freq_400KHz,
};

struct i2c_bus_conf {
	int bus_id;
	uint8_t device_addr; /**< 7 bits device address */
};

struct i2c_api {
	int (*write)(struct device* dev, uint8_t addr, const uint8_t* buffer, size_t len);
	int (*read)(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len);
};

static inline int i2c_write(struct device* dev, uint8_t addr, const uint8_t* buffer, size_t len) {
	if (!dev) {
		return -1;
	}

	struct i2c_api* api = dev->api;
	if (api && api->write) {
		return api->write(dev, addr, buffer, len);
	}

	return 0;
}

static inline int i2c_read(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len) {
	if (!dev) {
		return -1;
	}

	struct i2c_api* api = dev->api;
	if (api && api->read) {
		return api->read(dev, addr, buffer, len);
	}

	return 0;
}

struct device* i2c_get_instance(int bus_id);

#endif