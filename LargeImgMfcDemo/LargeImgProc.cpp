#include "stdafx.h"
#include "LargeImgProc.h"

#include "syscore.h"
#undef max
#undef min
#include <vips/vips8>
using namespace vips;

#ifdef _DEBUG
#pragma comment(lib, "libvips-cppd.lib")
#else
#pragma comment(lib, "libvips-cpp.lib")
#endif
#pragma comment(lib, "libvips.lib")
#pragma comment(lib, "libglib-2.0.lib")
#pragma comment(lib, "libgobject-2.0.lib")

CLargeImgProc::CLargeImgProc()
	: m_Width(0)
	, m_Height(0)
{
	memset(m_szPathName, 0, sizeof(m_szPathName));

	if (VIPS_INIT(NULL))
		vips_error_exit(NULL);
}


CLargeImgProc::~CLargeImgProc()
{
	vips_shutdown();
}

void CLargeImgProc::SetPathName(LPCTSTR lpszName)
{
	CString s(lpszName);
	CT2A ps(s);
	sprintf_s(m_szPathName, "%s", ps.m_psz);

	VImage img = VImage::new_from_file(m_szPathName, VImage::option()->set("access", VIPS_ACCESS_RANDOM));
	
	m_Width = img.width();
	m_Height = img.height();
}

CString CLargeImgProc::GetInfo() const
{
	CStringA sFileInfo;
	sFileInfo.Format(
		"Filename: %s\r\n"
		"Size: %d X %d\r\n"
		, m_szPathName
		, Width(), Height()
	);

	CString sRet(sFileInfo);

	return sRet;
}

BOOL CLargeImgProc::Convert(int _tw, int _th, BOOL _save)
{
	BOOL res = FALSE;

	VImage in = VImage::new_from_file(m_szPathName, VImage::option()->set("access", VIPS_ACCESS_RANDOM));
	out_log("width = %d, height = %d\n", in.width(), in.height());
	char * last_slash = strrchr(m_szPathName, '\\');
	char szFolder[256] = "";
	if (last_slash) {
		strcpy_s(szFolder, m_szPathName);
		szFolder[last_slash - m_szPathName+1] = 0;
	}
	int col = 0, row = 0;
	for (int y = 0; y < in.height(); y += _th, row++) {
		col = 0;
		for (int x = 0; x < in.width(); x += _tw, col++) {			
			int w = ymin(_tw, in.width() - 1 - x);
			int h = ymin(_th, in.height() - 1 - y);
			VImage crop = in.extract_area(x, y, w, h);
			if (_save) {
				char szname[256];
				sprintf_s(szname, "%s%03d-%03d.bmp", szFolder, col, row);
				crop.write_to_file(szname);
			}
		}
	}
	res = TRUE;

	return res;
}