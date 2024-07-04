#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/err.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>

#define WRITE_BUFFER_SIZE 10
#define USER_DRIVER "ShieldMotorDriver"
#define DRIVER_NUM 200

#define GPIO_0 (24) 
#define GPIO_1 (25) 
#define GPIO_2 (19) 
#define GPIO_3 (1) 

#define FORWARD 2
#define BACKWARD 4
#define STOP 1
#define MOTOR_OFF '0'
#define MOTOR_FORWARD '1'
#define MOTOR_BACKWARD '2'

static int GPIO[4] = {GPIO_0, GPIO_1, GPIO_2 , GPIO_3};

dev_t dev=DRIVER_NUM; // MAJOR, MINOR 번호 저장
static struct class *dev_class; 
static struct cdev motor_cdev;
static int count_Shield =0;


//Function Prototype
static int __init motor_driver_init(void);
static void __exit motor_device_exit(void);
static int motor_open(struct inode* inode, struct file * file);
static int motor_release(struct inode* inode, struct file *file);
static ssize_t motor_read(struct file *filp, char __user *buf, size_t len, loff_t *off );
static ssize_t motor_write(struct file *flip , const char *buf, size_t len, loff_t *off);
static void set_motor_pins(int step);
static void timer_callback(struct timer_list *t);


static struct hrtimer hr_timer;
static int current_step = 0;
static int direction = STOP; 

static struct file_operations fops={
    .owner = THIS_MODULE,
    .read = motor_read,
    .write = motor_write,
    .open = motor_open,
    .release = motor_release,
};

// Define the step sequences for a 4-step motor
static int step_sequence[4][4] = {
    {0, 0, 1, 1}, // Step 1
    {0, 1, 1, 0}, // Step 2
    {1, 1, 0, 0}, // Step 3
    {1, 0, 0, 1}  // Step 4
};



static void set_motor_pins(int step) {

    gpio_set_value(GPIO[0], step_sequence[step][0]);
    gpio_set_value(GPIO[1], step_sequence[step][1]);
    gpio_set_value(GPIO[2], step_sequence[step][2]);
    gpio_set_value(GPIO[3], step_sequence[step][3]);
}
enum hrtimer_restart my_hrtimer_callback(struct hrtimer* timer) {
    // Update the step based on the direction
    if (direction == STOP) {
        gpio_set_value(GPIO[0], 0);
        gpio_set_value(GPIO[1], 0);
        gpio_set_value(GPIO[2], 0);
        gpio_set_value(GPIO[3], 0);
        
    }
    else if (direction == FORWARD) {  //Forward
        
       

        if(count_Shield< 8192){
        current_step = (current_step + 1) % 4;
        count_Shield++;
        }else{
        gpio_set_value(GPIO[0], 0);
        gpio_set_value(GPIO[1], 0);
        gpio_set_value(GPIO[2], 0);
        gpio_set_value(GPIO[3], 0);
        }


       // if (current_step == 0) { pr_info("Current direction: FORWARD, Current step: %d\n", current_step); }
        //current_step = (current_step - 1 + 4) % 4; 
    }
    else if (direction == BACKWARD) { //Backward
  
        if(count_Shield> 0){
            current_step = (current_step - 1 + 4) % 4;
            count_Shield--;
        }else{
        gpio_set_value(GPIO[0], 0);
        gpio_set_value(GPIO[1], 0);
        gpio_set_value(GPIO[2], 0);
        gpio_set_value(GPIO[3], 0);
        }
      //  if (current_step == 0) { pr_info("Current direction: BACKWARD, Current step: %d\n", current_step); }
    }
    // Set the motor pins for the current step
    set_motor_pins(current_step);



    // Reschedule the timer for the next step
    //mod_timer(&timer, jiffies + usecs_to_jiffies(0));
    ktime_t interval = ktime_set(0, 3E6);
    hrtimer_forward(timer, hrtimer_cb_get_time(timer), interval);
    return HRTIMER_RESTART;
}



static int motor_open(struct inode *inode, struct file *file){
 //  pr_info("SHEILD MOTOR DRIVER OPEN");
   //count =0;
   count_Shield=0;
   direction = STOP;
   return 0;
}

static int motor_release(struct inode *inode, struct file *file){
  //  pr_info("SHIELD DRIVER RELEASE\n");


    return 0;
}

static ssize_t motor_read(struct file *filp, char __user *buf, size_t len, loff_t *off ){
   
  //  pr_info("SHIELD DRIVER READ\n");
    return 0;
}

static ssize_t motor_write(struct file* flip, const char __user* buf, size_t len, loff_t* off) {
    uint8_t _buf[WRITE_BUFFER_SIZE] = { 0 };
    size_t bytes_to_copy = min(len, sizeof(_buf));
    if (copy_from_user(_buf, buf, bytes_to_copy) != 0) {
      //  pr_err("Failed to copy data from user\n");
        return -EFAULT; // Return a standard error code
    }
  //  pr_info("Motor Diriver Get User Signal %d\n", _buf[0]);
    if (_buf[0] == MOTOR_OFF) { //         
        direction = STOP;
    }
    else {
        if (_buf[0] == MOTOR_FORWARD) {
            direction = FORWARD;
        }
        else if (_buf[0] == MOTOR_BACKWARD) {
            direction = BACKWARD;
        }
    }

    //mod_timer(&timer, jiffies + msecs_to_jiffies(2));
    return count_Shield; // It's more accurate to return the number of bytes actually written
}



