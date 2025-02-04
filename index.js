const { Modbus } = require ('./build/Release/modbusJS');

try {
	const modbus = new Modbus ("rtu","/dev/ttyUSB0",9600, 'N',8,1);
	console.log (modbus.connect());
} catch (error) {
	console.error (error);
}
