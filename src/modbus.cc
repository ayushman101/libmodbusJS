#include <napi.h>
#include <modbus/modbus.h>
#include "modbus.h"
#include <errno.h>
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
		Napi::Error::New (env, "Invalid modbus Mode | mode : tcp or rtur").ThrowAsJavaScriptException ();
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
		modbus_free (this->ctx);
		Napi::Error::New (env, "Modbus Connect Failed").ThrowAsJavaScriptException ();
		return Napi::Number::New (env, -1);	
	}
	return Napi::Number::New (env, 0);
}

Napi::Object Modbus::Init (Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass (env, "Modbus", {
			InstanceMethod ("connect", &Modbus::connect)
			});

	constructor = Napi::Persistent (func);
	constructor.SuppressDestruct ();
	exports.Set ("Modbus", func);
	return exports;
}
