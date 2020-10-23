const path = '/dev/tty.usbmodem14601'

const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort(path, { baudRate: 115200 })

const parser = new Readline()
port.pipe(parser)

let led = 'on'

parser.on('data', line => {
    const [item, state] = line.trim().split(':')
    if(item === 'Response') return
    console.log({item, state})

    // Faire clignotter la led
    led = led === 'on' ? 'off' : 'on'
    const cmd = 'L1 '+ led +'\n'
    port.write(cmd)
})


