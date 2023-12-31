# frozen_string_literal: true


module NNG
  module Socket
    RECEIVER_SOCKETS = [
      Pair0,
      Pair1,
      Req0,
      Rep0,
      Pull0,
      Sub0,
      Respondent0,
    ]

    module ReceiverMethods
      def recv_fd
        get_opt_int 'recv-fd'
      end


      def get_msg
        if Fiber.scheduler
          @recv_io ||= ::IO.for_fd recv_fd
          @recv_io.wait_readable # TODO: timeout
        end

        _get_msg
      end
    end

    RECEIVER_SOCKETS.each do |recv_klass|
      recv_klass.include ReceiverMethods
    end
  end
end
