/**
 * @file hid-compaq-sdm4540ul.c
 * @author Raleigh Littles <raleighlittles@gmail.com>
 * @brief Linux keyboard driver for Compaq Internet Keyboard (Model: SDM4540UL)
 *        Creates mapping for keys that were otherwise unsupported by the kernel.
 * @version 0.1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/module.h>

// This header is only accessible when you're compiling against the kernel itself, but isn't available inside the usual kernel headers.
//#include "hid-ids.h" 

MODULE_AUTHOR("Raleigh Littles <raleighlittles@gmail.com>");
MODULE_DESCRIPTION("Compaq Internet Keyboard (SDM4540UL) driver");
MODULE_LICENSE("GPL");

#define USB_VENDOR_ID_COMPAQ 0x049f
#define USB_PRODUCT_ID_COMPAQ_SDM4540UL_INTERNET_KEYBOARD 0x000e

static int compaq_sdm4540ul_raw_event(struct hid_device* hdev, struct hid_report*, u8* data, int size) {

    // Debug only
    // For keyboards, the HID data packet always has a length of 8. See https://wiki.osdev.org/USB_Human_Interface_Devices
    // dev_printk(KERN_DEBUG, &hdev->dev, "key event detected! [0] %d [1] %d [2] %d [3] %d [4] %d [5] %d [6] %d [7] %d ", data[0],
    //                                                                                                                   data[1],
    //                                                                                                                   data[2],
    //                                                                                                                   data[3],
    //                                                                                                                   data[4],
    //                                                                                                                   data[5],
    //                                                                                                                   data[6],
    //                                                                                                                   data[7]);

	// You'll need to get a pointer to the actual input_dev object to be able to produce a key event from it
	struct input_dev* input;
	struct hid_input* hidinput;
	hidinput = list_entry(hdev->inputs.next, struct hid_input, list);
	input = hidinput->input;

    int key_on, key_off;
    key_on = 1;
    key_off = 0;
    
    if ((data[0] == 4) && (data[3] == 16)) {
        // Magnifying glass
        input_report_key(input, KEY_F13, key_on);
        input_report_key(input, KEY_F13, key_off);
    }

    else if ((data[0] == 4) && (data[3] == 32)) {
        // Bullseye, currently not implemented
        input_report_key(input, KEY_F14, key_on);
        input_report_key(input, KEY_F14, key_off);
    }

    else if ((data[0] == 4) && (data[3] == 1)) {
        // Mail (envelope)
        input_report_key(input, KEY_F15, key_on);
        input_report_key(input, KEY_F15, key_off);
    }

    else if ((data[0] == 4) && (data[3] == 8)) {
        // Info 'i'
        input_report_key(input, KEY_F16, key_on);
        input_report_key(input, KEY_F16, key_off);
    }

    else if ((data[0] == 4) && (data[3] == 4)) {
        // Q logo
        input_report_key(input, KEY_F17, key_on);
        input_report_key(input, KEY_F17, key_off);
    }

    else if ((data[0] == 4) && (data[3] == 64)) {
        // e-marketplace
        input_report_key(input, KEY_F18, key_on);
        input_report_key(input, KEY_F18, key_off);
    }

    else if ((data[0] == 4) && (data[3] == 2)) {
        // Movies/Music
        input_report_key(input, KEY_F19, key_on);
        input_report_key(input, KEY_F19, key_off);
    }

    else if ((data[0] == 3) && (data[4] == 1)) {
        // Help ('?')
        input_report_key(input, KEY_F20, key_on);
        input_report_key(input, KEY_F20, key_off);
    }

    else if ((data[0] == 3) && (data[4] == 2)) {
        // Person/Contact
        input_report_key(input, KEY_F21, key_on);
        input_report_key(input, KEY_F21, key_off);
    }

    else {
        // In this case, the key being pressed is NOT one of the 9 special keys (the 7 from the blue bar and then the 2 on the right of that)
        // but instead, it's a key that would normally be already supported by the default kernel driver.
        // Hence, in this case, we're basically deferring to the default kernel interpretation -- so that normal key functionality still works
        input_report_key(input, data[2], key_on);
        input_report_key(input, data[2], key_off);
    }

    return 0;
}


static void compaq_sdm4540ul_remove(struct hid_device* hdev) {
    hid_hw_stop(hdev);
	dev_printk(KERN_INFO, &hdev->dev, "Compaq Internet Keyboard (SDM4540UL) removed");
}

static const struct hid_device_id compaq_sdm4540ul_id_table[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_COMPAQ, USB_PRODUCT_ID_COMPAQ_SDM4540UL_INTERNET_KEYBOARD) },
	{} // Last entry must be empty
};

static int compaq_probe(struct hid_device *hdev, const struct hid_device_id *id) {

    int ret;

    ret = hid_parse(hdev);

    if (ret) {
        hid_err(hdev, "Error parsing Compaq Internet Keyboard\n");
        return ret;
    }

    ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
    if (ret) {
        hid_err(hdev, "Error failed to start Compaq Internet Keyboard\n");
        return ret;
    }

    return 0;
}

MODULE_DEVICE_TABLE(hid, compaq_sdm4540ul_id_table);

static struct hid_driver compaq_sdm4540ul_driver = {
    .name = "Compaq Internet Keyboard (SDM4540UL)",
    .id_table = compaq_sdm4540ul_id_table,
    .probe = compaq_probe,
    .remove = compaq_sdm4540ul_remove,
    .raw_event = compaq_sdm4540ul_raw_event
};

module_hid_driver(compaq_sdm4540ul_driver);