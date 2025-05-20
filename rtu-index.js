const { Modbus } = require ('./build/Release/modbusJS');

(async () => {
    try {
	const modbus = new Modbus ("rtu","/dev/ttyACM1", 19200, 'N', 8, 1);
	console.log (modbus.connect());
	console.log ('Modbus connection success');
	console.log (modbus.setSlave (65));
	console.log ('Modubs slave ID: ', modbus.getSlave ());
	// console.log ('writeRegister: ', modbus.writeRegister (1, 512));
	console.log ('writeRegisters: ', modbus.writeRegisters (67, 2, [22, 512]));
	console.log ('readRegisters: ', modbus.readRegisters (1, 5));
	modbus.close ();
    } catch (error) {
	console.error (error);
    }
}) ();
