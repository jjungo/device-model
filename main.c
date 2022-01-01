#include <stdio.h>

#include "icm4000.h"

int main() {
	printf("initialize all devices ...\n");
	int r = device_init();
	if (r) {
		return r;
	}

	struct device* icm_4000 = device_get(device_icm4000);
	if (!icm_4000) {
		return -1;
	}

	static const struct icm_configuration icm_conf = {
	        .rate_hz = 100,
	        .fsr = 4,
	        .events = accel_event_new_sample | accel_event_tap_detected,
	};

	printf("playing with icm ...\n");
	accel_setup_conf(icm_4000, &icm_conf);
	int lpf = 15;

	r = accel_set_attrib(icm_4000, accel_lpf, &lpf);

	accel_setup_event(icm_4000, accel_event_tap_detected, 1);
	accel_register_data_cb(icm_4000, NULL);
	struct accel_new_sample sample = {0};
	accel_read_sample(icm_4000, &sample);

	accel_powerdown(icm_4000);
	return r;
}