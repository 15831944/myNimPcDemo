#ifndef NIM_P2P_DEVELOP_KIT_H
#define NIM_P2P_DEVELOP_KIT_H

#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h> // on Linux or macOS
#endif

// SDK header include
#include "include/nim_p2p_def.h"
#include "include/nim_p2p_sdk_def.h"
#include "include/nim_p2p_sdk_file_def.h"
#include "include/nim_p2p_sdk_proc_prog_def.h"

namespace nim_p2p {

static const std::wstring kSDKP2PDll_d = L"nim_p2p_sdk_d.dll";
static const std::wstring kSDKP2PDll = L"nim_p2p_sdk.dll";

#ifdef _WIN32
typedef HMODULE DYLIB_HINSTANCE;
#else
typedef void* DYLIB_HINSTANCE;
#endif

typedef struct
{
	std::string kFilePath;
	std::string kFileName;
	std::string kFileMD5;
	std::string kFileUserExt;
	__int64		kFileSize;
} FILE_INFO, *PFILE_INFO;

class NimP2PDvelopKit
{

public:
	NimP2PDvelopKit();
	~NimP2PDvelopKit();

	/**
	 * ����������Ϊ����
	 */
	static NimP2PDvelopKit* GetInstance()
	{
		static NimP2PDvelopKit nim_p2p_develop_kit;
		return &nim_p2p_develop_kit;
	}
	NimP2PDvelopKit(const NimP2PDvelopKit&) = delete;
	NimP2PDvelopKit& operator=(const NimP2PDvelopKit&) = delete;

	/**
	 * ��ʼ�� p2p ��
	 * @param[in] dll_path ���ļ�·��
	 * @param[in] trans_file_request_callback ���յ��ļ�����Ļص�
	 * @param[in] channel ��������ͨ�������ϲ㶨��
	 * @param[in] json_ext json ��չ�ֶΣ���ʱû��ʹ��
	 * @result �����Ƿ��ʼ���ɹ�
	 */
	bool Init(const std::wstring& dll_path,
		OnTransferFileRequest trans_file_request_callback,
		TransferFileSessionStateChangeCallback trans_file_state_change_callback,
		TransferFileProgressCallback trans_file_progress_callback,
		SendCommandChannel send_command_channel, const std::string& json_ext = "");

	/**
	 * �ͷ� p2p ��̬��
	 */
	void UnInit();

	/**
	 * ע��һ�����䷽��
	 * @param[in] info ���ϲ㶨��Ĵ��䷽���ṹ
	 */
	void RegTransferFileProcProg(const TransferFileProcProgRegInfo& info);

	/**
	 * ������һ���ļ�
	 * @param[in] remote_flag Զ�˱�־��IM Ϊ accid
	 * @param[in] file_info Ҫ���͵��ļ���Ϣ�ṹ
	 */
	TransferFileSessionID TransferFile(const RemoteFlagType remote_flag, const FILE_INFO& file_info);

	/**
	* ���ͷ�ȡ�������ļ�
	* @param[in] session_id �ļ�����ĻỰID
	*/
	bool CancelTransferFile(const TransferFileSessionID& session_id);

	/**
	 * ��ʼ����һ���ļ�
	 * @param[in] session_id �� Init �����г�ʼ���� OnTransferFileRequest �ص����յ����ļ�������Ϣ��Я���� TransferFileSessionID
	 * @param[in] file_path Ҫ������ļ�·��
	 * @result ���ؽ�������ִ���Ƿ�ɹ�
	 */
	bool ReceiveFile(const TransferFileSessionID& session_id, const std::string& file_path);

	/**
	* �ܾ������ļ�
	* @param[in] session_id �ļ�����ĻỰID
	*/
	bool RejectReceiveFile(const TransferFileSessionID& session_id);


	/**
	* ���շ�ȡ�������ļ�
	* @param[in] session_id �ļ�����ĻỰID
	*/
	bool CancelReceiveFile(const TransferFileSessionID& session_id);

public:
	/**
	 * ���յ� IM ��Ϣ���ݸ� SDK
	 * @param[in] remote_flag Զ����Ϣ
	 * @param[in] command Ҫ���ݸ� SDK ������
	 */
	void OnReceiveChannelCommand(const RemoteFlagType& remote_flag, const std::string& command);

private:
    template <typename F>
    F Function(const char* function_name)
    {
#ifdef _WIN32
        F f = (F) ::GetProcAddress((HMODULE)instance_p2p_, function_name);
#else
        F f = (F) dlsym(instance_p2p_, function_name);
#endif
        return f;
    }

private:
	DYLIB_HINSTANCE		instance_p2p_			= nullptr;
	OnChannelCommand	receive_command_channel	= nullptr;
};

}

#endif // NIM_P2P_DEVELOP_KIT_H
