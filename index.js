const { Modbus } = require ('./build/Release/modbusJS');

try {
	const modbus = new Modbus ("tcp","127.0.0.1",8502);
	console.log (modbus.connect());
	modbus.close ();
} catch (error) {
	console.error (error);
}
