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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dbus/dbus.h>
#include <unistd.h>


/**
 * Listens for signals on the bus
 */
void receive()
{
  DBusMessage* msg;
  DBusMessageIter args;
  DBusConnection* conn;
  DBusError err;
  int ret;
  char* sigvalue;

  printf("Listening for signals\n");

  // initialise the errors
  dbus_error_init(&err);

  // connect to the bus and check for errors
  conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Connection Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (NULL == conn) {
    exit(1);
  }

  // request our name on the bus and check for errors
  ret = dbus_bus_request_name(conn, "test.signal.sink", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Name Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
    exit(1);
  }

  // add a rule for which messages we want to see
  dbus_bus_add_match(conn, "type='signal',interface='test.signal.Type'", &err); // see signals from the given interface
  dbus_connection_flush(conn);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Match Error (%s)\n", err.message);
    exit(1);
  }
  printf("Match rule sent\n");

  // loop listening for signals being emmitted
  while (true) {

    // non blocking read of the next available message
    dbus_connection_read_write(conn, 0);
    msg = dbus_connection_pop_message(conn);

    // loop again if we haven't read a message
    if (NULL == msg) {
      sleep(1);
      continue;
    }

    // check if the message is a signal from the correct interface and with the correct name
    if (dbus_message_is_signal(msg, "test.signal.Type", "Test")) {

      // read the parameters
      if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message Has No Parameters\n");
      else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not string!\n");
      else
        dbus_message_iter_get_basic(&args, &sigvalue);

      printf("Got Signal with value \n %s\n", sigvalue);
    }

    // free the message
    dbus_message_unref(msg);
  }
}


int main(int argc, char ** argv)
{
  // to receive a siganl , only interface name and signal name is enough
  receive();

  return 0;
}
