var WebSocketServer = require('websocket').server;
var WebSocketClient = require('websocket').client;
var http = require('http');

var common = require('../common.js');
var METADATA_TYPE = common.METADATA_TYPE;
var port = 8080;

var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});

server.listen(port, function() {
    console.log((new Date()) + ' Server is listening on port '+port);
});

wsServer = new WebSocketServer({
    httpServer: server,
         //TODO: https://github.com/theturtle32/WebSocket-Node/blob/master/test/scripts/echo-server.js
         // You should not use autoAcceptConnections for production
         // applications, as it defeats all standard cross-origin protection
         // facilities built into the protocol and the browser.  You should
         // *always* verify the connection's origin and decide whether or not
         // to accept it.
         autoAcceptConnections: false
});



//TODO:
var fs_DB = [];
var fake_count = 0;

function protocolIsAllowed(request) {
    // put logic here to detect whether the specified origin is allowed.
    if(request["httpRequest"]["headers"]["sec-websocket-protocol"] != 'sync-protocol')
    { console.log(request["httpRequest"]["headers"]["sec-websocket-protocol"]);
        return false;}
    else{
    return true;}
}


function ms_event_handler(connection){
    connection.on("message", function(msg){
        if (msg.type === 'utf8') {
            var recv_json = JSON.parse(msg.utf8Data);
            console.log(msg)
            switch(recv_json.metadata_type)
            {
                case METADATA_TYPE.FS_REGISTER:
                {
                    console.log("MS: receive FS_REGISTER");
                    var fs_IP = (connection.remoteAddress).replace("::ffff:", "");
                    var fs_record = {"userID": "fakeRay"+fake_count, "fs_IP": fs_IP, 
                                    "fs_port": recv_json.fs_port, "fs_protocol": recv_json.fs_protocol};
                    //TODO: check fs_protocol, fs_IP, fs_port is undefined?
                    //TODO:fake_count
                    //fake_count = fake_count+1;
                    fs_DB.push(fs_record);
                    console.log("MS: register a FS: "+JSON.stringify(fs_record));
                    // send FS_REGISTER_OK back to the fs
                    var sent_data = {"metadata_type": METADATA_TYPE.FS_REGISTER_OK};
                    connection.sendUTF(JSON.stringify(sent_data));        
                    console.log("MS: send FS_REGISTER_OK to FS");
                    break;
                }
                case METADATA_TYPE.UPLOAD_REQUEST:
                {
                    console.log("MS: receive UPLOAD_REQUEST from client");
                    //TODO: the method to find the fs_record can be better
                    var userID = recv_json.userID;
                    for(var i=0; i<fs_DB.length && fs_DB[i].userID != userID; i++)
                    {}
                    var fs_record = fs_DB[i];
                    var client = new WebSocketClient();
                    client.connect("ws://"+fs_record.fs_IP+":"+fs_record.fs_port, fs_record.fs_protocol);
                    client.on("connect", function(fs_connection){
                        var sent_data = recv_json;
                        console.log("MS: send UPLOAD_REQUEST to FS");
                        fs_connection.sendUTF(JSON.stringify(sent_data)); 
                        fs_connection.on("message", function(fs_msg){
                            var fs_msg_json = JSON.parse(fs_msg.utf8Data);
                            if(fs_msg_json.metadata_type == METADATA_TYPE.UPLOAD_READY)
                            {console.log("MS: transfer UPLOAD_READY from FS to CLIENT")}
                            connection.sendUTF(fs_msg.utf8Data);
                            fs_connection.close();
                        });
                        //TODO: abort() is needed?
                        //client.abort();
                    });
                }
                default:
                    break;
            }
           
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            connection.sendBytes(message.binaryData);
        }
    });               
};

wsServer.on('request', function(request) {
    if (!protocolIsAllowed(request)) {
        // Make sure we only accept requests from an allowed origin
        request.reject();
        //console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
        return;
    }
    else
    {
        var ms_connection = request.accept('sync-protocol', request.origin);
        console.log((new Date()) + ' Connection accepted.');
        ms_event_handler(ms_connection)
        
        ms_connection.on('close', function(reasonCode, description) {
                //console.log((new Date()) + ' Peer ' + ms_connection.remoteAddress + ' disconnected.');
                });
    }
});



wsServer.on('close', function(webSocketConnection, closeReason, description){
    console.log('connection closed');
});

