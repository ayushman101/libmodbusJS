const { Modbus } = require ('./build/Release/modbusJS');

try {
	const modbus = new Modbus ("tcp","127.0.0.1",8502);
} catch (error) {
	console.error (error);
}
