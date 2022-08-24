# HWID Stuff

Here are some basic example utilities for GenuineAuthorization tickets and HWID block blobs.

If you'd like to learn more about the things those scripts work with, go to [massgravel/activation](https://github.com/massgravel/activation) (if I updated it already).

# Info

## `hwid_extract.py`

This scripts extracts the HWID block blob from a GenuineAuthorization ticket into a file:

```sh
# Writes to file
python hwid_extract.py GenuineTicket.xml hwid.bin

# Writes to stdout
python hwid_extract.py GenuineTicket.xml
```

## `hwid_dec.c`

This is a program that prints out all the individual fields in the HWID block blob. Build with `make`.

```sh
hwid_dec hwid.bin
```

## `ticket_decode.py`

This is a script that prints out meaningful individual fields in a ticket.

```sh
python ticket_decode.py GenuineTicket.xml
```