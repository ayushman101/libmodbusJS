#include <iostream>
#include <napi.h>
#include <modbus/modbus.h>
#include "modbus.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
Napi::FunctionReference Modbus::constructor;

Modbus::Modbus (const Napi::CallbackInfo& info) : Napi::ObjectWrap<Modbus>  (info) {

	Napi::Env env = info.Env ();

	if ( info.Length () < 3 )  {

		Napi::Error::New (env, "Not Enough Arguments").ThrowAsJavaScriptException ();
		return;
	}


	if ( !info[0].IsString())  {

		Napi::TypeError::New (env, "Expected a String").ThrowAsJavaScriptException ();
		return;
	}


	std::string mode = info[0].As<Napi::String> ().Utf8Value ();

	if (mode == "tcp") {

		if (!info[1].IsString ()) {
			Napi::TypeError::New (env, "Expected a String").ThrowAsJavaScriptException ();
			return;
		}

		if (!info[2].IsNumber ()) {
			Napi::TypeError::New (env, "Expected a Number").ThrowAsJavaScriptException ();
			return;
		}

		std::string ip 	= info [1].As <Napi::String> ().Utf8Value ();
		int port 	= info [2].As <Napi::Number> ().Int32Value ();

		ctx = modbus_new_tcp ( ip.c_str() , port);
	}else if (mode == "rtu") {

		if (info.Length () < 6)  // TODO:Argument Count subject to change. May give default values in future
		{
			Napi::Error::New (env, "Not Enough Arguments").ThrowAsJavaScriptException ();
			return;
		}

		if (!info[1].IsString ()  || !info[2].IsNumber () || !info[3].IsString () || !info[4].IsNumber () || !info [5].IsNumber ()) {
			Napi::TypeError::New (env, "Invalid argument type").ThrowAsJavaScriptException ();
			return;
		}

		std::string device 	= info[1].As<Napi::String> ().Utf8Value ();
		int baud 		= info[2].As<Napi::Number> ().Int32Value ();
		std::string parity 	= info[3].As<Napi::String> ().Utf8Value ();
		int data_bit		= info[4].As<Napi::Number> ().Int32Value ();
		int stop_bit 		= info[5].As<Napi::Number> ().Int32Value ();

		ctx = modbus_new_rtu (device.c_str(), baud, parity[0], data_bit, stop_bit);
	}else {
		Napi::Error::New (env, "Invalid modbus Mode | mode : tcp or rtu").ThrowAsJavaScriptException ();
		return;
	}

	if (ctx == NULL) {
		Napi::Error::New (env, "Modbus Context Allocation Failed").ThrowAsJavaScriptException ();
	}

}

Napi::Value Modbus::connect (const Napi::CallbackInfo& info) {

	Napi::Env env = info.Env ();
	if (this->ctx == NULL) {
		Napi::Error::New (env, "Modbus Context not Allocated").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}

	if (modbus_connect (this->ctx) == -1) {
		fprintf (stderr, "Modbus Connect Failed %s\n", modbus_strerror (errno) );
		free ();
		Napi::Error::New (env, std::string (strerror (errno))).ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}
	return Napi::Number::New (env, 0);
}

void Modbus::close (const Napi::CallbackInfo& info) {

	Napi::Env env = info.Env ();
	if (this->ctx == NULL) {
		Napi::Error::New (env, "Modbus Context not Allocated").ThrowAsJavaScriptException ();
		return;
	}
	modbus_close (this->ctx);
	free ();
}

void Modbus::free () {
	modbus_free (this->ctx);
	this->ctx = NULL;
}

Napi::Value Modbus::setSlave (const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env ();
	if (info.Length () < 1) {
		Napi::Error::New (env, "Modbus Set Slave: Not Enough arguments").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}

	if (!info[0].IsNumber ()) {
		Napi::TypeError::New (env, "Modbus Set Slave: Expected a number").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}

	int id = info[0].As<Napi::Number> ().Int32Value ();

	if (id < 1 || id >247) {
		Napi::Error::New (env, "Invalid Slave Id | range (1-247)").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}

	if (modbus_set_slave (this-> ctx, id) == -1) {
		Napi::Error::New (env, std::string (strerror (errno))).ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}

	return Napi::Number::New (env, 0);
}

