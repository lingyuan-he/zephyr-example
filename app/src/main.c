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

// !!

#define ROW1_PORT_BIT 21
#define ROW2_PORT_BIT 22
#define ROW3_PORT_BIT 15
#define ROW4_PORT_BIT 24
#define ROW5_PORT_BIT 19
  
#define COL1_PORT_BIT 28
#define COL2_PORT_BIT 11
#define COL3_PORT_BIT 31
#define COL4_PORT_BIT 5
#define COL5_PORT_BIT 30
  
static const struct device *gpio0, *gpio1;
int matrix_begin()
{
    int ret;
    // Configure the GPIO's     
    gpio0=DEVICE_DT_GET(DT_NODELABEL(gpio0));      //device_get_binding("GPIO_0");
    if (gpio0 == NULL)
    {
        printf("Error acquiring GPIO 0 interface\n");
        return -1;
    }
    gpio1=DEVICE_DT_GET(DT_NODELABEL(gpio1)); //device_get_binding("GPIO_1");
    if (gpio0 == NULL)
    {
        printf("Error acquiring GPIO 1 interface\n");
        return -2;
    }
    ret = gpio_pin_configure(gpio0,ROW1_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,ROW2_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,ROW3_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,ROW4_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,ROW5_PORT_BIT,GPIO_OUTPUT);
      
    ret = gpio_pin_configure(gpio0,COL1_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,COL2_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,COL3_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio1,COL4_PORT_BIT,GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0,COL5_PORT_BIT,GPIO_OUTPUT);

    ret = gpio_pin_set(gpio0,ROW1_PORT_BIT,1); 
    ret = gpio_pin_set(gpio0,ROW2_PORT_BIT,1);
    ret = gpio_pin_set(gpio0,ROW3_PORT_BIT,1);
    ret = gpio_pin_set(gpio0,ROW4_PORT_BIT,1);
    ret = gpio_pin_set(gpio0,ROW5_PORT_BIT,0);
    ret = gpio_pin_set(gpio0,COL1_PORT_BIT,0);
    ret = gpio_pin_set(gpio0,COL2_PORT_BIT,0);
    ret = gpio_pin_set(gpio0,COL3_PORT_BIT,1);
    ret = gpio_pin_set(gpio1,COL4_PORT_BIT,0);
    ret = gpio_pin_set(gpio0,COL5_PORT_BIT,0);
  
    LOG_INF("End of matrix");
    return 0;   
}


// !!


int main(void)
{
    int rc;
	const struct device *accel, *ledmatrix;
    struct accel_values accel_xyz;

	printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

    // !!
    //matrix_begin();

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

    LOG_ERR("ready %d", device_is_ready(ledmatrix));

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

