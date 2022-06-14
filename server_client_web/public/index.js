var socket = io.connect('localhost:5001', {'forceNew':true});

socket.on('desde_servidor', function(data){
    
    //console.log(data);
    
    /*
    var cadena = data.map(function(element,index){
        return "<div>" + "<strong>" + element.nom + ": " + "</strong>" + element.msj + "</div>"
    }).join(' ');
    */
    
    //console.log(cadena);
    //data_json = JSON.parse(data);
    data_json = data;
    document.getElementById(data_json.id).innerHTML = "Temperatura = <font color='blue'>"+ data_json.temperatura+"</font>, Humedad = <font color='green'>"+data_json.humedad+"</font>, Luz = <font color='purple'>"+data_json.luz+"</font>";

});

