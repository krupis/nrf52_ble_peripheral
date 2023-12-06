/* main.c - Application main entry point */

/*
 * Copyright (c) 2019 Aaron Tsui <aaron.tsui@outlook.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_BAS_VAL))};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err)
	{
		LOG_ERR("Connection failed (err 0x%02x)\n", err);
	}
	else
	{
		LOG_INF("Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_ERR("Disconnected (reason 0x%02x)\n", reason);
}

static void bt_ready(void)
{
	int err;

	LOG_INF("Bluetooth initialized\n");

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err)
	{
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return;
	}

	LOG_INF("Advertising successfully started\n");
}

int main(void)
{
	int err;

	err = bt_enable(NULL);
	if (err)
	{
		LOG_INF("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	bt_ready();

	/* Implement indicate. At the moment there is no suitable way
	 * of starting delayed work so we do it here
	 */
	while (1)
	{
		k_sleep(K_SECONDS(1));
		LOG_INF("hello world \n");
	}
	return 0;
}
