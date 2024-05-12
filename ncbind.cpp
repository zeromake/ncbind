#if 0
#include <windows.h>
#endif
#ifdef TVP_COMPILING_KRKRSDL2
#include "tjsCommHead.h"
#include "Extension.h"
#endif
#ifndef TVP_COMPILING_KRKRSDL2
#include "tp_stub.h"
#endif
#include "ncbind.hpp"
#include <set>
#include <map>


#ifndef TVP_COMPILING_KRKRSDL2
#ifdef _WIN32
#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall
#else
#define EXPORT(hr) extern "C" __attribute__((visibility ("default"))) hr
#endif
#endif

#ifndef TVP_COMPILING_KRKRSDL2
#ifdef _WIN32
#ifdef _MSC_VER
#pragma comment(linker, "/EXPORT:V2Link=_V2Link@4")
#pragma comment(linker, "/EXPORT:V2Unlink=_V2Unlink@0")
#endif
#endif

//--------------------------------------
#ifdef _WIN32
HINSTANCE gDllInstance = NULL;

//--------------------------------------
extern "C"
BOOL WINAPI
DllMain(HINSTANCE hinst, DWORD reason, LPVOID /*lpReserved*/)
{
  if (reason == DLL_PROCESS_ATTACH)
    gDllInstance = hinst;

  return 1;
}
#endif

//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;

EXPORT(HRESULT) V2Link(iTVPFunctionExporter *exporter)
{
	// スタブの初期化(必ず記述する)
	TVPInitImportStub(exporter);

	NCB_LOG_W("V2Link");

	// AutoRegisterで登録されたクラス等を登録する
	ncbAutoRegister::AllRegist();

	// この時点での TVPPluginGlobalRefCount の値を
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// として控えておく。TVPPluginGlobalRefCount はこのプラグイン内で
	// 管理されている tTJSDispatch 派生オブジェクトの参照カウンタの総計で、
	// 解放時にはこれと同じか、これよりも少なくなってないとならない。
	// そうなってなければ、どこか別のところで関数などが参照されていて、
	// プラグインは解放できないと言うことになる。

	return S_OK;
}
//---------------------------------------------------------------------------
EXPORT(HRESULT) V2Unlink()
{
	// 吉里吉里側から、プラグインを解放しようとするときに呼ばれる関数

	// もし何らかの条件でプラグインを解放できない場合は
	// この時点で E_FAIL を返すようにする。
	// ここでは、TVPPluginGlobalRefCount が GlobalRefCountAtInit よりも
	// 大きくなっていれば失敗ということにする。
	if (TVPPluginGlobalRefCount > GlobalRefCountAtInit) {
		NCB_LOG_W("V2Unlink ...failed");
		return E_FAIL;
		// E_FAIL が帰ると、Plugins.unlink メソッドは偽を返す
	} else {
		NCB_LOG_W("V2Unlink");
	}
	/*
		ただし、クラスの場合、厳密に「オブジェクトが使用中である」ということを
		知るすべがありません。基本的には、Plugins.unlink によるプラグインの解放は
		危険であると考えてください (いったん Plugins.link でリンクしたら、最後ま
		でプラグインを解放せず、プログラム終了と同時に自動的に解放させるのが吉)。
	*/

	// AutoRegisterで登録されたクラス等を削除する
	ncbAutoRegister::AllUnregist();

	// スタブの使用終了(必ず記述する)
	TVPUninitImportStub();

	return S_OK;
}
#endif

#ifdef TVP_COMPILING_KRKRSDL2
//---------------------------------------------------------------------------
// tTJSNC_KirikiriSDL2Internal : Kirikiri SDL2 internal class
//---------------------------------------------------------------------------
class tTJSNC_KirikiriSDL2Internal : public tTJSNativeClass
{
public:
	tTJSNC_KirikiriSDL2Internal();

	static tjs_uint32 ClassID;

protected:
	tTJSNativeInstance * CreateNativeInstance();
};

//---------------------------------------------------------------------------
// tTJSNC_KirikiriSDL2Internal
//---------------------------------------------------------------------------
tjs_uint32 tTJSNC_KirikiriSDL2Internal::ClassID = -1;
tTJSNC_KirikiriSDL2Internal::tTJSNC_KirikiriSDL2Internal() : tTJSNativeClass(TJS_W("KirikiriSDL2Internal"))
{
	TJS_BEGIN_NATIVE_MEMBERS(KirikiriSDL2Internal)
	TJS_DECL_EMPTY_FINALIZE_METHOD
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL_NO_INSTANCE(/*TJS class name*/KirikiriSDL2Internal)
{
	return TJS_S_OK;
}
TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/KirikiriSDL2Internal)
//----------------------------------------------------------------------

//----------------------------------------------------------------------
	TJS_END_NATIVE_MEMBERS

} // end of tTJSNC_Debug::tTJSNC_Debug
//---------------------------------------------------------------------------
tTJSNativeInstance *tTJSNC_KirikiriSDL2Internal::CreateNativeInstance()
{
	return NULL;
}
static iTJSDispatch2 * TVPCreateNativeClass_KirikiriSDL2Internal(iTJSDispatch2* global)
{
	ncbAutoRegister::AllRegist();
	iTJSDispatch2 *cls = new tTJSNC_KirikiriSDL2Internal();
	return cls;
}

static tTVPAtInstallClass TVPInstallClassKirikiriSDL2Internal(TJS_W("KirikiriSDL2Internal"), TVPCreateNativeClass_KirikiriSDL2Internal, TJS_W(""));
#endif


//---------------------------------------------------------------------------
// static変数の実体

// auto register 先頭ポインタ
ncbAutoRegister::ThisClassT const*
ncbAutoRegister::_top[ncbAutoRegister::LINE_COUNT] = NCB_INNER_AUTOREGISTER_LINES_INSTANCE;


std::set<ttstr> TVPRegisteredPlugins;
std::map<ttstr, ncbAutoRegister::INTERNAL_PLUGIN_LISTS > ncbAutoRegister::_internal_plugins;

bool ncbAutoRegister::LoadModule(const ttstr &_name)
{
	ttstr name = _name.AsLowerCase();
	if (TVPRegisteredPlugins.find(name) != TVPRegisteredPlugins.end())
		return false;
	auto it = _internal_plugins.find(name);
	if (it != _internal_plugins.end()) {
		for (int line = 0; line < ncbAutoRegister::LINE_COUNT; ++line) {
			const std::list<ncbAutoRegister const*> &plugin_list = it->second.lists[line];
			for (auto i = plugin_list.begin(); i != plugin_list.end(); ++i) {
				(*i)->Regist();
			}
		}
		TVPRegisteredPlugins.insert(name);
		return true;
	}
	return false;
}