static int __init motor_driver_init(void){

    int i;
   
    //Allocate Device MAJOR Number
    if((alloc_chrdev_region(&dev,0,1,USER_DRIVER ))<0){
     //   pr_err("Cannot allocate major number for Shield Motor Driver\n");
        goto r_unreg;
    }
   // pr_info("STAAAAAAAAAAAAAAAAAAART\n");
    
   // pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    //Create CDEV structures
    cdev_init(&motor_cdev, &fops);

   // pr_info("Success to make Shield Motor Class");

    //Add Character Device to System
    if((cdev_add(&motor_cdev, dev, 1))<0){
       // pr_err("Cannot add the Shield Motor Driver to the system\n");
        goto r_del;
    }

  //  pr_info("Success to Add Char Device to System");


    //Create Device File Automatically
        if(IS_ERR(dev_class = class_create(THIS_MODULE,USER_DRIVER ))){
            //pr_err("Cannot create the struct class for device\n");
            goto r_class;
        }

  //  pr_info("Success to create Device File");


    //Create Device
    //@dev_class : pointer to struct class that this device should be registered to
    //@parent: pointer to the parent struct device of this new device
    //@dev_t: Number of Device 
    //@drvdata : the data to be added to the device for callback
    //@fmt: string for device's name


        if(IS_ERR(device_create(dev_class, NULL, dev ,NULL,USER_DRIVER ))){
           // pr_err("Cannot create device\n");
            goto r_device;
        }
   // pr_err("Create Shiled Motor Device");
      

    // Check GPIO is vaild


    if(gpio_is_valid(GPIO[0])==false){ pr_err(" Shield Motor GPIO 0 IS NOT VALID"); goto r_device;}

    if(gpio_is_valid(GPIO[1])==false){ pr_err("Shield Motor GPIO 1 IS NOT VALID"); goto r_device;}

    if(gpio_is_valid(GPIO[2])==false){ pr_err("Shield Motor GPIO 2 IS NOT VALID"); goto r_device;}

    if(gpio_is_valid(GPIO[3])==false){ pr_err("Shield Motor GPIO 3 IS NOT VALID"); goto r_device;}
   


    //Request the GPIO


    if(gpio_request(GPIO[0],"IN0")<0){
        pr_err("Shield MOTOR GPIO ERROR in GPIO 0", GPIO[0]);
        goto r_gpio_GPIO0;
    }

    if(gpio_request(GPIO[1],"IN1")<0){
        pr_err("Shield MOTOR GPIO ERROR in GPIO 1",GPIO[1]);
        goto r_gpio_GPIO1;
    }

    if(gpio_request(GPIO[2],"IN2")<0){
        pr_err("Shield MOTOR GPIO ERROR in GPIO 2",GPIO[2]);
        goto r_gpio_GPIO2;
    }

    if(gpio_request(GPIO[3],"IN3")<0){
        pr_err("ERROR in GPIO 3",GPIO[3]);
        goto r_gpio_GPIO3;
    }

    //SET GPIO Output
   
    for(i = 0; i < 4; ++i){
        gpio_direction_output(GPIO[i], 0);
        gpio_export(GPIO[i], false); 
    }  

    //Timer Setting
     hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hr_timer.function = &my_hrtimer_callback;
    hrtimer_start(&hr_timer, ktime_set(0, 3E6), HRTIMER_MODE_REL);

    pr_info("Motor Module Inserted Successfully\n");
    return 0;


r_gpio_GPIO0:
   gpio_free(GPIO[0]);

r_gpio_GPIO1:
   gpio_free(GPIO[1]);

r_gpio_GPIO2:
   gpio_free(GPIO[2]);

r_gpio_GPIO3:
   gpio_free(GPIO[3]);


r_device:
    device_destroy(dev_class,dev);
r_class: 
    class_destroy(dev_class);
r_del:
    cdev_del(&motor_cdev);
r_unreg:
    unregister_chrdev_region(dev,1);
   
 return -1;

}


static void __exit motor_device_exit(void){
 

    gpio_unexport(GPIO[0]);
    gpio_unexport(GPIO[1]);
    gpio_unexport(GPIO[2]);
    gpio_unexport(GPIO[3]);


    gpio_free(GPIO[0]);
    gpio_free(GPIO[1]);
    gpio_free(GPIO[2]);
    gpio_free(GPIO[3]);
     hrtimer_cancel(&hr_timer);

    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&motor_cdev);
    unregister_chrdev_region(dev,1);
   // pr_info("motor Device Removed Successfully...");
}

module_init(motor_driver_init);
module_exit(motor_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nahyun");
MODULE_DESCRIPTION("Shield Motor Control Device Driver");
MODULE_VERSION("2.0");