#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app_version.h"
#include "app/lib/accel.h"

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void)
{
    int rc;
	const struct device * accel;
    struct accel_values accel_xyz;
    get_accel_values_(const struct device *accel, struct accel_values *values)

	printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

    accel = get_accel_device(accel);
    if (accel == NULL) {
        return -1;
    }

	while (1) {
        rc = get_accel_values(accel, accel_xyz);
		k_sleep(K_MSEC(500));
	}

	return 0;
}

