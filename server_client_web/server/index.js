var express = require('express');
var app = express();
var server = require('http').Server(app);
const io = require('socket.io')(server);

app.use(express.static('public'));

io.on('connection', function(socket){

    socket.on('position',function(data){
        console.log(data);
        io.sockets.emit('desde_servidor',data);
    });

    socket.on('telemetria',function(data){
        console.log(data);
        io.sockets.emit('desde_servidor',data);
    });

    socket.on('ESP32',function(data){
        console.log(data);
        io.sockets.emit('desde_servidor',data);
    });

    socket.on('controlar_ESP',function(data){
        console.log(data);
        io.sockets.emit('controlar_ESP',data);
    });

});


server.listen(5001, function(){
    console.log("Servidor corriendo en el puerto 5001.")
});