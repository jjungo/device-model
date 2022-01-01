#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "device.h"
#include "i2c.h"

#define get_data(_dev) ((_dev)->data)

struct i2c_priv_t {
	int a, b;
};

static int i2c_init(struct device* dev) {
	struct i2c_priv_t* data = get_data(dev);

	printf("[%s]: a %d b %d\n",__FUNCTION__ , data->a, data->b);
	if (dev->config) {
		printf("config is present\n");
	}
	return 0;
}

static int i2c_deinit(struct device* dev) {
	return 0;
}


static int write(struct device* dev, uint8_t addr, const uint8_t* buffer, size_t len) {
	struct i2c_priv_t* data = get_data(dev);
	printf("[%s]: a %d b %d\n",__FUNCTION__ , data->a, data->b);

	return 0;
}

static int read(struct device* dev, uint8_t addr, uint8_t* buffer, size_t len) {
	struct i2c_priv_t* data = get_data(dev);
	printf("[%s]: a %d b %d\n",__FUNCTION__ , data->a, data->b);
	printf("[%s]: addr: 0x%02x, bufer: %p, len: %zu \n",__FUNCTION__ , addr, buffer, len);
	return 0;
}


static struct i2c_priv_t private_data = {
        .b = 42,
        .a = 21,
};

static struct i2c_api api = {
        .write = write,
        .read = read,
};

DEVICE_DEF(i2c_device, i2c_init, i2c_deinit, &api, NULL, &private_data)

struct device* i2c_get_instance(int bus_id) {
	return &i2c_device;
}
