#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "device.h"
#include "i2c.h"

#define I2C_CONF_ASSIGN(_bus_id, _gpio_scl, _gpio_sda, _freq, _mode) \
	{                                                                \
		.bus_id = (_bus_id),                                         \
		.gpio_scl = (_gpio_scl),                                     \
		.gpio_sda = (_gpio_sda),                                     \
		.freq = (_freq),                                             \
		.mode = (_mode),                                             \
	}

#define get_data(_dev) ((_dev)->data)
#define get_conf(_dev) ((_dev)->config)

#define I2C_BUSES 2

struct i2c_priv {
	int a, b;
};
struct i2c_conf {
	enum i2c_bus bus_id;
	int gpio_sda;
	int gpio_scl;
	enum i2c_freq freq;
	enum i2c_mode mode;
};

static int i2c_init(struct device* dev) {
	struct i2c_priv* data = get_data(dev);
	const struct i2c_conf* conf = get_conf(dev);

	printf("[%s]: a %d b %d bus: %d\n", __FUNCTION__, data->a, data->b, conf->bus_id);
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
	const struct i2c_conf* conf = get_conf(dev);

	printf("[%s]: a %d b %d bus: %d\n", __FUNCTION__, data->a, data->b, conf->bus_id);

	return 0;
}

static int read(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len) {
	struct i2c_priv* data = get_data(dev);
	const struct i2c_conf* conf = get_conf(dev);
	printf("[%s]: a %d b %d bus: %d\n", __FUNCTION__, data->a, data->b, conf->bus_id);
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

static const struct i2c_conf conf[I2C_BUSES] = {
        [i2c_bus_0] = I2C_CONF_ASSIGN(i2c_bus_0, 32, 33, i2c_freq_400KHz, i2c_mode_master),
        [i2c_bus_1] = I2C_CONF_ASSIGN(i2c_bus_1, 42, 43, i2c_freq_100KHz, i2c_mode_master),
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
