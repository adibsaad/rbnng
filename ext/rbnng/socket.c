/*
 * Copyright (c) 2021 Adib Saad
 *
 */
#include "socket.h"
#include "msg.h"
#include "rbnng.h"
#include <ruby.h>
#include <ruby/thread.h>

static void
socket_free(void* ptr)
{
  RbnngSocket* p_rbnngSocket = (RbnngSocket*)ptr;
  nng_close(p_rbnngSocket->socket);
  xfree(p_rbnngSocket);
}

VALUE
socket_alloc(VALUE klass)
{
  RbnngSocket* p_rbnngSocket = ZALLOC(RbnngSocket);
  return rb_data_object_wrap(klass, p_rbnngSocket, 0, socket_free);
}

void*
socket_get_msg_blocking(RbnngSocket* p_rbnngSocket)
{
  nng_msg* p_msg = NULL;
  int rv;
  if ((rv = nng_recvmsg(p_rbnngSocket->socket, &p_msg, 0)) != 0) {
    return (void*)rv;
  }

  p_rbnngSocket->p_getMsgResult = p_msg;
  return (void*)0;
}

VALUE
socket_get_msg(VALUE self)
{
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);

  int rv = (int) rb_thread_call_without_gvl(socket_get_msg_blocking, p_rbnngSocket, 0, 0);
  /* int rv = (int) socket_get_msg_blocking(p_rbnngSocket); */

  if (rv == 0) {
    RbnngMsg* p_newMsg;
    VALUE newMsg = rb_class_new_instance(0, 0, rbnng_MsgClass);
    Data_Get_Struct(newMsg, RbnngMsg, p_newMsg);
    p_newMsg->p_msg = p_rbnngSocket->p_getMsgResult;
    return newMsg;
  } else {
    raise_error(rv);
    return Qnil;
  }
}

void*
socket_send_msg_blocking(void* data)
{
  RbnngSendMsgReq* p_sendMsgReq = (RbnngSendMsgReq*)data;
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(p_sendMsgReq->socketObj, RbnngSocket, p_rbnngSocket);
  int rv;
  nng_msg* p_msg;

  if ((rv = nng_msg_alloc(&p_msg, 0)) != 0) {
    return (void*)rv;
  }

  if (RB_TEST(p_sendMsgReq->nextMsgHeader)) {
    rv = nng_msg_header_append(p_msg,
        StringValuePtr(p_sendMsgReq->nextMsgHeader),
        RSTRING_LEN(p_sendMsgReq->nextMsgHeader));
    if (rv != 0) {
      return (void*)rv;
    }
  }

  rv = nng_msg_append(p_msg,
      StringValuePtr(p_sendMsgReq->nextMsgBody),
      RSTRING_LEN(p_sendMsgReq->nextMsgBody));
  if (rv != 0) {
    return (void*)rv;
  }

  // nng_sendmsg takes ownership of p_msg, so no need to free it afterwards.
  if ((rv = nng_sendmsg(p_rbnngSocket->socket, p_msg, 0)) != 0) {
    return (void*)rv;
  }

  return (void*)0;
}

VALUE
socket_send_msg(VALUE self, VALUE rb_strMsg)
{
  Check_Type(rb_strMsg, T_STRING);
  RbnngSendMsgReq sendMsgReq = {
    .socketObj = self,
    .nextMsgBody = rb_strMsg,
  };
  int rv = (int) rb_thread_call_without_gvl(socket_send_msg_blocking,
      &sendMsgReq, 0, 0);
  /* int rv = (int) socket_send_msg_blocking(&sendMsgReq); */
  if (rv != 0) {
    raise_error(rv);
  }

  return Qnil;
}

VALUE
socket_send_msg_raw(VALUE self, VALUE rb_strHeader, VALUE rb_strBody)
{
  Check_Type(rb_strHeader, T_STRING);
  Check_Type(rb_strBody, T_STRING);
  RbnngSendMsgReq sendMsgReq = {
    .socketObj = self,
    .nextMsgBody = rb_strBody,
    .nextMsgHeader = rb_strHeader,
  };
  int rv = (int) rb_thread_call_without_gvl(socket_send_msg_blocking,
      &sendMsgReq, 0, 0);
  /* int rv = (int) socket_send_msg_blocking(&sendMsgReq); */
  if (rv != 0) {
    raise_error(rv);
  }

  return Qnil;
}

VALUE
socket_dial(VALUE self, VALUE url)
{
  Check_Type(url, T_STRING);
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);

  int rv;
  if ((rv = nng_dial(p_rbnngSocket->socket, StringValueCStr(url), 0, 0)) != 0) {
    raise_error(rv);
  }
}

VALUE
socket_listen(VALUE self, VALUE url)
{
  Check_Type(url, T_STRING);
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);

  int rv;
  if ((rv = nng_listen(p_rbnngSocket->socket, StringValueCStr(url), NULL, 0)) !=
      0) {
    raise_error(rv);
  }

  return Qnil;
}

VALUE
socket_get_opt_int(VALUE self, VALUE opt)
{
  Check_Type(opt, T_STRING);
  RbnngSocket* p_rbnngSocket;
  Data_Get_Struct(self, RbnngSocket, p_rbnngSocket);

  int rv;
  int val;
  if ((rv = nng_socket_get_int(p_rbnngSocket->socket, StringValueCStr(opt), &val)) !=
      0) {
    raise_error(rv);
  }

  return INT2NUM(val);
}
