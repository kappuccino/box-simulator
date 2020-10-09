// NODE
const express = require('express')
const app = express()
const server = require('http').createServer(app)
const io = require('socket.io')(server)

// ARDUINO
const path = '/dev/tty.usbmodem14601'
const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort(path, { baudRate: 115200 })
const parser = new Readline()
port.pipe(parser)

// SERVE SATIC FILE
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
})

// SOCKET
io.on('connection', function(client){
    console.log('Connection de ', client.id)

    client.on('join', function(msg){
        console.log('client says', msg)
    })
})

parser.on('data', line => {
    const [item, state] = line.trim().split(':')
    if(item === 'Response') return
    console.log({item, state})
    
    io.emit('arduino', {item, state})
})


server.listen(3000, () => {
    console.log('listening on *:3000');
});