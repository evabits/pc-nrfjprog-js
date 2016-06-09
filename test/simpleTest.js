'use strict';

const nrfjprog = require('../index.js');
let probe = new nrfjprog.DebugProbe();
probe.getSerialNumbers((err, serialNumbers) => {
    console.log(JSON.stringify(serialNumbers));
    console.log('I was called back!');

    console.log(nrfjprog.NRF51_FAMILY);
    console.log(nrfjprog.NRF52_FAMILY);

    probe.program(serialNumbers[0].serialNumber, { 0: 'connectivity_115k2_with_s130_2.0.1.hex', 1: 'connectivity_115k2_with_s132_2.0.1.hex'}, err => {
        console.log(err);
        console.log('Done programming');
    });

    probe.getVersion(serialNumbers[0].serialNumber, (err, version) => { console.log('Error: ' + err); console.log('Version: ' + version);});
});
