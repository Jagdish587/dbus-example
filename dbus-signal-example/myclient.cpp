/*
 * This file is part of the Ideal Library
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@ereslibre.es>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dbus/dbus.h>

using namespace std;

void sendsignal(char* sigvalue)
{
  DBusMessage* msg;
  DBusMessageIter args;
  DBusConnection* conn;
  DBusError err;
  int ret;
  dbus_uint32_t serial = 0;

  printf("Sending signal with value %s\n", sigvalue);

  // initialise the error value
  dbus_error_init(&err);

  // connect to the DBUS system bus, and check for errors
  conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Connection Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (NULL == conn) {
    exit(1);
  }

  // register our name on the bus, and check for errors
  ret = dbus_bus_request_name(conn, "test.signal.source", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Name Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
    exit(1);
  }

  // create a signal & check for errors
  msg = dbus_message_new_signal("/test/signal/Objects", // object name of the signal
                                "test.signal.Type", // interface name of the signal
                                "Test"); // name of the signal
  if (NULL == msg)
  {
    fprintf(stderr, "Message Null\n");
    exit(1);
  }

  // append arguments onto signal
  dbus_message_iter_init_append(msg, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &sigvalue)) {
    fprintf(stderr, "Out Of Memory!\n");
    exit(1);
  }

  // send the message and flush the connection
  if (!dbus_connection_send(conn, msg, &serial)) {
    fprintf(stderr, "Out Of Memory!\n");
    exit(1);
  }
  dbus_connection_flush(conn);

  printf("Signal Sent\n");

  // free the message
  dbus_message_unref(msg);
}

int main(int argc, char ** argv)
{
  // Jagdish , lets add logic for signal


  printf("Adding Logic for Sending SIgnal \n");
  char* mybuffer = "Jagdish , How are you, u good ??";
  sendsignal(mybuffer);

  return 0;

}
