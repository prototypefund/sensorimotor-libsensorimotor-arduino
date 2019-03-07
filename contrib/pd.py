import sigrokdecode as srd
#from .lists import *

class No_more_data(Exception):
    '''This exception is a signal that we should stop parsing an ADU as there
    is no more data to parse.'''
    pass

class Data:
    '''The data class holds the bytes from the serial decoder'''
    def __init__(self, start, end, data):
        self.start = start
        self.end = end
        self.data = data

class Message:
    def __init__(self, parent, start):
            self.data = [] # list of all the data received until now
            self.parent = parent # reference to the decoder class
            self.start = start
            self.last_read = start # the last timestamp parsed by this message object

            # any sensorimotor message needs to be at least 4 bytes long
            self.minimum_length = 4

            # This variable determines when the next frame shoult be started
            self.start_new_frame = False

            # If there has been an error, we want to highlight it so we keep
            # track of errors.
            self.has_error = False

    def add_data(self, start, end, data):
        '''Let the message handle another piece of data'''
        ptype, rxtx, pdata = data
        self.last_read = end
        if ptype == 'DATA':
            self.data.append(Data(start, end, pdata[0]))
            self.parse()

class Decoder(srd.Decoder):
    api_version = 3
    name = 'sensori'
    id = 'sensori'
    longname = 'Sensorimotor protocol'
    desc = 'Sensorimotor protocol'
    inputs = ['uart']
    outputs = ['sensori']
    channels = (
        {'id': 'bus', 'name': 'Bus', 'desc': 'RS485 Bus'},
    )

class Decoder(srd.Decoder):
    options = (
        {'id': 'channel', 'desc': 'bus channel', 'default': 'RX',
            'values': ('RX', 'TX')},
    )
    annotations = (
        ('sync', 'Sync bytes'),
        ('cmd', 'Command'),
        ('mid', 'Motor ID'),
        ('checksum', 'Checksum'),
        ('longpause', 'Long pause'),
        ('shortpause', 'Short pause'),
        ('warnings', 'Warnings'),
    )
    annotation_rows = (
        ('bits', 'Bits', (0, 1, 2, 3)),
        ('pauses', 'Pauses', (4, 5)),
        ('warnings', 'Warnings', (6,)),
    )
    def decode_message(self, ss, es, data):
        '''Decode the next byte or bit (depending on type) in the ADU.
        ss: Start time of the data
        es: End time of the data
        data: Data as passed from the UART decoder'''
        ptype, rxtx, pdata = data

    def __init__(self):
        self.reset()

    def reset(self):
        self.bitlength = None # We will later test how long a bit is.
        self.Message = None   # Start of with empty bus message

    def decode(self, ss, es, data):
        ptype, rxtx, pdata = data

        # We don't have a nice way to get the baud rate from UART, so we have
        # to figure out how long a bit lasts. We do this by looking at the
        # length of (probably) the startbit.
        if self.bitlength is None:
            if ptype == 'STARTBIT' or ptype == 'STOPBIT':
                self.bitlength = es - ss
            else:
                # If we don't know the bitlength yet, we can't start decoding.
                return

