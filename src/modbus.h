#ifndef MODBUSJS_H
#define MODBUSJS_H

#include <napi.h>
#include <modbus/modbus.h>

class Modbus : public Napi::ObjectWrap<Modbus> {

	public:
		static Napi::Object Init ( Napi::Env env, Napi::Object exports );
		Modbus ( const Napi::CallbackInfo& info );
	private:
		static Napi::FunctionReference constructor;
		Napi::Value connect (const Napi::CallbackInfo& info);
		void close (const Napi::CallbackInfo& info);
		modbus_t *ctx;
};

#endif
