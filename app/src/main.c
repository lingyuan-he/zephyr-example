#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


//!!
#include <zephyr/drivers/gpio.h>

#include "app_version.h"
#include "app/drivers/ledmatrix.h"
#include "app/lib/accel.h"

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#define MIN_GRAVITY_ACCEL 9

int main(void)
{
    int rc;
    const struct device *accel, *ledmatrix;
    struct accel_values accel_xyz;

    printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

    accel = get_accel_device();
    if (accel == NULL) {
        return -1;
    }

    /*
     * An alternative to get the device:
     * DEVICE_DT_GET(DT_NODELABEL(ledmatrix_label))
     */
    ledmatrix = DEVICE_DT_GET(DT_PATH(custom_ledmatrix));
    if (ledmatrix == NULL) {
        LOG_ERR("Failed to get the LED matrix device");
        return -1;
    }

	while (1) {
        rc = get_accel_values(accel, &accel_xyz);

        if (rc != 0) {
            LOG_ERR("Failed to get accelerometer values: %d, retrying in 5 "
                    "seconds", rc);
            k_sleep(K_SECONDS(5));
            continue;
        }

        /* When turning the board on a side, the accelerometer will detect
         * gravity on its x or y axis instead of the z axis.
         * When the LED matrix is initially facing up, which is on the reverse
         * side of the accelerometer, below are the results of turning the
         * board sideways:
         * Left - negative X.
         * Right - positive X.
         * Up - positive Y.
         * Down - negative Y.
         * Detection of gravity will be in the above sequence, and 9m/s2 is used
         * as the threshold.
         */
        if (accel_xyz.x < -MIN_GRAVITY_ACCEL) {
            LOG_INF("The board was turned on its left side");
            ledmatrix_set_left_col(ledmatrix);
        } else if (accel_xyz.x > MIN_GRAVITY_ACCEL) {
            LOG_INF("The board was turned on its right side");
            ledmatrix_set_right_col(ledmatrix);
        } else if (accel_xyz.y > MIN_GRAVITY_ACCEL) {
            LOG_INF("The board was turned on its up side");
            ledmatrix_set_top_row(ledmatrix);
        } else if (accel_xyz.y < -MIN_GRAVITY_ACCEL) {
            LOG_INF("The board was turned on its down side");
            ledmatrix_set_bottom_row(ledmatrix);
        } else {
            ledmatrix_turn_off(ledmatrix);
        }

        k_sleep(K_SECONDS(1));
    }

    return 0;
}

