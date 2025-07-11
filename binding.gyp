{
	"targets": [
	{
		"target_name": "libmodbusJS",
		"sources": [
			"src/addon.cc",
			"src/modbus.cc"
		],
		"libraries": ["<(module_root_dir)/deps/libmodbus.a"],
		"include_dirs": [
			"<!@(node -p \"require('node-addon-api').include\")",
			"/usr/include"
		],
		"dependencies": [
			"<!(node -p \"require('node-addon-api').gyp\")"
		],
		"defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
	}
    ]
}
