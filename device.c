#include "device.h"
#include <stddef.h>


#define ARRAY_SIZE(x) (sizeof((x)) / sizeof(((x)[0])))

extern struct device i2c_devices[];
extern struct device icm_4000_device;
extern struct device spi_device_0;

static struct device const* all_devices[] = {
        [device_spi_dev0] = &spi_device_0,
        [device_i2c_dev0] = &i2c_devices[0],
        [device_i2c_dev1] = &i2c_devices[1],
        [device_icm4000] = &icm_4000_device,
};


static int init_all_devices(struct device const** devs, size_t len) {
	for (unsigned int i = 0; i < len; i++) {
		struct device* dev = devs[i];
		if (!dev->init) { continue; }
		int r = dev->init(dev);
		if (r) {
			return r;
		}
	}

	return 0;
}

int device_init() {
	return init_all_devices(all_devices, ARRAY_SIZE(all_devices));
}

struct device* device_get(enum device_instance device_instance) {
	if (device_instance >= device_max) {
		return NULL;
	}

	return all_devices[device_instance];
}
