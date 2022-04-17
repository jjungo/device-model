# device-model

This is a small idea how to create a device model where private data members
of a driver are not exposed to the client in the header file. The idea also
is to avoid to use a opaque pointer because it is a bit a mess to handle 
allocation and dependancy injection.

Here I show a simple imaginary accelerometer device (icm4000) that implement
an interface `accel`. The icm4000 depends on a `i2c` interface as a bus 
interface.

Each `device` holds a data structure containing: 
* a pointer to a init function
* a pointer to a de init function
* a pointer to private data (device implement specific detail), this pointer 
* a const pointer to a device configuration
* a generic pointer to expose the device api

A new `device` can be instanciated like the following: The icm is a concrete
implementation of the `accel` interface.
```c

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

DEVICE_DEF(icm_4000_device, icm_init, icm_deinit, &api, &config, &icm_4000_priv_data);
```

An then the user of the device has access to the device through the accel interface:
```c
static const struct icm_configuration icm_conf = {
    .rate_hz = 100,
    .fsr = 4,
    .events = accel_event_new_sample | accel_event_tap_detected,
};

struct device* icm_4000 = device_get(device_icm4000);
if (!icm_4000) {
  return -1;
}

int r = accel_setup_conf(icm_4000, &icm_conf);
int lpf = 15;
r = accel_set_conf(icm_4000, accel_lpf, &lpf);
accel_setup_event(icm_4000, accel_event_tap_detected, 1);
```
