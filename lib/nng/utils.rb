# frozen_string_literal: true


module NNG
  module Utils
    module_function


    # length in bytes of an ID in the header (stack of IDs)
    ID_LEN = 4


    def new_request_id
      first, *rest = Random.bytes(ID_LEN).bytes
      first |= 0b1000_0000
      [first, *rest].pack('C*')
    end


    def new_peer_id
      first, *rest = Random.bytes(ID_LEN).bytes
      first &= 0b0111_1111
      [first, *rest].pack('C*')
    end
  end
end
