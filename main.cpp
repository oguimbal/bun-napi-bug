/* cppsrc/main.cpp */
#include <napi.h>
#include <iostream>


// this is a reproduction of https://github.com/Level/classic-level/blob/e70d4c23b11cfe35f59a285297ed7f6c1b602b28/binding.cc#L175-L180
// see original issue https://github.com/oven-sh/bun/issues/6784
int GetEncoding(napi_env env, napi_value options, const char *option)
{
    napi_value value;
    size_t copied;
    char buf[2];

    bool getprop_ok = napi_get_named_property(env, options, option, &value) == napi_ok;
    bool getvalue_str = getprop_ok && napi_get_value_string_utf8(env, value, buf, 2, &copied) == napi_ok;

    std::cout << "Copied: " << copied << std::endl;
    std::cout << "Value str: " << buf << std::endl;

    if (getprop_ok &&
        getvalue_str && copied == 1)
    {
        // Value is either "buffer", "utf8" or "view" so we only have to read the first char
        switch (buf[0])
        {
        case 'b':
            return 1;
        case 'v':
            return 2;
        }
    }

    return 3;
}


napi_value GetValueEncoding(const Napi::CallbackInfo &info)
{
    napi_value options = info[0];
    Napi::Env env = info.Env();

    const int encoding = GetEncoding(env, options, "valueEncoding");

    napi_value num;
    napi_status status = napi_create_int32(env, encoding, &num);
    return num;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        "getValueEncoding", Napi::Function::New(env, GetValueEncoding));
    return exports;
}

NODE_API_MODULE(bufferBug, InitAll)