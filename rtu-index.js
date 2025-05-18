const { Modbus } = require ('./build/Release/modbusJS');

(async () => {
    try {
	const modbus = new Modbus ("rtu","/dev/ttyS0", 9600, 'N', 8, 1);
	console.log (modbus.connect());
	console.log ('Modbus connection success');
	console.log (modbus.setSlave (2));
	console.log ('Modubs slave ID: ', modbus.getSlave ());
	console.log ('writeRegister: ', modbus.writeRegister (0, 10));
	// console.log ('writeRegisters: ', modbus.writeRegisters (2, 3, [12, 15, 23]));
	// console.log ('readRegisters: ', modbus.readRegisters (1, 5));
	modbus.close ();
    } catch (error) {
	console.error (error);
    }
}) ();
