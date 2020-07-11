#pragma once
class CLargeImgProc
{
public:
	CLargeImgProc();
	~CLargeImgProc();

	void SetPathName(LPCTSTR lpszName);

	const int Width() const { return m_Width; }
	const int Height() const { return m_Height; }
	CString GetInfo() const;
private:
	char	m_szPathName[256];
	int		m_Width;
	int		m_Height;
};

