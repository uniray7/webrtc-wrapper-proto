var WebSocketClient = require("websocket").client;
var WebRTC = require("wrtc");
var RTCPeerConnection = WebRTC.RTCPeerConnection;
var RTCSessionDescription = WebRTC.RTCSessionDescription;
var RTCIceCandidate = WebRTC.RTCIceCandidate;

var FileReader = require('filereader');
var FileAPI = require('file-api')
      , File = FileAPI.File;
var fs = require('fs');

var common = require("../common.js")
var METADATA_TYPE = common.METADATA_TYPE;
const FS_MAX_PORT = common.FS_MAX_PORT;
const FS_MIN_PORT = common.FS_MIN_PORT;
const optionalRtpDataChannels = common.optionalRtpDataChannels;
const mediaConstraints = common.mediaConstraints;
const iceServers = common.iceServers;

function error_handler(error)
{
    throw error;  
}



function client_upload(file_path){
    // check the file is existed
    fs.stat(file_path, function(err, stats){if(err){throw err}});

    // generate offer
    console.log("CLIENT: upload");
    console.log("CLIENT: create peerconnection");
    var offerer = new RTCPeerConnection(iceServers, optionalRtpDataChannels);
    // TODO: channel label "upload" can be for verification in the future

    offerer.onicegatheringstatechange = onicegatheringstatechange;

    console.log("CLIENT: create datachannel");
    var offerer_dc = offerer.createDataChannel("upload");
    set_channel_events(offerer_dc);

    console.log("CLIENT: create offer");
    offerer.createOffer(set_local_description, error_handler);



    function set_local_description(description){
        console.log("CLIENT: set local description");
        offerer.setLocalDescription(new RTCSessionDescription(description),
                function(){
                    console.log("CLIENT: success set local description");
                },
                error_handler);
    }

    function onicegatheringstatechange(evt){
        console.log("CLIENT: ice gathering state: "+offerer.iceGatheringState);
        if (offerer.iceGatheringState == "complete")
        {
            send_upload_request(offerer.localDescription);
        }
    }

    function send_upload_request(localDescription){
        // send offer & UPLOAD_REQUEST via MS to FS
        var client = new WebSocketClient();
        client.connect("ws://192.168.0.103:8080", "sync-protocol");

        client.on("connect", 
                function(connection){
                    connection.on("message", 
                        function(msg){
                            // whenever recieve answer then close the connection;
                            console.log(msg.utf8Data);
                            offerer.setRemoteDescription(new RTCSessionDescription(JSON.parse(msg.utf8Data).answer))
                        });

                    var sent_data = {"metadata_type":METADATA_TYPE.UPLOAD_REQUEST,
                        "userID":"fakeRay0",
                        "offer": localDescription
                    };
                    console.log("CLIENT: send offer & UPLOAD_REQUEST to FS via MS");
                    connection.sendUTF(JSON.stringify(sent_data));
                });

        client.on("connectFailed", function(error){
            console.log("client connect failed");
        });
    }



    function set_channel_events(channel){
        channel.onopen = function(){
            console.log("CLIENT: channel on open");
            console.log("CLIENT: open file");
            var fileReader = new FileReader();
            var packet_index;
            fileReader.setNodeChunkedEncoding(true);
            fileReader.readAsText(new File(file_path));

            fileReader.on('error', function(err){console.log(err);throw err;});
            var start_time, end_time;
            fileReader.on('loadstart', function(){console.log("start");packet_index=0;start_time = new Date().getTime()});

            // non-standard alias of `addEventListener` listening to non-standard `data` event
            fileReader.on('data', function (data) {
                var slice_size = 8192;
                for (var i=0; i<((data.length)/slice_size); i++)
                {
                    var data_slice = data.slice(i*slice_size, (i+1)*slice_size);
                    //console.log("chunkSize:", data_slice.length);
                    var data_base64 = data_slice.toString("base64");
                    var packet = {"index" :packet_index, "length": data_slice.length, "type": "base64", "data": data_base64};
                    console.log("packet_index", packet.index);
                    channel.send(JSON.stringify(packet)); 
                    packet_index++;
                    common.usleep(500);
                    if(packet_index%1000 == 0)
                    {
                        common.usleep(300000) 
                    }
                    
                }
                
            });
        
        
            // `onloadend` as property
            fileReader.on('loadend', function () {
                end_time = new Date().getTime();
                console.log(end_time-start_time);
                console.log("Success");
            });
                        
        }
    }
} //function client_upload(file_path)

function protocolIsAllowed(request, fs_protocol) {
    // put logic here to detect whether the specified origin is allowed.
    if(request["httpRequest"]["headers"]["sec-websocket-protocol"] != fs_protocol)
    { console.log(request["httpRequest"]["headers"]["sec-websocket-protocol"]);
        return false;}
    else{
        return true;}
}


client_upload("../../videos/Sample.mp4");


