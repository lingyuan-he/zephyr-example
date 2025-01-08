#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/errno.h>

#include <app/lib/accel.h>

/**
 * @brief Return a pointer to the accelerometer device.
 * @param None
 * @retval Pointer to a const accelerometer device; NULL if the device is not
 *         found or is not ready.
 */
const struct device * get_accel_device(void)
{
    const struct device * sensor = DEVICE_DT_GET_ANY(accel);

	if (sensor == NULL) {
		LOG_ERR("No accelerometer device found in the device tree");
		return NULL;
	}

	if (!device_is_ready(sensor)) {
		LOG_ERR("Device %s is not ready", sensor->name);
		return NULL;
	}
}

/**
 * @brief Return sensor values in a struct.
 * @param sensor Pointer to the accelerometer device.
 * @param values Pointer to an accel_values struct.
 * @retval 0 if successful, -errno otherwise.
 */
int get_accel_values(const struct device *accel, struct accel_values *values)
{
    int rc;
    struct sensor_value s_values[3];

    if (sensor == NULL) {
        return -EINVAL;
    }

    rc = sensor_sample_fetch(sensor);
    if (rc != 0) {
        LOG_ERR("Sampling accelerometer %s failed: %d", sensor->name, rc);
        return rc;
    }

	rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, s_values);
	if (rc < 0) {
		LOG_ERR("Fetch data from accelerometer %s failed: %d", sensor->name, rc);
        return rc;
	}

    values->x = sensor_value_to_double(&s_values[0]);
    values->y = sensor_value_to_double(&s_values[1]);
    values->z = sensor_value_to_double(&s_values[2]);

    LOG_DBG("Accelerometer values: x=%f y=%f z=%f",
            values->x, values->y, values->z);
    return 0;
} 
