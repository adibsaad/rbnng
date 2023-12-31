#! /usr/bin/env ruby
require 'async'
require 'nng'

ADDR = "ipc:///tmp/reqrep.ipc"

def start_client(i)
  sock = NNG::Socket::Req0::Raw.new
  sock.dial(ADDR)

  loop_times = (rand * 10).ceil
  puts "[client #{i}] sending #{loop_times} messages"

  loop_times.times do
    sock.send_msg(NNG::Utils.new_request_id, "msg from client #{i}")
    msg = sock.get_msg
  end
  puts "[client #{i}] finished"
  sock.send_msg(NNG::Utils.new_request_id, "fin-#{i}")
end

def start_server(total_clients)
  sock = NNG::Socket::Rep0::Raw.new
  sock.listen(ADDR)
  running_set = Set.new

  total_clients.times do |i|
    running_set.add i
  end

  puts "[server] listening at #{ADDR}"

  loop do
    msg = sock.get_msg
    body = msg.body
    if body.start_with? 'fin'
      id = body.split('-')[1].to_i
      running_set.delete(id)
      puts "[server] client #{id} sent 'fin' message (remaining: #{running_set.to_a.join(', ')})"
      break if running_set.empty?
    else
      # sleep 0.5
      sock.send_msg msg.header, msg.body
    end
  end

  puts "[server] finished"
end

def start
  total_clients = 10

  Async do
    start_server(total_clients)
  end

  # Wait for server to start
  sleep 0.1

  total_clients.times { |i|
    Async do
      start_client i
    end
  }
end

Async do |t|
  start
end
