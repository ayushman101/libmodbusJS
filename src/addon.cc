#include <napi.h>
#include "modbus.h"

Napi::Object Init (Napi::Env env, Napi::Object exports) {
	return Modbus::Init (env,exports);
}

NODE_API_MODULE(myaddon,Init);
