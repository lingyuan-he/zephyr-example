/*
 * DT_DRV_COMPAT defines what device the driver class is compatible with.
 * This matches the "compatible" attribute of the device node in the device
 * tree overlay.
 */
#define DT_DRV_COMPAT custom_ledmatrix

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app/drivers/ledmatrix.h"

LOG_MODULE_REGISTER(ledmatrix, CONFIG_LEDMATRIX_LOG_LEVEL);

/* Assumes 5x5 as code below hard-codes the number of pin values to set. */
#define NUM_ROW 5 
#define NUM_COL 5

/* Used with gpio_pin_set_dt(). */
#define ACTIVE 1
#define INACTIVE 0

/*
 * @brief LED matrix dervice config struct definition.
 * @param GPIO specs of the row GPIO pins.
 * @param GPIO specs of the columns GPIO pins.
 */
struct ledmatrix_config {
    struct gpio_dt_spec rows[NUM_ROW];
    struct gpio_dt_spec cols[NUM_COL];
};

/**
 * @brief Static helper to set GPIO rows and columns of the LED matrix.
 *
 * @param dev LED matrix device.
 * @param row_values A static NUM_ROW array of values to set on the row GPIO
 * pins.
 * @param col_values A static NUM_COL array of values to set on the column GPIO
 * pins.
 * @retval 0 If successful.
 * @retval -errno Negative errno code on failure.
 */
static int _set_ledmatrix_rows_cols(const struct device *dev,
                                    const uint32_t *row_values,
                                    const uint32_t *col_values)
{
    int i;
    int retval;
    const struct gpio_dt_spec *spec;

    const struct ledmatrix_config *config = dev->config;

    for (i = 0; i < NUM_ROW; i++) {
        spec = &config->rows[i];
        LOG_DBG("Setting row GPIO pin %d to %d", spec->pin, row_values[i]);
        retval = gpio_pin_set_dt(spec, row_values[i]);
        if (retval != 0) {
            LOG_ERR("Failed to set GPIO of row %d: %d", i, retval);
            return retval;
        }
    }

    for (i = 0; i < NUM_COL; i++) {
        spec = &config->cols[i];
        LOG_DBG("Setting column GPIO pin %d to %d", spec->pin, col_values[i]);
        retval = gpio_pin_set_dt(spec, col_values[i]); 
        if (retval != 0) {
            LOG_ERR("Failed to set GPIO of column %d: %d", i, retval);
            return retval;
        }
    }
    return 0;
}

/**
 * @brief Set the left-most column of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
static int set_left_col(const struct device *dev)
{
    LOG_INF("Setting the left column LED on");

    /**
     * Row GPIOs are GPIO_ACTIVE_HIGH. When a row pin is set to high, the row
     * gets current to light up the LED.
     */
    const uint32_t row_values[NUM_ROW] =
        {ACTIVE, ACTIVE, ACTIVE, ACTIVE, ACTIVE};
    /**
     * Column GPIO pins are GPIO_ACTIVE_LOW. When a column pin is set to logic
     * level 1/active, it means it is physically low and not sink the current.
     * In other words, active column can light up depending on whether rows are
     * active.
     */
    const uint32_t col_values[NUM_COL] =
        {ACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE};

    return _set_ledmatrix_rows_cols(dev, row_values, col_values);
}

