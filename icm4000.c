
#include "icm4000.h"
#include "accel.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct icm_4000_priv {
	struct device* i2c;
	int fsr;
	int rate_hz;
	int lpf;

	accel_event_handler event_handler;
};


struct icm_4000_config {
	int int_pin;
	struct i2c_bus_conf i2c;
};


static int bus_read(struct device* dev, uint8_t reg, uint8_t* data, size_t len) {
	static uint8_t buffer[256] = {0};
	const struct icm_4000_config* conf = dev->config;
	struct icm_4000_priv* icm = dev->data;

	buffer[0] = reg;
	memcpy(&buffer[1], data, len);
	return i2c_read(icm->i2c, conf->i2c.device_addr, buffer, len + 1);
}

static int bus_write(struct device* dev, uint8_t reg, const uint8_t* data, size_t len) {
	static uint8_t buffer[256] = {0};
	const struct icm_4000_config* conf = dev->config;
	struct icm_4000_priv* icm = dev->data;

	buffer[0] = reg;
	memcpy(&buffer[1], data, len);
	return i2c_write(icm->i2c, conf->i2c.device_addr, buffer, len + 1);
}


static int enable_new_sample_event(struct device* dev, int enable) {
	printf("[%s] %d\n", __FUNCTION__, enable);
	return 0;
}

static int enable_tap_detected_event(struct device* dev, int enable) {
	printf("[%s] %d\n", __FUNCTION__, enable);
	return 0;
}


static int set_fsr(struct device* dev, int fsr) {
	printf("[%s] fsr: %d\n", __FUNCTION__, fsr);
	struct icm_4000_priv* icm = dev->data;
	icm->fsr = fsr;
	return 0;
}

static int set_rate(struct device* dev, int rate) {
	printf("[%s] rate: %d\n", __FUNCTION__, rate);
	struct icm_4000_priv* icm = dev->data;
	icm->rate_hz = rate;
	return 0;
}

static int set_lpf(struct device* dev, int lpf) {
	printf("[%s] lpf: %d\n", __FUNCTION__, lpf);
	struct icm_4000_priv* icm = dev->data;
	icm->lpf = lpf;
	return 0;
}


static int set_conf(struct device* dev, enum accel_conf_type type, void* data) {
	printf("[%s]\n", __FUNCTION__);

	int r = 0;
	switch (type) {
		case accel_fsr:
			r = set_fsr(dev, *(int*) data);
			break;
		case accel_lpf:
			r = set_lpf(dev, *(int*) data);
			break;
		case accel_rate_hz:
			r = set_rate(dev, *(int*) data);
			break;
		default:
			return -1;
	}

	return r;
}

static void enable_interrupt_pin(struct device* device) {
	const struct icm_4000_config* conf = device->config;
	printf("[%s] enable interrupt pin : %d\n", __FUNCTION__, conf->int_pin);
}


static int setup_event(struct device* dev, enum accel_event_type event, int enable) {
	printf("[%s]\n", __FUNCTION__);

	int r = 0;
	if (event & accel_event_new_sample) {
		r = enable_new_sample_event(dev, enable);
		if (r) {
			return r;
		}
	}

	if (event & accel_event_tap_detected) {
		r = enable_tap_detected_event(dev, enable);
		if (r) {
			return r;
		}
	}

	if (enable) {
		enable_interrupt_pin(dev);
	}

	return r;
}

static int setup_conf(struct device* dev, const void* conf) {
	printf("[%s]\n", __FUNCTION__);

	const struct icm_configuration* sensor_conf = conf;

	int r = 0;
	if (sensor_conf->events) {
		r = setup_event(dev, sensor_conf->events, 1);
	}

	r = set_fsr(dev, sensor_conf->fsr);
	if (r) {
		return r;
	}
	r = set_rate(dev, sensor_conf->rate_hz);

	return r;
}


static int register_data_cb(struct device* dev, accel_event_handler event_handler) {
	printf("[%s]\n", __FUNCTION__);

	struct icm_4000_priv* icm = dev->data;
	icm->event_handler = event_handler;
	return 0;
}

static int powerdown(struct device* dev) {
	printf("[%s]\n", __FUNCTION__);

	return 0;
}

static int read_sample(struct device* dev, void* sample) {
	printf("[%s]\n", __FUNCTION__);

	return 0;
}

static int ping(struct device* dev) {
	static const uint8_t whoami_reg = 0x41;
	static const uint8_t whoami = 0x42;
	uint8_t response = 0;
	int r = bus_read(dev, whoami_reg, &response, 1);
	if (!r) {
		return r;
	}

	return whoami == response ? 0 : -1;
}
static int wakeup_icm(struct device* dev) {
	int r = ping(dev);
	if (!r) {
		return r;
	}

	uint8_t wakeup_command = (1 << 3);
	return bus_write(dev, 0x031, &wakeup_command, 1);
}

static int icm_init(struct device* dev) {
	struct icm_4000_priv* icm = dev->data;
	const struct icm_4000_config* conf = dev->config;

	printf("[%s]\n", __FUNCTION__);

	icm->i2c = i2c_get_instance(conf->i2c.bus_id);

	int r = wakeup_icm(dev);
	if (!r) {
		return r;
	}

	r = powerdown(dev);
	return r;
}

static struct icm_4000_priv icm_4000_priv_data = {
        .fsr = 16,
        .rate_hz = 100,
        .lpf = 0,
};

static struct icm_4000_config config = {
        .int_pin = 34,
        .i2c = {
                .bus_id = 0,
                .device_addr = 0x43,
        },
};

static struct accel_api api = {
        .setup_conf = setup_conf,
        .set_conf = set_conf,
        .setup_event = setup_event,
        .register_data_cb = register_data_cb,
        .powerdown = powerdown,
        .read_sample = read_sample,
};

DEVICE_DEF(icm_4000_device, icm_init, NULL, &api, &config, &icm_4000_priv_data);