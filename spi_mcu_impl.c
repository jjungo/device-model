

#include "spi.h"
#include <stdio.h>

#define get_data(_dev) ((_dev)->data)
#define get_conf(_dev) ((_dev)->config)

// stubs
#define gpio_set(x) ((void) (x))
#define gpio_clear(x) ((void) (x))

struct spi_priv {
};

struct spi_conf {
	enum spi_bus bus_id;
	enum spi_bit_order bit_order;
	enum spi_mode mode;
	enum spi_bitrate bitrate;

	int gpio_sckl;
	int gpio_miso;
	int gpio_mosi;
};

void spi_cs_enable(struct device* dev, const struct spi_bus_conf* conf) {
	gpio_clear(conf->gpio_cs);
	printf("[%s] gpio %d\n", __FUNCTION__, conf->gpio_cs);
}

void spi_cs_disable(struct device* dev, const struct spi_bus_conf* conf) {
	gpio_set(conf->gpio_cs);
	printf("[%s] gpio %d\n", __FUNCTION__, conf->gpio_cs);
}

int spi_xfer(struct device* dev, const struct spi_bus_conf* conf, const struct spi_xfer* xfer) {
	spi_cs_enable(dev, conf);
	printf("[%s] bus: %d\n", __FUNCTION__, conf->bus_id);
	spi_cs_disable(dev, conf);
	return 0;
}

int spi_init(struct device* dev) {
	struct spi_priv* data = get_data(dev);
	const struct spi_conf* conf = get_conf(dev);

	printf("[%s] bus: %d sclk: %d, mosi: %d, miso: %d, bitrate: %d, bitorder: %d, mode: %d, \n", __FUNCTION__,
	       conf->bus_id,
	       conf->gpio_sckl,
	       conf->gpio_mosi,
	       conf->gpio_miso,
	       conf->bitrate,
	       conf->bit_order,
	       conf->mode);

	return 0;
}

static struct spi_api api = {
        .xfer = spi_xfer,
};

static struct spi_priv priv = {

};

static const struct spi_conf conf = {
        .bus_id = spi_bus_0,
        .mode = spi_mode_0,
        .bit_order = spi_bit_order_msb_first,
        .bitrate = spi_bitrate_64Mb,
        .gpio_miso = 12,
        .gpio_mosi = 13,
        .gpio_sckl = 14,
};

DEVICE_DEF(spi_device_0, spi_init, NULL, &api, &conf, &priv)