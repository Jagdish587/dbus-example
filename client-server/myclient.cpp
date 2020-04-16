#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dbus/dbus.h>

using namespace std;

int main(int argc, char ** argv)
{
  DBusError error;
  dbus_error_init(&error);

  DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
  if (!connection || dbus_error_is_set(&error)) {
    perror("Connection error.");
    exit(1);
  }

  const int ret = dbus_bus_request_name(connection, "test.foo.caller", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER || dbus_error_is_set(&error)) {
    perror("Ouch.");
    exit(1);
  }

  DBusMessage *const msg = dbus_message_new_method_call("test.foo.bar",
                                                        "/test/foo/Object",
                                                        "test.foo.Roll",
                                                        "Method");

  if (!msg) {
    perror("Ouch.");
    exit(1);
  }

  dbus_uint32_t serial = dbus_message_get_serial(msg);
  dbus_connection_send(connection, msg, &serial);
  dbus_connection_flush(connection);
  dbus_message_unref(msg);

  exit(0);
}
