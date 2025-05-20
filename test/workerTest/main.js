import { Worker } from 'node:worker_threads';

(async () => {
    const worker = new Worker ('./worker.js');

    worker.on ('message', (msg)=> {
	console.log (msg);
    })
}) ();
