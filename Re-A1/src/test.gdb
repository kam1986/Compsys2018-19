# start
# TODO: Add debugging commands here.

# p = print, x: print in hexadecimal, t: print in binary,
# d: print in decimal
p "Hexadecimal representation of 192_10:"
p/x  192
p "Binary      representation of 192_10:"
p/t  192
p "Binary      representation of 80_16:"
p/t 0x80
p "Decimal     representation of 80_16:"
p/d 0x80
p "Hexadecimal representation of 110_2:"
p/x 0b110
p "Decimal     representation of 110_2:"
p/d 0b110

# Defining macros
macro define UTF8_2B(value)   (((value & 0xe0) == 0xc0) ? 1 : 0) // 2-byte
macro define UTF8_3B(value)   (((value & 0xf0) == 0xe0) ? 1 : 0) // 3-byte
macro define UTF8_4B(value)   (((value & 0xf8) == 0xf0) ? 1 : 0) // 4-byte
macro define UTF8_CONT(value) (((value & 0xc0) == 0x80) ? 1 : 0) // continuation byte

p "Some basic tests.."
p UTF8_CONT(128) != 0
p UTF8_2B(192)   != 0
p UTF8_3B(224)   != 0
p UTF8_4B(240)   != 0
#
p "Testing UTF8_CONT.."
p UTF8_CONT(128 + 1)  != 0
p UTF8_CONT(128 | 1)  != 0
p UTF8_CONT(128 | 63) != 0
p UTF8_CONT(128 | 63)  > 0
p UTF8_CONT(128 + 64) == 0
p UTF8_CONT(128 | 64) == 0
#
p "Testing UTF8_2B.."
p UTF8_2B(128 + 64)      != 0
p UTF8_2B(128 | 64)      != 0
p UTF8_2B(128 | 64 | 31) != 0
p UTF8_2B(128 | 64 | 31)  > 0
p UTF8_2B(128 + 64 + 32) == 0
p UTF8_2B(128 | 64 | 32) == 0
#
p "Testing UTF8_3B.."
p UTF8_3B(128 + 64 + 32)      != 0
p UTF8_3B(128 | 64 | 32)      != 0
p UTF8_3B(128 | 64 | 32 | 15) != 0
p UTF8_3B(128 | 64 | 32 | 15)  > 0
p UTF8_3B(128 + 64 + 32 + 16) == 0
p UTF8_3B(128 | 64 | 32 | 16) == 0
#
p "Testing UTF8_4B.."
p UTF8_4B(128 + 64 + 32 + 16)     != 0
p UTF8_4B(128 | 64 | 32 | 16)     != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7)  > 0
p UTF8_4B(128 + 64 + 32 + 16 + 8) == 0
p UTF8_4B(128 | 64 | 32 | 16 | 8) == 0
#
p "More student tests.."
p UTF8_CONT(0)        == 0
p UTF8_2B(0)          == 0
p UTF8_3B(0)          == 0
p UTF8_4B(0)          == 0
#
p UTF8_CONT(64 << 1)  != 0
p UTF8_CONT(256 >> 1) != 0
#
p "Edge cases.."
p UTF8_2B(191) == 0
p UTF8_2B(192) != 0
p UTF8_2B(223) != 0
p UTF8_2B(224) == 0
#
p UTF8_3B(223) == 0 
p UTF8_3B(224) != 0
p UTF8_3B(239) != 0
p UTF8_3B(240) == 0
#
p UTF8_4B(239) == 0
p UTF8_4B(240) != 0
p UTF8_4B(247) != 0
p UTF8_4B(248) == 0
#
p UTF8_CONT(127) == 0
p UTF8_CONT(128) != 0
p UTF8_CONT(191) != 0
p UTF8_CONT(192) == 0

# Quits 
q