#ifndef APP_DRIVERS_LEDMATRIX_H
#define APP_DRIVERS_LEDMATRIX_H

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/**
 * @defgroup drivers_ledmatrix LED matrix driver class
 * @ingroup drivers
 * @{
 *
 * @brief A custom driver class for LED matrix devices.
 *
 * This driver class shows how to create a custom driver class.
 * It provides an interface to operate an LED matrix device.
 */

/**
 * @defgroup drivers_ledmatrix_ops LED matrix driver operations
 * @{
 *
 * @brief Operations of the LED matrix driver.
 *
 * Each driver class tipically provides a set of operations that need to be
 * implemented by each driver. These are used to implement the public API. If
 * support for system calls is needed, the operations structure must be tagged
 * with `__subsystem` and follow the `${class}_driver_api` naming scheme.
 */

/** @brief LED matrix driver API struct */
__subsystem struct ledmatrix_driver_api {
	/**
	 * @brief Set the left-most column of LED to illuminate.
	 */
	int (*set_left_col)(const struct device *dev);

    /**
     * @brief Set the right-most column of LED to illuminate.
     */
    int (*set_right_col)(const struct device *dev);

    /**
     * @brief Set the top row of LED to illuminate.
     */
    int (*set_top_row)(const struct device *dev);

    /**
     * @brief Set the bottom row of LED to illuminate.
     */
    int (*set_bottom_row)(const struct device *dev);

    /**
     * @brief Turn the LED matrix off.
     */
    int (*turn_off)(const struct device *dev);
};

/**
 * @brief Set the left-most column of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
__syscall int ledmatrix_set_left_col(const struct device *dev);

static inline int z_impl_ledmatrix_set_left_col(const struct device *dev)
{
	__ASSERT_NO_MSG(DEVICE_API_IS(ledmatrix, dev));

	return DEVICE_API_GET(ledmatrix, dev)->set_left_col(dev);
}

/**
 * @brief Set the right-most column of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */

__syscall int ledmatrix_set_right_col(const struct device *dev);

static inline int z_impl_ledmatrix_set_right_col(const struct device *dev)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(ledmatrix, dev));

    return DEVICE_API_GET(ledmatrix, dev)->set_right_col(dev);
}

/**
 * @brief Set the top row of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
__syscall int ledmatrix_set_top_row(const struct device *dev);

static inline int z_impl_ledmatrix_set_top_row(const struct device *dev)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(ledmatrix, dev));

    return DEVICE_API_GET(ledmatrix, dev)->set_top_row(dev);
}


/**
 * @brief Set the bottom row of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
__syscall int ledmatrix_set_bottom_row(const struct device *dev);

static inline int z_impl_ledmatrix_set_bottom_row(const struct device *dev)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(ledmatrix, dev));

    return DEVICE_API_GET(ledmatrix, dev)->set_bottom_row(dev);
}


/**
 * @brief Turn the LED matrix off.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
__syscall int ledmatrix_turn_off(const struct device *dev);

static inline int z_impl_ledmatrix_turn_off(const struct device *dev)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(ledmatrix, dev));

    return DEVICE_API_GET(ledmatrix, dev)->turn_off(dev);
}

/**
 * Import the generated header with the driver syscalls. Must be placed after
 * the declarations of the syscalls. The format is:
 * <syscalls/name_of_this_header_file.h>.
 */
#include <syscalls/ledmatrix.h>

/** @} */

#endif /* APP_DRIVERS_LEDMATRIX_H */
