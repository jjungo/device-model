#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "device.h"
#include "i2c.h"

#define get_data(_dev) ((_dev)->data)
#define get_conf(_dev) ((_dev)->config)

#define I2C_BUSES 2

struct i2c_priv {
	int a, b;
};
struct i2c_conf {
	int gpio_sda;
	int gpio_scl;
	enum i2c_freq freq;
	enum i2c_mode mode;
};

static int i2c_init(struct device* dev) {
	struct i2c_priv* data = get_data(dev);
	const struct i2c_conf* conf = get_conf(dev);

	printf("[%s]: a %d b %d\n", __FUNCTION__, data->a, data->b);
	if (dev->config) {
		printf("config is present\n");
	}
	return 0;
}

static int i2c_deinit(struct device* dev) {
	return 0;
}


static int write(struct device* dev, uint8_t addr, const uint8_t* buffer, size_t len) {
	struct i2c_priv* data = get_data(dev);
	printf("[%s]: a %d b %d\n", __FUNCTION__, data->a, data->b);

	return 0;
}

static int read(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len) {
	struct i2c_priv* data = get_data(dev);
	printf("[%s]: a %d b %d\n", __FUNCTION__, data->a, data->b);
	printf("[%s]: addr: 0x%02x, bufer: %p, len: %zu \n", __FUNCTION__, addr, buffer, len);
	return 0;
}


static struct i2c_priv private_data[I2C_BUSES] = {
        [i2c_bus_0] = {
                .b = 31,
                .a = 32,
        },

        [i2c_bus_1] = {
                .b = 41,
                .a = 42,
        },

};

const static struct i2c_conf conf[I2C_BUSES] = {
        [i2c_bus_0] = {
                .gpio_scl = 32,
                .gpio_sda = 33,
                .freq = i2c_freq_400KHz,
                .mode = i2c_mode_master,
        },

        [i2c_bus_1] = {
                .gpio_scl = 45,
                .gpio_sda = 46,
                .freq = i2c_freq_100KHz,
                .mode = i2c_mode_master,
        },

};

static struct i2c_api api = {
        .write = write,
        .read = read,
};


struct device i2c_devices[I2C_BUSES] = {
        [i2c_bus_0] = DEVICE_ASSIGN(i2c_init, i2c_deinit, &api, &conf[0], &private_data[0]),
        [i2c_bus_1] = DEVICE_ASSIGN(i2c_init, i2c_deinit, &api, &conf[1], &private_data[1]),
};

struct device* i2c_get_instance(enum i2c_bus bus) {
	return &i2c_devices[bus];
}
