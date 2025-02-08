const { Modbus } = require ('./build/Release/modbusJS');

try {
	const modbus = new Modbus ("tcp","127.0.0.1",8502);
	console.log (modbus.connect());
	console.log (modbus.setSlave (1));
	console.log ('Modubs slave ID: ', modbus.getSlave ());
	console.log ('writeRegister: ', modbus.writeRegister (1,0));
	console.log ('readRegister: ', modbus.readRegisters (1,1));
	modbus.close ();
} catch (error) {
	console.error (error);
}
