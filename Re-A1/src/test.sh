#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."


# Data test-files:
printf "Hello,\x00World!\n" > test_files/data01.input   # ASCII with NUL + n1
printf "\0\1\5\10" > test_files/data02.input            # Decimals
printf "\3\10\n\100" > test_files/data03.input          # Decimals with linebreak
printf "\x0\xF" > test_files/data04.input               # Hexadecimals
printf "\x4\xD\n" > test_files/data05.input             # Hexadecimals with linebreak
printf "\000\030" > test_files/data06.input             # Octals
printf "\002\042\n" > test_files/data07.input           # Octals with linebreak
printf "\0\x2F\012" > test_files/data08.input           # Dec, Hex & Oct together
printf "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09" > test_files/data09.input # More Hex 
printf "\x0A\x0B\x0C\x0D\x0E\x0F" > test_files/data10.input                 # More Hex
printf "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19" > test_files/data11.input # More Hex 
printf "\x1A\x1B\x1C\x1D\x1E\x1F\x20" > test_files/data12.input             # More Hex


# Empty test-files:
printf "" > test_files/empty.input                     # Empty file


# ASCII text test-files:
printf "Hello, World!\n" > test_files/ascii01.input    # Basic ASCII with linebreak
printf "Hello, World!" > test_files/ascii02.input      # Basic ASCII
printf "              \n" > test_files/ascii03.input   # Spaces with linebreak
echo   "\n" > test_files/ascii04.input                 # Linebreak
printf "\10\128" > test_files/ascii05.input            # Decimals
printf "\x5F\n" > test_files/ascii06.input             # Hexadecimals
printf "\100\110" > test_files/ascii07.input           # Octals
printf "\150\x0C\102" > test_files/ascii08.input       # Dec, Hex & Oct together
printf "!?^$<>=?" > test_files/ascii09.input           # Special symbols
echo   "Hello, World!" > test_files/ascii10.input      # Hello, World! using echo
printf "'{}[]()" > test_files/ascii11.input            # Quotes and Paranthesis
printf ".,:;_-*&@" > test_files/ascii12.input          # More symbols
printf "123456789" > test_files/ascii13.input          # Numbers


# ISO-8859 test-files:
echo "Hello, World! öüä" > test_files/iso-8859         # Even tho we use plain ASCII, we also use öüä 
                                                       # which is ISO-8859.
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859a.input  # Convert iso to utf-8.

echo "ÆØÅ" > test_files/iso-8859                       # Danish letters
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859b.input  # ISO-8859-1 for latin 1 (can also use -4 for latin 4).
echo "ÆØÅ" > test_files/iso-8859                       # Danish letters
iconv -f UTF-8 -t ISO-8859-4 test_files/iso-8859 > test_files/iso-8859c.input  # Can also use ISO-8859-4 for latin 4 
                                                                               # (only for scandinavia/Baltics).
echo "ÆØÅ" > test_files/iso-8859                       # Danish letters
iconv -f UTF-8 -t ISO-8859-10 test_files/iso-8859 > test_files/iso-8859d.input  # Also works with ISO-8859-10 for latin/Nordic.

echo "Hässlich" > test_files/iso-8859                  # German
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859e.input
echo "Français" > test_files/iso-8859                  # French
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859f.input
echo "Fjäder" > test_files/iso-8859                    # Swedish (also works with ISO-8859-4 and 10)
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859g.input
echo "مرحبا" > test_files/iso-8859                    # Latin/Arabic (Arabic alphabet)
iconv -f UTF-8 -t ISO-8859-6 test_files/iso-8859 > test_files/iso-8859h.input
echo "γεια σας" > test_files/iso-8859                 # Latin/Greek  (Greek alphabet)
iconv -f UTF-8 -t ISO-8859-7 test_files/iso-8859 > test_files/iso-8859i.input
echo "שלום" > test_files/iso-8859                     # Latin/Hebrew (Hebrew alphabet)
iconv -f UTF-8 -t ISO-8859-8 test_files/iso-8859 > test_files/iso-8859j.input
echo "áéíóúäëïöüàèìòùâêîôû" > test_files/iso-8859     # Different kinds of accents
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859k.input

echo "±µ¼½¾¬÷" > test_files/iso-8859                  # Math symbols
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859l.input
echo "$£¢¥" > test_files/iso-8859                     # Some currencies
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859m.input
echo "€" > test_files/iso-8859                        # ISO-8859-15 is similar to ISO-8859-1, but replaces less common 
                                                      # symbols with euro sign and some other missing characters. 
iconv -f UTF-8 -t ISO-8859-15 test_files/iso-8859 > test_files/iso-8859n.input


