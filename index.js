import { createRequire } from 'module';
const require = createRequire(import.meta.url);

const { Modbus } = require (`./lib/binding/${process.platform}-${process.arch}/modbusJS.node`);
export default Modbus;
