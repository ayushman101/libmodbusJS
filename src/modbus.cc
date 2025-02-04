#include <napi.h>
#include <modbus/modbus.h>
#include "modbus.h"

Napi::FunctionReference Modbus::constructor;

Modbus::Modbus (const Napi::CallbackInfo& info) : Napi::ObjectWrap<Modbus>  (info) {

	Napi::Env env = info.Env ();

	if (info.Length () < 1 || !info[0].IsNumber ()) {
		Napi::TypeError::New(env, "Expected a number").ThrowAsJavaScriptException();
	}	
	
	ctx = modbus_new_tcp ("127.0.0.1", info[0].As<Napi::Number>().Int32Value());

	if (ctx == NULL) {
		Napi::Error::New (env, "tcp connect fail").ThrowAsJavaScriptException ();	
	}
}

Napi::Value Modbus::Add(const Napi::CallbackInfo& info) {
	double num = info[0].As<Napi::Number>().DoubleValue();
	return Napi::Number::New(info.Env(), 1 + num);
}

Napi::Object Modbus::Init (Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass (env, "Modbus", {
		InstanceMethod ("add", &Modbus::Add),
	});

	constructor = Napi::Persistent (func);
	constructor.SuppressDestruct ();
	exports.Set ("Modbus", func);
	return exports;
}
