#pragma once
#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

#include <vector>
#define EXCEL_BUFFER_SIZE	512
class EXPORT_LIB excel
{
public:
	excel(void);
	~excel(void);
public:
	//	读取指定的表格文件
	//	strFileName	:	文件名
	bool ReadExcelFile( _lpctstr strFileName, int nTitleLine = 0 );

	//	分析读取的一行数据
	bool AnalyzeLine( _lpctstr pstrLine, int nSize );

	//	添加一个元素到指定位置。
	bool AddElement( int nLine, int nCol, _string& Value );

	bool GetElement( int nLine, int nCol, const _string **pstrValue );
	bool GetElement( int nLine, int nCol, _lpctstr *pstrValue );

	bool GetElement( int nLine, _lpctstr lpszTitle, const _string **pstrValue );
	bool GetElement( int nLine, _lpctstr lpszTitle, _lpctstr *pstrValue );

	float	GetElement( int nLine, _lpctstr lpszTitle, float *pFloatVal, float fDefault );
	int		GetElement( int nLine, _lpctstr lpszTitle, int *pIntVal, int nDefault );


	int	 GetLineElementCount( int nLine );
	void DestoryTable();

	int	 GetColMax(){	return m_nColMax; }
	int	 GetLineMax(){	return m_nLineMax; }
protected:
	struct impl;
	impl* m_impl;

	int		m_nColMax;
	int		m_nLineMax;

	int		m_nCurCol;
	bool	m_bReadString;
	_tchar	m_szUnit[EXCEL_BUFFER_SIZE];
	int		m_nCP;

};
