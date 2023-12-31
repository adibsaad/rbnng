# frozen_string_literal: true


module NNG
  module Socket
    SENDER_SOCKETS = [
      Pair0,
      Pair1,
      Req0,
      Rep0,
      Push0,
      Pub0,
      Surveyor0,
    ]

    module SenderMethods
      def send_fd
        get_opt_int 'send-fd'
      end


      def send_msg(...)
        if Fiber.scheduler
          @send_io ||= ::IO.for_fd send_fd
          @send_io.wait_readable # TODO: timeout
        end

        _send_msg(...)
      end
    end

    SENDER_SOCKETS.each do |send_klass|
      send_klass.include SenderMethods
    end
  end
end