# UTF-8 Unicode text test-files:
echo "Hello, World! ÅÅÅÅ" > test_files/utf-8a.input            # Even tho we use plain ASCII, we also use Å which is UTF-8.
echo "ÆØÅ" > test_files/utf-8b.input                           # Danish   letters
echo "こんにちは" > test_files/utf-8c.input                      # Japanese letters
echo "مرحبا" > test_files/utf-8d.input                         # Arabic   letters
echo "Привет" > test_files/utf-8e.input                        # Russian  letters
echo "হ্যালো" > test_files/utf-8f.input                          # Bengali  letters
echo "Здравейте" > test_files/utf-8g.input                     # Bulgarian  letters
echo "สวัสดี" > test_files/utf-8h.input                          # Thai  letters
echo "नमस्ते" > test_files/utf-8i.input                          # Hindi  letters
echo "ℝℙαβγδΔεθλπμρσΣΩ" > test_files/utf-8j.input              # Math symbols
echo "\/!''@#€£$%&{}[]()+-?¿.,_=<>&" > test_files/utf-8k.input # Common symbols
echo "⚤⚾⛕⚅♥♢♖♐☺☹♏➞³" > test_files/utf-8l.input               # Random special symbols


# Little endian UTF-16 Unicode text
echo "The quick brown fox jumps over the lazy dog.!" > test_files/utf-16  # Some random text
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-a
FILE_ENCODING="$( file --brief --mime-encoding test_files/utf-16-a )"
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-a >\
  test_files/utf-16le-a.input

echo "0123456789" > test_files/utf-16                                    # Some Numbers
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-b
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-b >\
  test_files/utf-16le-b.input

echo "\/!''@#€£$%&{}[]()+-?¿.,_=<>&" > test_files/utf-16                 # Different kinds of symbols
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-c  
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-c >\
  test_files/utf-16le-c.input

echo "⚤⚾⛕⚅♥♢♖♐☺☹♏➞³" > test_files/utf-16                               # Special symbols
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-d
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-d >\
  test_files/utf-16le-d.input

echo "\x00\x09\x0A\x0F\x10\x19\x1A\x1F\x20" > test_files/utf-16          # Some Hexadecimals
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-e
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-e >\
  test_files/utf-16le-e.input

echo "ABCDEFGHIJKLMNOPQRSTUVXYZ" > test_files/utf-16                     # BIG Letters
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-f
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-f >\
  test_files/utf-16le-f.input

echo "abcdefghijklmnopqrstuvxyz" > test_files/utf-16                     # SMALL Letters
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-g
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-g >\
  test_files/utf-16le-g.input

echo "™©®" > test_files/utf-16                                          # Trademark, Copyright, Registered sign
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-h
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-h >\
  test_files/utf-16le-h.input

echo "ʻʼʽʾʿˆˇˊ˜_˽ͲΔ˝˟˥˩" > test_files/utf-16                            # More special symbols
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-i
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-i >\
  test_files/utf-16le-i.input

echo "¬±+-°²³•><=/ΦφΩΡρΠπΣσΛΔωϴ" > test_files/utf-16                    # Math symbols
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-j
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-j >\
  test_files/utf-16le-j.input

echo "ЗдйтеПриহ্যালোんにちمرحبا𠻹𡇙𨳊𩶘𢯊𢱕𢳂𢴈𢵌𠀋ÆØÅ" > test_files/utf-16    # Letters from different countries 
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-k
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-k >\
  test_files/utf-16le-k.input

echo "                \n" > test_files/utf-16                           # Whitespaces with linebreak
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-l
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-l >\
  test_files/utf-16le-l.input


# Big endian UTF-16 Unicode text
iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-a >\
  test_files/utf-16be-a.input                                            # Some random text

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-b >\
  test_files/utf-16be-b.input                                            # Some Numbers

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-c >\
  test_files/utf-16be-c.input                                            # Different kinds of symbols

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-d >\
  test_files/utf-16be-d.input                                            # More symbols

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-e >\
  test_files/utf-16be-e.input                                            # Some Hexadecimals

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-f >\
  test_files/utf-16be-f.input                                            # BIG Letters

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-g >\
  test_files/utf-16be-g.input                                            # SMALL Letters

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-h >\
  test_files/utf-16be-h.input                                            # Trademark, Copyright, Registered sign

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-i >\
  test_files/utf-16be-i.input                                            # More special symbols

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-j >\
  test_files/utf-16be-j.input                                            # Math symbols

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-k >\
  test_files/utf-16be-k.input                                            # Letters from different countries  

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-l >\
  test_files/utf-16be-l.input                                            # Whitespaces with linebreak


echo "Running tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    ${f} | sed \
  -e 's/ASCII text.*/ASCII text/' > test_files/expected \
  -e 's/UTF-8 Unicode text.*/UTF-8 Unicode text/' > test_files/expected \
  -e 's/Big-endian.*/Big-endian UTF-16 Unicode text/' > test_files/expected \
  -e 's/Little-endian.*/Little-endian UTF-16 Unicode text/' > test_files/expected
./file  ${f} > test_files/actual

if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

exit $exitcode