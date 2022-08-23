#!/usr/bin/env python

from functools import reduce
import xml.etree.ElementTree as ET

from base64 import b64decode

# Microsoft really hates padding base64
# appending "===" is just so that b64decode
# doesn't complain about it.
# It's neither valid padding nor magic.

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
    
    def __init__(self, genuine_authorization: ET.Element):
        self.gen_auth = genuine_authorization
        self.gen_props = genuine_authorization.find('./{*}genuineProperties')

        props = self.gen_props.find('./{*}properties').text
        self.props = self.split_keyval(props)
        self.props['SessionId'] = self.split_keyval(b64decode(self.props['SessionId'] + '===').decode('utf-16'))

if __name__ == '__main__':
    import argparse

    main_parser = argparse.ArgumentParser(
        'ticket_decode',
        description='Print out contents of a GenuineAuthorization ticket'
    )
    main_parser.add_argument('input',  type=argparse.FileType('r'))
    args = main_parser.parse_args()

    ticket = Ticket(ET.parse(args.input).getroot())
    
    # Print out stuff
    
    for prop in ticket.props:
        if prop == 'SessionId':
            for sess_prop in ticket.props['SessionId']:
                print(sess_prop, ticket.props['SessionId'][sess_prop], sep=': ')
            continue
        print(prop, ticket.props[prop], sep=': ')