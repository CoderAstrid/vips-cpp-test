#include "stdafx.h"
#include "LargeImgProc.h"
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

	VImage img = VImage::new_from_file(m_szPathName, VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL_UNBUFFERED));

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