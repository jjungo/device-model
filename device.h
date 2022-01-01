#ifndef _DEVICE_H
#define _DEVICE_H

#include <stddef.h>

struct device {
	int (*init)(struct device* dev);
	int (*deinit)(struct device* dev);
	void* const api;

	const void* config;

	void* const data;
};

#define DEVICE_DEF(_name, _init_fn, _deinit_fn, _api, _config, _data) \
	struct device(_name) = DEVICE_ASSIGN((_init_fn), (_deinit_fn), (_api), (_config), (_data));

#define DEVICE_ASSIGN(_init_fn, _deinit_fn, _api, _config, _data) \
	{                                                             \
	        .init = (_init_fn),                                   \
	        .deinit = (_deinit_fn),                               \
	        .api = (_api),                                        \
	        .config = (_config),                                  \
	        .data = (_data),                                      \
	}

enum device_instance {
	device_i2c_dev0,
	device_i2c_dev1,
	device_icm4000,
	device_max,
};


int device_init();

struct device* device_get(enum device_instance device_instance);

#endif