const { Modbus } = require ('./build/Release/modbusJS');

try {
	const modbus = new Modbus ("tcp","127.0.0.1",8502);
	console.log (modbus.connect());
	console.log (modbus.setSlave (2));
	console.log ('Modubs slave ID: ', modbus.getSlave ());
	console.log ('writeRegister: ', modbus.writeRegister (1, 10));
	console.log ('writeRegisters: ', modbus.writeRegisters (2, 3, [12, 15, 23]));
	console.log ('readRegisters: ', modbus.readRegisters (1, 5));
	modbus.close ();
} catch (error) {
	console.error (error);
}