/**
 * @brief Set the right-most column of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
static int set_right_col(const struct device *dev)
{
    LOG_INF("Setting the right column LED on");

    const uint32_t row_values[NUM_ROW] =
        {ACTIVE, ACTIVE, ACTIVE, ACTIVE, ACTIVE};
    const uint32_t col_values[NUM_COL] =
        {INACTIVE, INACTIVE, INACTIVE, INACTIVE, ACTIVE};

    return _set_ledmatrix_rows_cols(dev, row_values, col_values);
}

/**
 * @brief Set the top row of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
static int set_top_row(const struct device *dev)
{
    LOG_INF("Setting the top row LED on");

    const uint32_t row_values[NUM_ROW] =
        {ACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE};
    const uint32_t col_values[NUM_COL] =
        {ACTIVE, ACTIVE, ACTIVE, ACTIVE, ACTIVE};

    return _set_ledmatrix_rows_cols(dev, row_values, col_values);
}

/**
 * @brief Set the bottom row of LED to illuminate.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
static int set_bottom_row(const struct device *dev)
{
    LOG_INF("Setting the bottom row LED on");

    const uint32_t row_values[NUM_ROW] =
        {INACTIVE, INACTIVE, INACTIVE, INACTIVE, ACTIVE};
    const uint32_t col_values[NUM_COL] =
        {ACTIVE, ACTIVE, ACTIVE, ACTIVE, ACTIVE};

    return _set_ledmatrix_rows_cols(dev, row_values, col_values);
}

/**
 * @brief Turn the LED matrix off.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
static int turn_off(const struct device *dev)
{
    LOG_INF("Turning the LED matrix off");

    const uint32_t row_values[NUM_ROW] =
        {INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE};
    const uint32_t col_values[NUM_COL] =
        {INACTIVE, INACTIVE, INACTIVE, INACTIVE, INACTIVE};

    return _set_ledmatrix_rows_cols(dev, row_values, col_values);
}

/**
 * @brief Initialize the LED matrix.
 *
 * @param dev LED matrix device.
 *
 * @retval 0 if successful.
 * @retval -errno Negative errno code on failure.
 */
static int instance_init(const struct device *dev)
{
    int i;
    int retval;
    const struct gpio_dt_spec *spec;

    const struct ledmatrix_config *config = dev->config;

    for (i = 0; i < NUM_ROW; i++) {
        spec = &config->rows[i];
        if (!gpio_is_ready_dt(spec)) {
            return -ENODEV;
        }
        /* Row pins: GPIO_ACTIVE_HIGH | GPIO_OUTPUT_INACTIVE */
        retval = gpio_pin_configure_dt(spec, GPIO_OUTPUT_INACTIVE);
        if (retval != 0) {
            return retval;
        }
    }

    for (i = 0; i < NUM_COL; i++) {
        spec = &config->cols[i];
        if (!gpio_is_ready_dt(spec)) {
            return -ENODEV;
        }
        /* Column pins: GPIO_ACTIVE_LOW | GPIO_OUTPUT_INACTIVE */
        retval = gpio_pin_configure_dt(spec, GPIO_OUTPUT_INACTIVE);
        if (retval != 0) {
            LOG_ERR("Failed to set GPIO of column %d: %d", i, retval);
            return retval;
        }
    }

	return 0;
}

/*
 * @brief Device API struct of the LED matrix driver.
 */
static const struct ledmatrix_driver_api driver_api = {
    .set_left_col = set_left_col,
    .set_right_col = set_right_col,
    .set_top_row = set_top_row,
    .set_bottom_row = set_bottom_row,
    .turn_off = turn_off,
};

/**
 * Apply the driver for each "custom_ledmatrix" compatible device using the
 * convenience macro DT_INST_FOREACH_STATUS_OKAY. While microbit:v2 has only
 * matrix, this is to illustrate the common approach.
 *
 * Within the macro below:
 * inst expands to the device tree node per each device occurrence.
 * ##inst expands to a sequence number like 0, 1, 2 etc.
 *
 * Each device will have its own config struct, but all devices share the same
 * init function and driver API functions.
 * This example does not have any driver data struct for the devices, i.e. the
 * second NULL in DEVICE_DT_INST_DEFINE.
 */

#define LEDMATRIX_DEFINE(inst)                                                 \
                                                                               \
    static const struct ledmatrix_config config##inst = {                      \
        .rows = { DT_INST_FOREACH_PROP_ELEM_SEP(inst, led_row_gpios,           \
                                           GPIO_DT_SPEC_GET_BY_IDX, (, )) },   \
        .cols = { DT_INST_FOREACH_PROP_ELEM_SEP(inst, led_col_gpios,           \
                                           GPIO_DT_SPEC_GET_BY_IDX, (, )) },   \
    };                                                                         \
                                                                               \
    DEVICE_DT_INST_DEFINE(inst, instance_init, NULL, NULL, &config##inst,      \
                          POST_KERNEL, CONFIG_LEDMATRIX_INIT_PRIORITY,         \
                          &driver_api);

DT_INST_FOREACH_STATUS_OKAY(LEDMATRIX_DEFINE)
