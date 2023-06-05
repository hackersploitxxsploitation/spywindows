#ifndef _CAMERACAPTURE_H_
#define _CAMERACAPTURE_H_
#include <windows.h>
#include <vfw.h>
#include <stdio.h>
#pragma comment (lib, "vfw32.lib")

#ifdef DLL_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CAMERA_INDEX 10

// ˵������ӡ���������б�
// ��������
// ���أ�������������
 UINT WebCamDriver();

// ˵�����������豸
// �������豸���
// ���أ�TRUE���򿪳ɹ���FALSE����ʧ��
 BOOL WebCamOpen(UINT idx);

// ˵�����ر������豸
// ������idx���豸���
// ���أ���
 void WebCamClose(UINT idx);

// ˵�����������豸��ȡһ��ͼƬ��BMP�ļ�
// ������idx���豸��ţ�file���ļ�������a.bmp��
// ���أ�TRUE����ȡ�ɹ���FALSE����ȡʧ��
 BOOL   Webmp(UINT idx, WCHAR *file);

// ˵�����������豸��ȡһ��ͼƬ��������
// ������idx���豸���
// ���أ�TRUE����ȡ�ɹ���FALSE����ȡʧ��
 BOOL WebCamclipboard(UINT idx);

// ˵
 BOOL WebCamStart(UINT idx, WCHAR *file);

// ˵����ֹͣ¼����Ƶ
// ������idx���豸���
// ���أ���
 void WebCamStop(UINT idx);

// ˵������������豸����
// ������idx���豸��ţ�width��ͼ����ȣ�height��ͼ��߶ȣ�frame_rate��֡��
// ���أ���
 BOOL WebCamlparam(UINT idx, int *width, int *height, int *frame_rate);

#ifdef __cplusplus
}
#endif

#endif
