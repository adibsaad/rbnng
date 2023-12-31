/*
 * Copyright (c) 2021 Adib Saad
 *
 */
#include "msg.h"
#include "rbnng.h"
#include "socket.h"
#include <nng/protocol/pubsub0/sub.h>
#include <ruby.h>

static VALUE
socket_sub0_initialize(VALUE self)
{
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);
  int rv;
  if ((rv = nng_sub0_open(&p_rbnngSocket->socket)) != 0) {
    raise_error(rv);
    return Qnil;
  }

  if ((rv = nng_socket_set(p_rbnngSocket->socket, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0) {
    raise_error(rv);
    return Qnil;
  }

  return self;
}

void
rbnng_sub0_Init(VALUE nng_module)
{
  VALUE rbnng_SocketModule = rb_define_module_under(nng_module, "Socket");
  VALUE rbnng_SocketSub0Class =
    rb_define_class_under(rbnng_SocketModule, "Sub0", rb_cObject);
  rb_define_alloc_func(rbnng_SocketSub0Class, socket_alloc);
  rb_define_method(
    rbnng_SocketSub0Class, "initialize", socket_sub0_initialize, 0);
  rb_define_method(rbnng_SocketSub0Class, "get_msg", socket_get_msg, 0);
  rb_define_method(rbnng_SocketSub0Class, "dial", socket_dial, 1);
  rb_define_method(rbnng_SocketSub0Class, "get_opt_int", socket_get_opt_int, 1);
}
