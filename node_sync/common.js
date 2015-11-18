var METADATA_TYPE = {
    FS_REGISTER: 0,
    FS_REGISTER_OK: 1,
    FS_REGISTER_FAIL: 2,
    UPLOAD_REQUEST: 3,
    UPLOAD_READY: 4
};

var FS_MAX_PORT = 51000;
var FS_MIN_PORT = 50000;

var optionalRtpDataChannels = {
    optional: [
    {DtlsSrtpKeyAgreement: true},
    {RtpDataChannels: false}
    ]
}

var iceServers = {
    "iceServers": [{
        'url': 'stun:stun.services.mozilla.com'    }]
};

var mediaConstraints = {
    optional: [],
    mandatory: {
        OfferToReceiveAudio: false,
        OfferToReceiveVideo: false
    }
};

function usleep(microseconds) {
    var start = new Date().getTime();
    while (new Date().getTime() < (start + microseconds/1000));
}

exports.METADATA_TYPE = METADATA_TYPE;
exports.FS_MIN_PORT = FS_MIN_PORT;
exports.FS_MAX_PORT = FS_MAX_PORT;
exports.optionalRtpDataChannels = optionalRtpDataChannels;
exports.iceServers = iceServers;
exports.mediaConstraints = mediaConstraints;
exports.usleep = usleep;
