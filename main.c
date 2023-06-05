#include <stdio.h>
#include <stdlib.h>
#include "webcam.h"
// tire fotos com a  webcam e  camarea
int main()
{
    UINT cm_idx, cm_cnt;
	int width, height, frame_rate;
	;
	cm_idx = 0;
cm_cnt =WebCamDriver();
system("pause");
	// 测试使用0号
	cm_idx = 0;

	WebCamOpen(cm_idx);
Webmp(cm_idx,L"spy.bmp");
WebCamStart(cm_idx,L"Cam.avi");
getchar();
WebCamClose(cm_idx);
WebCamClose(cm_idx);
system("pause");
    return 0;
}
