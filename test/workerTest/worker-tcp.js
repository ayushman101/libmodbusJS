import { parentPort, workerData } from 'node:worker_threads';
// import { Modbus } from '#build/Release/modbusJS.node';

import { createRequire } from 'module';
const require = createRequire(import.meta.url);

// Use require for the native addon
const { Modbus } = require ('../../build/Release/modbusJS');

class Worker {
	constructor () {
        const modbus = new Modbus ("tcp","127.0.0.1",8502);
		modbus.connect();
		console.log (modbus.connect());
		console.log ('Modbus connection success');
		console.log (modbus.setSlave (2));
		console.log ('Modubs slave ID: ', modbus.getSlave ());
		console.log ('writeRegisters: ', modbus.writeRegisters (2, 2, [22, 512]));
		console.log ('readRegisters: ', modbus.readRegisters (1, 5));

		parentPort.postMessage ({
			type : 'construct',
			msg : 'worker thread created',
		});
	}
}

new Worker ();
