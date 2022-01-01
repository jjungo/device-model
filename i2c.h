#ifndef _I2C_H
#define _I2C_H
#include "device.h"
#include <stddef.h>
#include <stdint.h>

enum i2c_freq {
	i2c_freq_100KHz,
	i2c_freq_400KHz,
};

enum i2c_mode {
	i2c_mode_master,
	i2c_mode_slave,
};

enum i2c_bus {
	i2c_bus_0,
	i2c_bus_1,
	i2c_bus_2,
	i2c_bus_3,
	i2c_bus_4,
	i2c_bus_max
};

struct i2c_bus_conf {
	enum i2c_bus bus_id;
	uint8_t device_addr; /**< 7 bits device address */
};

struct i2c_api {
	int (*write)(struct device* dev, uint8_t addr, const uint8_t* buffer, size_t len);
	int (*read)(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len);
};

static inline int i2c_write(struct device* dev, uint8_t addr, const uint8_t* buffer, size_t len) {
	struct i2c_api* api = dev->api;
	return api->write(dev, addr, buffer, len);
}

static inline int i2c_read(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len) {
	struct i2c_api* api = dev->api;
	return api->read(dev, addr, buffer, len);
}

struct device* i2c_get_instance(enum i2c_bus bus_id);

#endif