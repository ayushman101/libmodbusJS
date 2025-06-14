import { createRequire } from 'module';
const require = createRequire(import.meta.url);

// Use require for the native addon
const { Modbus } = require (`./lib/binding/${process.platform}-${process.arch}/libmodbusjs.node`);
export default Modbus;
