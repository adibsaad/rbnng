/*
 * Copyright (c) 2021 Adib Saad
 *
 */
#include "msg.h"
#include "rbnng.h"
#include "socket.h"
#include <nng/protocol/reqrep0/req.h>
#include <ruby.h>
#include <ruby/thread.h>

static VALUE
socket_req0_open(VALUE self)
{
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);
  int rv;
  if ((rv = nng_req0_open(&p_rbnngSocket->socket)) != 0) {
    raise_error(rv);
    return Qnil;
  }

  return self;
}

static VALUE
socket_req0_open_raw(VALUE self)
{
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);
  int rv;
  if ((rv = nng_req0_open_raw(&p_rbnngSocket->socket)) != 0) {
    raise_error(rv);
    return Qnil;
  }

  return self;
}

void
rbnng_req0_Init(VALUE nng_module)
{
  VALUE rbnng_SocketModule = rb_define_module_under(nng_module, "Socket");
  VALUE rbnng_SocketReq0Class =
    rb_define_class_under(rbnng_SocketModule, "Req0", rb_cObject);
  rb_define_alloc_func(rbnng_SocketReq0Class, socket_alloc);
  rb_define_private_method(
    rbnng_SocketReq0Class, "_open", socket_req0_open, 0);
  rb_define_private_method(
    rbnng_SocketReq0Class, "_open_raw", socket_req0_open_raw, 0);
  rb_define_method(rbnng_SocketReq0Class, "_get_msg", socket_get_msg, 0);
  rb_define_method(rbnng_SocketReq0Class, "_send_msg", socket_send_msg, 1);
  rb_define_method(rbnng_SocketReq0Class, "dial", socket_dial, 1);
  rb_define_method(rbnng_SocketReq0Class, "get_opt_int", socket_get_opt_int, 1);

  // Req0::Raw < Req0
  VALUE rbnng_SocketReq0RawClass =
    rb_define_class_under(rbnng_SocketReq0Class, "Raw", rbnng_SocketReq0Class);
  rb_define_private_method(
    rbnng_SocketReq0RawClass, "initialize", socket_req0_open_raw, 0);
  rb_define_method(rbnng_SocketReq0RawClass, "_get_msg", socket_get_msg, 0);
  rb_define_method(rbnng_SocketReq0RawClass, "_send_msg", socket_send_msg_raw, 2);
}
