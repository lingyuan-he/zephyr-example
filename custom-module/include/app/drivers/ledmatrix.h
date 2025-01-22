#ifndef APP_DRIVERS_LEDMATRIX_H
#define APP_DRIVERS_LEDMATRIX_H

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

/** @brief LED matrix driver class operations */
__subsystem struct ledmatrix_driver_api {
	/**
	 * @brief Set the left-most column of LED to illuminate.
	 *
	 * @param dev LED matrix device.
	 *
	 * @retval 0 if successful.
	 * @retval -errno Negative errno code on failure.
	 */
	int (*set_left_col)(const struct device *dev);

    /**
     * @brief Set the right-most column of LED to illuminate.
     *
     * @param dev LED matrix device.
     *
     * @retval 0 if successful.
     * @retval -errno Negative errno code on failure.
     */
    int (*set_right_col)(const struct device *dev);

    /**
     * @brief Set the top row of LED to illuminate.
     *
     * @param dev LED matrix device.
     *
     * @retval 0 if successful.
     * @retval -errno Negative errno code on failure.
     */
    int (*set_top_row)(const struct device *dev);

    /**
     * @brief Set the bottom row of LED to illuminate.
     *
     * @param dev LED matrix device.
     *
     * @retval 0 if successful.
     * @retval -errno Negative errno code on failure.
     */
    int (*set_bottom_row)(const struct device *dev);

    /**
     * @brief Turn the LED matrix off.
     *
     * @param dev LED matrix device.
     *
     * @retval 0 if successful.
     * @retval -errno Negative errno code on failure.
     */
    int (*turn_off)(const struct device *dev);
};

/** @} */

#endif /* APP_DRIVERS_LEDMATRIX_H */
