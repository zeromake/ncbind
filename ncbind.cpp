#if 0
#include <windows.h>
#endif
#include "tjsCommHead.h"
#include "Extension.h"
#include "ncbind.hpp"


#if 0
#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall

#ifdef _MSC_VER
#pragma comment(linker, "/EXPORT:V2Link=_V2Link@4")
#pragma comment(linker, "/EXPORT:V2Unlink=_V2Unlink@0")
#endif

//--------------------------------------
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

//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;

EXPORT(HRESULT) V2Link(iTVPFunctionExporter *exporter)
{
	// �X�^�u�̏�����(�K���L�q����)
	TVPInitImportStub(exporter);

	NCB_LOG_W("V2Link");

	// AutoRegister�œo�^���ꂽ�N���X����o�^����
	ncbAutoRegister::AllRegist();

	// ���̎��_�ł� TVPPluginGlobalRefCount �̒l��
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// �Ƃ��čT���Ă����BTVPPluginGlobalRefCount �͂��̃v���O�C������
	// �Ǘ�����Ă��� tTJSDispatch �h���I�u�W�F�N�g�̎Q�ƃJ�E���^�̑��v�ŁA
	// ������ɂ͂���Ɠ������A����������Ȃ��Ȃ��ĂȂ��ƂȂ�Ȃ��B
	// �����Ȃ��ĂȂ���΁A�ǂ����ʂ̂Ƃ���Ŋ֐��Ȃǂ��Q�Ƃ���Ă��āA
	// �v���O�C���͉���ł��Ȃ��ƌ������ƂɂȂ�B

	return S_OK;
}
//---------------------------------------------------------------------------
EXPORT(HRESULT) V2Unlink()
{
	// �g���g��������A�v���O�C����������悤�Ƃ���Ƃ��ɌĂ΂��֐�

	// �������炩�̏����Ńv���O�C��������ł��Ȃ��ꍇ��
	// ���̎��_�� E_FAIL ��Ԃ��悤�ɂ���B
	// �����ł́ATVPPluginGlobalRefCount �� GlobalRefCountAtInit ����
	// �傫���Ȃ��Ă���Ύ��s�Ƃ������Ƃɂ���B
	if (TVPPluginGlobalRefCount > GlobalRefCountAtInit) {
		NCB_LOG_W("V2Unlink ...failed");
		return E_FAIL;
		// E_FAIL ���A��ƁAPlugins.unlink ���\�b�h�͋U��Ԃ�
	} else {
		NCB_LOG_W("V2Unlink");
	}
	/*
		�������A�N���X�̏ꍇ�A�����Ɂu�I�u�W�F�N�g���g�p���ł���v�Ƃ������Ƃ�
		�m�邷�ׂ�����܂���B��{�I�ɂ́APlugins.unlink �ɂ��v���O�C���̉����
		�댯�ł���ƍl���Ă������� (�������� Plugins.link �Ń����N������A�Ō��
		�Ńv���O�C������������A�v���O�����I���Ɠ����Ɏ����I�ɉ��������̂��g)�B
	*/

	// AutoRegister�œo�^���ꂽ�N���X�����폜����
	ncbAutoRegister::AllUnregist();

	// �X�^�u�̎g�p�I��(�K���L�q����)
	TVPUninitImportStub();

	return S_OK;
}
#endif

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


//---------------------------------------------------------------------------
// static�ϐ��̎���

// auto register �擪�|�C���^
ncbAutoRegister::ThisClassT const*
ncbAutoRegister::_top[ncbAutoRegister::LINE_COUNT] = NCB_INNER_AUTOREGISTER_LINES_INSTANCE;

