import { Worker } from 'node:worker_threads';

(async () => {
    const worker = new Worker ('./worker-rtu.js');
	
	const worker2 = new Worker ('./worker-rtu.js', {
		workerData: {
			port : '/dev/ttyACM2',
		}
	});

    worker.on ('message', (msg)=> {
	console.log (msg);
    })

	worker.postMessage ({
		type: 'read',
		slaveId : 65,
		reg: 1,
	});
		
	worker2.postMessage ({
		type: 'read',
		slaveId : 2,
		reg: 10,
	});
		
}) ();
