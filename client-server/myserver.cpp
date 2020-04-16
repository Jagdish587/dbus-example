
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dbus/dbus.h>
#include <unistd.h>

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

  const int ret = dbus_bus_request_name(connection, "test.foo.bar", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER || dbus_error_is_set(&error)) {
    perror("Ouch.");
    exit(1);
  }

  while (true) {
    dbus_connection_read_write(connection, 0);
    DBusMessage *const msg = dbus_connection_pop_message(connection);
    if (!msg) {
      continue;
    }
    if (dbus_message_is_method_call(msg, "test.foo.Roll", "Method")) {
         std::cout << "Method call received" << std::endl;
    }
    dbus_message_unref(msg);
  }

  return 0;
}
