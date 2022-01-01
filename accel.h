
#ifndef CPLAYGROUND_ACCEL_H
#define CPLAYGROUND_ACCEL_H

#include "device.h"

enum accel_conf_type {
	accel_fsr,
	accel_lpf,
	accel_rate_hz,
};

enum accel_event_type {
	accel_event_new_sample = (1 << 0),
	accel_event_tap_detected = (1 << 1),
};

struct accel_new_sample {
	int x, y, z;
};

struct accel_tap_detected {
	int detect;
};

struct accel_event {
	enum accel_event_type type;
	union {
		struct accel_new_sample new_sample;
		struct accel_tap_detected tap_detected;
	};
};

typedef void (*accel_event_handler)(struct device* dev, struct accel_event event);

struct accel_api {
	int (*setup_conf)(struct device* dev, const void* conf);
	int (*set_conf)(struct device* dev, enum accel_conf_type type, void* data);
	int (*setup_event)(struct device* dev, enum accel_event_type events, int enable);
	int (*register_data_cb)(struct device* dev, accel_event_handler event_handler);
	int (*read_sample)(struct device* dev, void* sample);
	int (*powerdown)(struct device* dev);
};

static inline int accel_setup_conf(struct device* dev, const void* conf);
static inline int accel_set_conf(struct device* dev, enum accel_conf_type type, void* data);
static inline int accel_setup_event(struct device* dev, enum accel_event_type events, int enable);
static inline int accel_register_data_cb(struct device* dev, accel_event_handler event_handler);
static inline int accel_read_sample(struct device* dev, void* sample);
static inline int accel_powerdown(struct device* dev);

static inline int accel_setup_conf(struct device* dev, const void* conf) {
	if (!dev) {
		return -1;
	}
	struct accel_api* api = dev->api;
	if (api && api->setup_conf) {
		return api->setup_conf(dev, conf);
	}

	return 0;
}

static inline int accel_set_conf(struct device* dev, enum accel_conf_type type, void* data) {
	if (!dev) {
		return -1;
	}
	struct accel_api* api = dev->api;
	if (api && api->set_conf) {
		return api->set_conf(dev, type, data);
	}

	return 0;
}
static inline int accel_register_data_cb(struct device* dev, accel_event_handler event_handler) {
	if (!dev) {
		return -1;
	}
	struct accel_api* api = dev->api;
	if (api && api->register_data_cb) {
		return api->register_data_cb(dev, event_handler);
	}

	return 0;
}
static inline int accel_powerdown(struct device* dev) {
	if (!dev) {
		return -1;
	}
	struct accel_api* api = dev->api;
	if (api && api->powerdown) {
		return api->powerdown(dev);
	}

	return 0;
}

static inline int accel_read_sample(struct device* dev, void* sample) {
	if (!dev) {
		return -1;
	}
	struct accel_api* api = dev->api;
	if (api && api->read_sample) {
		return api->read_sample(dev, sample);
	}

	return 0;
}

static inline int accel_setup_event(struct device* dev, enum accel_event_type events, int enable) {
	if (!dev) {
		return -1;
	}
	struct accel_api* api = dev->api;
	if (api && api->setup_event) {
		return api->setup_event(dev, events, enable);
	}

	return 0;
}
#endif//CPLAYGROUND_ACCEL_H