Napi::Value Modbus::getSlave (const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env ();
	if (this->ctx == NULL) {
		Napi::Error::New (env, "Modbus Context not Allocated").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}
	int rc = modbus_get_slave (this->ctx);
	if (rc == -1) {
		Napi::Error::New (env, std::string (strerror (errno))).ThrowAsJavaScriptException ();
	}

	return Napi::Number::New (env, rc);
}

Napi::Value Modbus::readRegisters (const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env ();

	if (this->ctx == NULL) {
		Napi::Error::New (env, "Modbus Context not Allocated").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);	
	}

	if (info.Length () < 2) {
		Napi::Error::New (env, "Not enough arguments").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}
	
		
	const int reg_addr = info[0].As <Napi::Number> ().Int32Value (), nb = info[1].As <Napi::Number> ().Int32Value ();
	uint16_t tab_reg[nb];

	int rc = modbus_read_registers (this->ctx, reg_addr, nb, tab_reg);
	if (rc == -1) {
		Napi::Error::New (env, std::string (strerror (errno))).ThrowAsJavaScriptException ();
		return Napi::Number::New (env, rc);
	}

	Napi::Array arr = Napi::Array::New(env, nb);	

	for (int i = 0;i < nb;i++) {
		arr.Set ((uint32_t)i, Napi::Number::New (env, tab_reg[i]));
	}
	
	return arr;
}

Napi::Value Modbus::writeRegisters (const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env ();

	if (this->ctx == NULL) {
		Napi::Error::New (env, "Modbus Context not Allocated").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);	
	}

	if (info.Length () < 3) {
		Napi::Error::New (env, "Not enough arguments").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}

	if (!info[2].IsArray ()) {
		Napi::Error::New (env, "Expected an array").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}	
		
	const int reg_addr = info[0].As <Napi::Number> ().Int32Value (), nb = info[1].As <Napi::Number> ().Int32Value ();
	Napi::Array inpArr = info[2].As <Napi::Array>  ();
	size_t length 	   = inpArr.Length ();
	uint16_t arr [length];

	for (size_t i=0;i<length;i++) {
		
		Napi::Value val = inpArr.Get (i);	

		if (!val.IsNumber()) {
			Napi::TypeError::New(env, "Array elements must be numbers").ThrowAsJavaScriptException();
			return Napi::Number::New (env, -1);
		}

		arr[i] = static_cast<uint16_t>(val.As<Napi::Number>().Uint32Value());
	}
	
	int rc = modbus_write_registers (this->ctx, reg_addr, nb, arr);
	if (rc == -1) {
		Napi::Error::New (env, std::string (strerror (errno))).ThrowAsJavaScriptException ();
	}

	return Napi::Number::New (env, rc);
}

Napi::Value Modbus::writeRegister (const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env ();

	if (this->ctx == NULL) {
		Napi::Error::New (env, "Modbus Context not Allocated").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);	
	}

	if (info.Length () < 2) {
		Napi::Error::New (env, "Not enough arguments").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);
	}
	
	const int reg_addr  = info[0].As <Napi::Number> ().Int32Value ();
	const uint16_t 	val = info[1].As <Napi::Number> ().Int32Value ();

	int rc = modbus_write_register (this->ctx, reg_addr, val);

	if (rc == -1) {
		Napi::Error::New (env, std::string (strerror (errno))).ThrowAsJavaScriptException ();
	}
	return Napi::Number::New (env, rc);
}

Napi::Object Modbus::Init (Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass (env, "Modbus", {
			InstanceMethod ("connect", &Modbus::connect),
			InstanceMethod ("close", &Modbus::close),
			InstanceMethod ("setSlave", &Modbus::setSlave),
			InstanceMethod ("getSlave", &Modbus::getSlave),
			InstanceMethod ("readRegisters", &Modbus::readRegisters),
			InstanceMethod ("writeRegister", &Modbus::writeRegister),
			InstanceMethod ("writeRegisters", &Modbus::writeRegisters)
			});

	constructor = Napi::Persistent (func);
	constructor.SuppressDestruct ();
	exports.Set ("Modbus", func);
	return exports;
}
