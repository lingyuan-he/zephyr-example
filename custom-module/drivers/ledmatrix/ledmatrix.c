/*
 * DT_DRV_COMPAT defines what device the driver class is compatible with.
 * This matches the "compatible" attribute of the node in the device tree.
 */
#define DT_DRV_COMPAT ledmatrix

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app/drivers/ledmatrix.h"

LOG_MODULE_REGISTER(ledmatrix, CONFIG_LEDMATRIX_LOG_LEVEL);

/* Uses the first ledmatrix compatible device. */
#define LEDMATRIX_NODE DT_NODELABEL(ledmatrix_device)
//#define LEDMATRIX_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(ledmatrix)
/* Assumes 5x5 as code below hard-codes the number of pin values. */
#define NUM_ROW 5 
#define NUM_COL 5

#define GPIO_HIGH GPIO_OUTPUT_ACTIVE
#define GPIO_LOW GPIO_OUTPUT_INACTIVE

/*
 * @brief LED matrix dervice config struct.
 * @param rows GPIO specs of the rows.
 * @param cols GPIO specs of the columns.
 */
struct ledmatrix_config {
    struct gpio_dt_spec rows[NUM_ROW];
    struct gpio_dt_spec cols[NUM_COL];
};


/**
 * @brief static helper to set GPIO rows and columns of the LED matrix.
 *
 * @param dev LED matrix device.
 * @param row_values A static NUM_ROW array of values to set on the row GPIO
 * pins.
 * @param col_values A static NUM_COL array of values to set on the column GPIO
 * pins.
 * @retval 0 If successful.
 * @retval -errno Negative errno code on failure.
 */
static int _set_ledmatrix_rows_cols(const struct device *dev, const uint32_t *row_values,
                                    const uint32_t *col_values)
{
    int i;
    int retval;
    const struct gpio_dt_spec *spec;

    const struct ledmatrix_config *config = dev->config;

    for (i = 0; i < NUM_ROW; i++) {
        spec = &config->rows[i];
        retval = gpio_pin_set_dt(spec, row_values[i]);
        if (retval != 0) {
            LOG_ERR("Failed to set GPIO of row %d: %d", i, retval);
            return retval;
        }
    }

    for (i = 0; i < NUM_COL; i++) {
        spec = &config->cols[i];
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
int set_leftcol_led(const struct device *dev)
{
    LOG_DBG("Setting the left column LED on");

    const uint32_t row_values[NUM_ROW] =
        {GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH};
    const uint32_t col_values[NUM_COL] =
        {GPIO_LOW, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH};

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
int set_rightcol_led(const struct device *dev)
{
    LOG_DBG("Setting the right column LED on");

    const uint32_t row_values[NUM_ROW] =
        {GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH};
    const uint32_t col_values[NUM_COL] =
        {GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_HIGH, GPIO_LOW};
    
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
int set_toprow_led(const struct device *dev)
{
    LOG_DBG("Setting the top row LED on");

    const uint32_t row_values[NUM_ROW] =
        {GPIO_HIGH, GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW};
    const uint32_t col_values[NUM_COL] =
        {GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW};

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
int set_bottomrow_led(const struct device *dev)
{
    LOG_DBG("Setting the bottom row LED on");

    const uint32_t row_values[NUM_ROW] =
        {GPIO_HIGH, GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW};
    const uint32_t col_values[NUM_COL] =
        {GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW};

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
int ledmatrix_off(const struct device *dev)
{
    LOG_DBG("Turning the LED matrix off");

    const uint32_t row_values[NUM_ROW] =
        {GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW};
    const uint32_t col_values[NUM_COL] =
        {GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW, GPIO_LOW};

    return _set_ledmatrix_rows_cols(dev, row_values, col_values);
}

static int ledmatrix_init(const struct device *dev)
{
    int i;
    int retval;
    const struct gpio_dt_spec *spec;

    const struct ledmatrix_config *config = dev->config;

    for (i = 0; i < NUM_ROW; i++) {
        spec = &config->rows[i];
        if (!gpio_is_ready_dt(spec)) {
            LOG_ERR("GPIO of row %d is not ready", i);
            return -ENODEV;
        }
        retval = gpio_pin_configure_dt(spec, GPIO_LOW);
        if (retval != 0) {
            LOG_ERR("Failed to set GPIO of row %d: %d", i, retval);
            return retval;
        }
    }

    for (i = 0; i < NUM_COL; i++) {
        spec = &config->cols[i];
        if (!gpio_is_ready_dt(spec)) {
            LOG_ERR("GPIO of column %d is not ready", i);
            return -ENODEV;
        }
        retval = gpio_pin_configure_dt(spec, GPIO_LOW);
        if (retval != 0) {
            LOG_ERR("Failed to set GPIO of column %d: %d", i, retval);
            return retval;
        }
    }

	return 0;
}



//!!
const struct gpio_dt_spec spec = GPIO_DT_SPEC_INST_GET_BY_IDX(ledmatrix, row_gpio_pins, 0);


const struct gpio_dt_spec spec2 = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(ledmatrix_device),
                                                     row_gpio_pins, 0);


/*
 * @brief configuration of the LED matrix that contains the specs of the row
 * and column GPIO pins.
 */
static const struct ledmatrix_config config = {
	.rows = { DT_FOREACH_PROP_ELEM_SEP(DT_NODELABEL(ledmatrix_device), row_gpio_pins,
                                       GPIO_DT_SPEC_GET_BY_IDX, (, )) },
    .cols = { DT_FOREACH_PROP_ELEM_SEP(DT_NODELABEL(ledmatrix_device), col_gpio_pins,
                                       GPIO_DT_SPEC_GET_BY_IDX, (, )) },
};

/*
 * @brief Device API struct of the LED matrix driver.
 */
static DEVICE_API(ledmatrix, driver_api) = {
    .set_leftcol_led = set_leftcol_led,
    .set_rightcol_led = set_rightcol_led,
    .set_toprow_led = set_toprow_led,
    .set_bottomrow_led = set_rightcol_led,
    .ledmatrix_off = ledmatrix_off,
};

DEVICE_DT_DEFINE(LEDMATRIX_NODE,
                 ledmatrix_init, NULL, NULL /* No mutable data */, &config,
		         POST_KERNEL, CONFIG_LEDMATRIX_INIT_PRIORITY, &driver_api);
