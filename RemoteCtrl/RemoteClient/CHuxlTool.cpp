#include "pch.h"
#include "CHuxlTool.h"

void CHuxlTool::dump(BYTE* pData, size_t nSize) {
	std::string strOut;
	for (size_t i = 0; i < nSize; i++)
	{
		char buf[16] = "";
		if (i > 0 && (i % 16 == 0))
		{
			strOut += "\n";
		}
		snprintf(buf, sizeof(buf), "%02X ", pData[i] & 0xFF);
		strOut += buf;
	}
	strOut += "\n";
	OutputDebugStringA(strOut.c_str());
}

int CHuxlTool::BytesToImage(CImage& image, const std::string& strBuffer)
{
	BYTE* pData = (BYTE*)strBuffer.c_str();
	//存入CImage
	//创建一块内存句柄，用于目标流
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, 0);
	if (!hMem)
	{
		TRACE("内存不足了!");
		Sleep(1);
		return -1;
	}
	IStream* pStream = NULL;
	HRESULT hRet = CreateStreamOnHGlobal(hMem, TRUE, &pStream);
	if (hRet == S_OK) {
		ULONG length = 0;
		pStream->Write(pData, strBuffer.size(), &length);
		LARGE_INTEGER bg{ 0 };
		pStream->Seek(bg, STREAM_SEEK_SET, NULL);
		if ((HBITMAP)image != NULL)image.Destroy();
		image.Load(pStream);
		return hRet;
	}
}

