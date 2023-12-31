/*
 * Copyright (c) 2021 Adib Saad
 *
 */
#include "msg.h"
#include "rbnng.h"
#include "socket.h"
#include <nng/protocol/pipeline0/push.h>
#include <ruby.h>

static VALUE
socket_push0_initialize(VALUE self)
{
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);
  int rv;
  if ((rv = nng_push0_open(&p_rbnngSocket->socket)) != 0) {
    raise_error(rv);
    return Qnil;
  }

  return self;
}

void
rbnng_push0_Init(VALUE nng_module)
{
  VALUE rbnng_SocketModule = rb_define_module_under(nng_module, "Socket");
  VALUE rbnng_SocketPush0Class =
    rb_define_class_under(rbnng_SocketModule, "Push0", rb_cObject);
  rb_define_alloc_func(rbnng_SocketPush0Class, socket_alloc);
  rb_define_method(
    rbnng_SocketPush0Class, "initialize", socket_push0_initialize, 0);
  rb_define_method(rbnng_SocketPush0Class, "_send_msg", socket_send_msg, 1);
  rb_define_method(rbnng_SocketPush0Class, "dial", socket_dial, 1);
  rb_define_method(rbnng_SocketPush0Class, "get_opt_int", socket_get_opt_int, 1);
}
