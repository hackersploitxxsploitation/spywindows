#include "webcam.h"

#ifdef __cplusplus
extern "C" {
#endif

 HWND m_hCapWnd[MAX_CAMERA_INDEX];
 CAPDRIVERCAPS m_CapDrvCap[MAX_CAMERA_INDEX];
 CAPSTATUS m_CapStatus[MAX_CAMERA_INDEX];
 CAPTUREPARMS m_Parms[MAX_CAMERA_INDEX];

UINT WebCamDriver()
{
	UINT wIndex, wCameraCnt;
	WCHAR szDriverName[80];
	WCHAR szDriverVersion[80];
	wCameraCnt = 0;
	for (wIndex = 0; wIndex < MAX_CAMERA_INDEX; wIndex++)
	{
		if( TRUE == capGetDriverDescription (wIndex,
					szDriverName, sizeof(szDriverName),
					szDriverVersion, sizeof(szDriverVersion)) )
		{
			wprintf(L"[%d] %s - %s\n", wIndex, szDriverName, szDriverVersion);
			wCameraCnt++;
		}
	}
	return wCameraCnt;
}

 BOOL WebCamOpenInternal(UINT idx)
{
	// �������ھ��
	m_hCapWnd[idx] = capCreateCaptureWindow(NULL, 0, 0, 0, 200, 200, NULL, 0);
	if (!m_hCapWnd)
	{
		printf("m_hCapWnd failed !!!\n");
		return FALSE;
	}
	// ���ӵ�idx��������
	else if (TRUE == capDriverConnect(m_hCapWnd[idx], idx))
	{
		capOverlay(m_hCapWnd[idx], FALSE);
		capPreview(m_hCapWnd[idx], FALSE);
		// �õ�������������
		memset(&m_CapDrvCap[idx], 0, sizeof(CAPDRIVERCAPS));
		if (FALSE == capDriverGetCaps(m_hCapWnd[idx], &m_CapDrvCap[idx], sizeof(CAPDRIVERCAPS)))
		{
			printf("capDriverGetCaps failed !!!\n");
		}
		else if (m_CapDrvCap[idx].fCaptureInitialized)
		{
			// ��ʼ���ɹ�
			return TRUE;
		}
		else
		{
			printf("fCaptureInitialized failed !!!\n");
		}
	}
	else
	{
		printf("connect failed !!!\n");
	}
	capCaptureStop(m_hCapWnd[idx]); // ֹͣ��׽
	capDriverDisconnect(m_hCapWnd[idx]);
	DestroyWindow(m_hCapWnd[idx]);
	return FALSE;
}

void WebCamClose(UINT idx)
{
	capCaptureStop(m_hCapWnd[idx]);
	capCaptureAbort(m_hCapWnd[idx]); // ֹͣ����
	capDriverDisconnect(m_hCapWnd[idx]); // �Ͽ���׽����������������
	DestroyWindow(m_hCapWnd[idx]);
}

BOOL WebCamOpen(UINT idx)
{
	BOOL ret = TRUE;
	int n = 0;
	do
	{
		printf("%d try to open %d\n", n, idx);
		Sleep(500);
		ret = WebCamOpenInternal(idx);
		n++;
	} while(FALSE == ret && n < 5);
	if (FALSE == ret)
	{
		printf("open failed !!!\n");
	}
	return ret;
}

BOOL WebCamlparam(UINT idx, int *width, int *height, int *frame_rate)
{
	BOOL ret = TRUE;
	// �õ����ò���
	ret = capCaptureGetSetup(m_hCapWnd[idx], &m_Parms[idx], sizeof(CAPTUREPARMS));
	if (FALSE == ret)
	{
		printf("capCaptureGetSetup failed !!!\n");
		return ret;
	}
	// �õ�������״̬
	ret = capGetStatus(m_hCapWnd[idx], &m_CapStatus[idx], sizeof(CAPSTATUS));
	if (FALSE == ret)
	{
		printf("capGetStatus failed !!!\n");
		return ret;
	}
	*width = (int)m_CapStatus[idx].uiImageHeight;
	*height = (int)m_CapStatus[idx].uiImageWidth;
	*frame_rate = (int)1000000 / (int)m_Parms[idx].dwRequestMicroSecPerFrame;

	return ret;
}

BOOL Webmp(UINT idx, WCHAR *file)
{
	BOOL ret = TRUE;
	// �ػ�ǰͼ��
	ret = capGrabFrame(m_hCapWnd[idx]);
	if (FALSE == ret)
	{
		printf("capGrabFrame failed !!!\n");
		return ret;
	}
	// ���浽�ļ�
	ret = capFileSaveDIB(m_hCapWnd[idx], file);
	if (FALSE == ret)
	{
		printf("capFileSaveDIB failed !!!\n");
		return ret;
	}
	return ret;
}

BOOL WebCamclipboard(UINT idx)
{
	BOOL ret = TRUE;
	// �ػ�ǰͼ��
	ret = capGrabFrame(m_hCapWnd[idx]);
	if (FALSE == ret)
	{
		printf("capGrabFrame failed !!!\n");
		return ret;
	}
	// ���浽������
	ret = capEditCopy(m_hCapWnd[idx]);
	if (FALSE == ret)
	{
		printf("capEditCopy failed !!!\n");
		return ret;
	}
	return ret;
}

//LRESULT CALLBACK StreamCallbackProc(HWND hCapWnd, LPVIDEOHDR lphdr)
//{
//	return (LRESULT)TRUE;
//}

BOOL  WebCamStart(UINT idx, WCHAR *file)
{
	BOOL ret = TRUE;
	// ��Ƶ���ص�����
	//ret = capSetCallbackOnVideoStream(m_hCapWnd[idx], StreamCallbackProc);
	//if (FALSE == ret)
	//{
	//	printf("capSetCallbackOnVideoStream failed !!!\n");
	//	return ret;
	//}
	// ���ò������
	capCaptureGetSetup(m_hCapWnd[idx], &m_Parms[idx], sizeof(CAPTUREPARMS));
	m_Parms[idx].fYield = TRUE; // ��̨����
	m_Parms[idx].fCaptureAudio = FALSE; // ������
	m_Parms[idx].wTimeLimit = FALSE; // ��ʱ������
	capCaptureSetSetup(m_hCapWnd[idx], &m_Parms[idx], sizeof(CAPTUREPARMS));
	// ���ò�����Ƶ���ļ�
	capFileSetCaptureFile(m_hCapWnd[idx], file);
	// ��ʼ¼���߳�
	ret = capCaptureSequence(m_hCapWnd[idx]);
	if (FALSE == ret)
	{
		printf("capCaptureSequence failed !!!\n");
		return ret;
	}
	return ret;
}

void  WebCamStop(UINT idx)
{
	//capSetCallbackOnVideoStream(m_hCapWnd[idx], NULL);
	capCaptureStop(m_hCapWnd[idx]);
	capCaptureAbort(m_hCapWnd[idx]);
}

#ifdef __cplusplus
}
#endif
