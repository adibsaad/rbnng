/*
 * Copyright (c) 2021 Adib Saad
 *
 */
#include "msg.h"
#include "rbnng.h"
#include "socket.h"
#include <nng/protocol/pipeline0/pull.h>
#include <ruby.h>

static VALUE
socket_pull0_initialize(VALUE self)
{
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);
  int rv;
  if ((rv = nng_pull0_open(&p_rbnngSocket->socket)) != 0) {
    raise_error(rv);
    return Qnil;
  }

  return self;
}

void
rbnng_pull0_Init(VALUE nng_module)
{
  VALUE rbnng_SocketModule = rb_define_module_under(nng_module, "Socket");
  VALUE rbnng_SocketPull0Class =
    rb_define_class_under(rbnng_SocketModule, "Pull0", rb_cObject);
  rb_define_alloc_func(rbnng_SocketPull0Class, socket_alloc);
  rb_define_method(
    rbnng_SocketPull0Class, "initialize", socket_pull0_initialize, 0);
  rb_define_method(rbnng_SocketPull0Class, "_get_msg", socket_get_msg, 0);
  rb_define_method(rbnng_SocketPull0Class, "listen", socket_listen, 1);
  rb_define_method(rbnng_SocketPull0Class, "get_opt_int", socket_get_opt_int, 1);
}
