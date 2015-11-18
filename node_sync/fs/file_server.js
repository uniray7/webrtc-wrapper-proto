var WebSocketServer = require("websocket").server;
var WebSocketClient = require("websocket").client;

var fs = require("fs");

var WebRTC = require("wrtc");
var RTCPeerConnection     = WebRTC.RTCPeerConnection;
var RTCSessionDescription = WebRTC.RTCSessionDescription;
var RTCIceCandidate       = WebRTC.RTCIceCandidate;
  
var http = require("http");
var crypto = require('crypto')

var common = require("../common.js")
var METADATA_TYPE = common.METADATA_TYPE;
const FS_MAX_PORT = common.FS_MAX_PORT;
const FS_MIN_PORT = common.FS_MIN_PORT;


// generate port num of fs websocket server
// and generate garbled protocol name
var fs_port = Math.floor(Math.random()*(FS_MAX_PORT - FS_MIN_PORT)+FS_MIN_PORT); 
var md5 = crypto.createHash('md5');
md5.update(fs_port.toString());
var fs_protocol = md5.digest('hex');

var server = http.createServer(function(request,response){
    console.log((new Date()) + "Received request for ", request.url);
    response.writeHead(404);
    response.end();
});

server.listen(fs_port, function() {
    console.log((new Date()) + ' File server is listening on port '+fs_port);
});

fs_wsServer = new WebSocketServer({
    httpServer: server,
    autoAcceptConnections: false
});

fs_wsServer.on('request', function(request) {
   if (!protocolIsAllowed(request, fs_protocol)) {
       // Make sure we only accept requests from an allowed origin
       request.reject();
       console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
       return;
   }
   else{
        var fs_connection = request.accept(fs_protocol, request.origin);
        //console.log((new Date()) + ' Connection accepted.');
        
        set_fs_events(fs_connection);

   }
});



fs_reg_client = new WebSocketClient();

fs_reg_client.connect("ws://192.168.0.103:8080", "sync-protocol");

fs_reg_client.on("connect", 
                function(connection){
                    connection.on("message", function(msg){
                        var recv_json = JSON.parse(msg.utf8Data);
                        switch(recv_json.metadata_type)
                        {
                            case METADATA_TYPE.FS_REGISTER_OK:
                            {
                                console.log("FS: receive FS_REGISTER_OK");
                                console.log("FS: register completed");
                                connection.close();
                                // TODO: abort() needed?
                                fs_reg_client.abort();
                                break;
                            }
                            default:
                                break;
                        }
                    });

                    var sent_data = {"metadata_type":METADATA_TYPE.FS_REGISTER,
                                     "fs_port":fs_port,
                                     "fs_protocol":fs_protocol};

                    console.log("FS: send FS_REGISTER");
                    console.log("FS: send "+JSON.stringify(sent_data)+"to MS");
                    connection.sendUTF(JSON.stringify(sent_data));
                });

fs_reg_client.on("connectFailed", function(error){
    console.log("client connect failed");
});

function set_fs_events(connection)
{
    connection.on('close', function(reasonCode, description) {
        //console.log((new Date()) + ' Peer ' + fs_connection.remoteAddress + ' disconnected.');
    });

    connection.on("message", 
               function(msg){
                   var recv_json = JSON.parse(msg.utf8Data);
                       switch(recv_json.metadata_type)
                       {
                           case METADATA_TYPE.UPLOAD_REQUEST:
                           {
                               console.log("FS: receive UPLOAD_REQUEST");
                               var description = JSON.parse(msg.utf8Data).offer;
                               var answerer = new RTCPeerConnection();
                               answerer.onicegatheringstatechange = onicegatheringstatechange;
                               answerer.ondatachannel = function(event){
                                               console.log("on channel!!");
                                               var dc = event.channel;
                                               var fake_count = 0;
                                               dc.onmessage = function(event){
                                                   //var data = JSON.parse(event.data);
                                                   console.log("receive: "+fake_count+ " size: "+event.data.length);
                                                   fake_count++;
                                               }
                               };
                               
                               console.log("FS: setRemoteDescription");
                               answerer.setRemoteDescription(new RTCSessionDescription(description),
                                                    function(){
                                                        console.log("FS: createAnswer");
                                                        answerer.createAnswer(set_local_description, error_handler);
                                                    }, 
                                                    error_handler);
                               
                               function onicegatheringstatechange(evt){
                                    console.log("CLIENT: ice gathering state: "+answerer.iceGatheringState);
                                    if (answerer.iceGatheringState == "complete")
                                    {
                                        var sent_data = {"metadata_type": METADATA_TYPE.UPLOAD_READY, "answer":answerer.localDescription};
                                        connection.sendUTF(JSON.stringify(sent_data));
                                    }
                                }


                               function set_local_description(description){
                                    console.log("FS: set_local_description");
                                    answerer.setLocalDescription(new RTCSessionDescription(description), null, error_handler)
                               }

                               break;

                           }
                           default:
                               break;
                       }
               });
    function error_handler(error)
    {throw error;}

}


function protocolIsAllowed(request, fs_protocol) {
    // put logic here to detect whether the specified origin is allowed.
    if(request["httpRequest"]["headers"]["sec-websocket-protocol"] != fs_protocol)
    { console.log(request["httpRequest"]["headers"]["sec-websocket-protocol"]);
        return false;}
    else{
        return true;}
}

