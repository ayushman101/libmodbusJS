#include <napi.h>
#include <modbus/modbus.h>
#include "modbus.h"

Napi::FunctionReference Modbus::constructor;

Modbus::Modbus (const Napi::CallbackInfo& info) : Napi::ObjectWrap<Modbus>  (info) {

	Napi::Env env = info.Env ();

		if ( info.Length () < 3 ) 
			 Napi::Error::New (env, "Not Enough Arguments").ThrowAsJavaScriptException ();


		if ( !info[0].IsString()) 
			 Napi::TypeError::New (env, "Expected a String").ThrowAsJavaScriptException ();


		std::string mode = info[0].As<Napi::String> ().Utf8Value ();

		if (mode == "tcp") {

			if (!info[1].IsString ()) 
				 Napi::TypeError::New (env, "Expected a String").ThrowAsJavaScriptException ();

			if (!info[2].IsNumber ())
				 Napi::TypeError::New (env, "Expected a Number").ThrowAsJavaScriptException ();

			std::string ip 	= info [1].As <Napi::String> ().Utf8Value ();
			int port 	= info [2].As <Napi::Number> ().Int32Value ();

			ctx = modbus_new_tcp ( ip.c_str() , port);
		}

		else if (mode == "rtu") {

			if (info.Length () < 6)  // TODO:Argument Count subject to change. May give default values in future
				 Napi::Error::New (env, "Not Enough Arguments").ThrowAsJavaScriptException ();
		}

		if (ctx == NULL) {
			Napi::Error::New (env, "Modbus connect fail").ThrowAsJavaScriptException ();	
		}

}

Napi::Object Modbus::Init (Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass (env, "Modbus", {
			});

	constructor = Napi::Persistent (func);
	constructor.SuppressDestruct ();
	exports.Set ("Modbus", func);
	return exports;
}
