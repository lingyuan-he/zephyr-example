#ifndef APP_LIB_ACCEL_H
#define APP_LIB_ACCEL_H

#include <zephyr/device.h>

/**
 * @defgroup accelerometer lib
 * @{
 *
 * @brief Routines for the accelerometer.
 */

/**
 * @brief Struct that represents the accelerometer values.
 */
struct accel_values {
    double x;
    double y;
    double z;
};

/**
 * @brief Return a pointer to the accelerometer device.
 * @param None
 * @retval Pointer to a const accelerometer device; NULL if the device is not
 *         found or is not ready.
 */
const struct device * get_accel_device(void);

/**
 * @brief Return sensor values in a struct.
 * @param sensor Pointer to the accelerometer device.
 * @param values Pointer to an accel_values struct.
 * @retval 0 if successful, -errno otherwise.
 */
int get_accel_values(const struct device *accel, struct accel_values *values);

/** @} */

#endif /* APP_LIB_ACCEL_H */
