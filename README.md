# libmodbusjs

`libmodbusjs` is a high-performance Node.js library for Modbus communication built on top of the native [libmodbus](https://libmodbus.org/) C library. It is implemented as a Node.js C++ addon, providing low-level access to Modbus RTU and TCP interfaces with the efficiency of native code and the simplicity of JavaScript.

---

## 🚀 Why libmodbusjs?

- 🔧 **C++ Powered:** Built using a Node.js native addon, leveraging `libmodbus` under the hood.
- 🔄 **Thread-Compatible:** Designed to be thread-safe, enabling use in multi-threaded applications (e.g., worker threads in Node.js).
- 🐧🍎 **Cross-Platform:** Works on **Linux** and **macOS**.
- ❌ **No Windows Support:** Currently, the library **does not support Windows**.

---

## ⚠️ Compatibility Notes

This library uses precompiled binaries specific to your OS and Node.js version.

### ✅ Supported:
- Linux (x64, arm64)
- macOS (x64, arm64)

### ❌ Not Supported:
- Windows

---

### 🧩 Check Binary Compatibility

Before using, **please visit the [latest releases](https://github.com/ayushman101/libmodbusJS/releases)** and ensure there is a `.tar.gz` file matching your:

- OS (Linux/macOS)
- Architecture (x64/arm64)
- Node.js ABI version

If your version is **not available**, `npm install libmodbusjs` may attempt to rebuild the addon and fail if required tools (like a C++ compiler or `libmodbus`) are missing.

---

## 📦 Installation

```bash
npm install libmodbusjs
```

## 📘 API Reference

All methods are exposed through the `Modbus` class. You first instantiate a `Modbus` object, set the slave address, connect, then read/write as needed.

### 🛠️ Constructor

```
new Modbus(mode: 'rtu' | 'tcp', options: object)
```

- `mode`: Either `'rtu'` or `'tcp'`
- `options`:
  - For RTU:
    - `path`: Serial port path (e.g. `/dev/ttyUSB0`)
    - `baudRate`: Baud rate (e.g. `9600`)
  - For TCP:
    - `ip`: Device IP
    - `port`: Port (default: `502`)

---

### 🔌 `connect() → boolean`
Establishes the Modbus connection.
Returns `true` if successful, throws error otherwise.

---

### 🔚 `close()`
Closes the current Modbus connection and frees native memory.

---

### 🎯 `setSlave(id: number) → boolean`
Sets the slave address.

---

### 🔍 `getSlave() → number`
Returns the currently set slave address.

---

### 📖 `readRegisters(startAddr: number, count: number) → number[]`
Reads a sequence of holding registers.

- `startAddr`: Starting register address
- `count`: Number of registers to read
- Returns an array of 16-bit integers.

---

### ✍️ `writeRegister(addr: number, value: number) → boolean`
Writes a single value to a register.

---

### ✍️ `writeRegisters(startAddr: number, values: number[]) → boolean`
Writes multiple values starting from a specific address.

---

## 🔁 Example (RTU)

```js
import Modbus from 'libmodbusjs';

const modbus = new Modbus('rtu', { path: '/dev/ttyUSB0', baudRate: 9600 });
modbus.setSlave(1);
modbus.connect();

const values = modbus.readRegisters(0, 4);
console.log('Register values:', values);

modbus.writeRegister(0, 42);
modbus.close();
```

---

## 🌐 Example (TCP)

```js
import Modbus from 'libmodbusjs';

const modbus = new Modbus('tcp', { ip: '192.168.0.10', port: 502 });
modbus.setSlave(1);
modbus.connect();

const values = modbus.readRegisters(10, 2);
console.log(values);

modbus.writeRegisters(10, [100, 200]);
modbus.close();
```
