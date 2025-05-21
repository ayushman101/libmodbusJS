import { Worker } from 'node:worker_threads';
import { fileURLToPath } from 'node:url';
import path from 'node:path';

const __filename= fileURLToPath (import.meta.url);
const __dirname = path.dirname (__filename);

(async () => {
    const worker = new Worker (path.resolve (__dirname, './worker-tcp.js'));
	//
	// const worker2 = new Worker ('./worker-rtu.js', {
	// 	workerData: {
	// 		port : '/dev/ttyACM2',
	// 	}
	// });

    worker.on ('message', (msg)=> {
	console.log (msg);
    })

	worker.postMessage ({
		type: 'read',
		slaveId : 65,
		reg: 1,
	});
		
	// worker2.postMessage ({
	// 	type: 'read',
	// 	slaveId : 2,
	// 	reg: 10,
	// });
		
}) ();
