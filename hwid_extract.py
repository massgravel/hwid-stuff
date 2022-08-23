#!/usr/bin/env python

from functools import reduce
import xml.etree.ElementTree as ET

from base64 import b64decode

class Ticket:
    @staticmethod
    def get_properties(genAuth: ET.Element) -> str:
        properties = genAuth.find('./{*}genuineProperties/{*}properties')
        return properties.text

    @staticmethod
    def split_keyval(x: str) -> dict:
        parameters = {}
        
        for params in x.split(';'):
            if not params or params == '\x00':
                break
            key_val = params.split('=')
            parameters[key_val[0]] = key_val[1]
        return parameters

    @staticmethod
    def get_hwid(genAuth: ET.Element) -> str:
        props = Ticket.split_keyval(Ticket.get_properties(genAuth))
        params = Ticket.split_keyval(b64decode(props['SessionId'] + '===').decode('utf-16'))

        return params['Hwid']

if __name__ == '__main__':
    import argparse

    main_parser = argparse.ArgumentParser(
        'hwid_extract',
        description='Extract the binary hardware id from ticket'
    )
    main_parser.add_argument('input',  type=argparse.FileType('rb'))
    main_parser.add_argument('output', type=argparse.FileType('wb'), nargs='?')
    args = main_parser.parse_args()

    ticket = ET.parse(args.input).getroot()
    hwid = Ticket.get_hwid(ticket)

    hwid_block = b64decode(hwid + '===')
    if args.output is None:
        print(hwid_block)
    else:
        args.output.write(hwid_block)