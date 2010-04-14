#include "stdafx.h"
#include "excel.h"

struct excel::impl
{
	std::vector< std::vector< _string > >	strTable;
	std::map< _string, int >				ColumnMap;
};

excel::excel(void)
: m_impl( new excel::impl )
{
	m_nColMax	=	0;
	m_nLineMax	=	0;

	m_nCurCol		=	0;
	m_bReadString	=	FALSE;
	m_nCP			=	0;

	_tcscpy( m_szUnit, _T("") );
}

excel::~excel(void)
{
	SAFE_DELETE(m_impl); 
}

bool excel::ReadExcelFile( _lpctstr strFileName, int nTitleLine )
{
	FILE* fp;
	if( ( fp = _tfopen( strFileName, _T("r") ) ) == NULL )
	{
		return FALSE;
	}
	_tchar buffer[1024];
	int nLine = 0;
	while( !feof( fp ) )
	{
		buffer[0] = 0;
		_fgetts( buffer, sizeof( buffer ), fp );
		if( !AnalyzeLine( buffer, sizeof( buffer ) ) )
		{
			DestoryTable();
			return false;
		}

		if( nTitleLine == nLine )
		{
			--m_nLineMax;
			std::vector< _string >& line = m_impl->strTable[m_nLineMax];
			for( size_t n = 0; n < line.size(); ++n )
			{
				_string &idx = line[n];
				if( !idx.empty() )
				{
					m_impl->ColumnMap[idx] = n;
				}
			}
			// m_impl->strTable.erase( m_impl->strTable.begin() + m_nLineMax );
		}
		++nLine;
	}
	fclose( fp );
	return TRUE;
}

bool excel::AnalyzeLine( _lpctstr pstrLine, int nSize )
{
	if( !pstrLine || nSize == 0)	return false;
	int i = 0;

	int n = 0;
	do
	{
		ASSERT( i < nSize );
		if( i > nSize )
			return false;

		ASSERT( n < sizeof( m_szUnit ) );
		if( n >= sizeof( m_szUnit ) )	
			return false;

		switch( pstrLine[i] )
		{
			case 0:
			case _T('\t'):
			case _T('\n'):
			{
				if( !m_bReadString && i > 0 )
				{
					m_szUnit[m_nCP] = 0;
					m_nCP = 0;
					AddElement( m_nLineMax, m_nCurCol++, _string( m_szUnit ) );
				}
				else
				{
					m_szUnit[m_nCP++] = pstrLine[i];
				}
			}
			break;
			case _T('"'):
			{
				m_bReadString = !m_bReadString;
			}
			break;
			default:
			{
				m_szUnit[m_nCP++] = pstrLine[i];
			}
		}
	}while( pstrLine[i++] != 0 );
	// 该行结束的时候，如果不是在读取字符串状态，开始新的一条记录。
	if( !m_bReadString && m_nCurCol != 0 )
	{
		m_nLineMax++;
		m_nCurCol = 0;
	}
	return true;
}

bool excel::AddElement( int nLine, int nCol, _string& Value )
{
	if( nLine > m_nLineMax )	m_nLineMax = nLine;
	if( nCol > m_nColMax )	m_nColMax = nCol;

	int nLineSizeMax = ( int )m_impl->strTable.size();
	if( nLine >= nLineSizeMax )	m_impl->strTable.resize( nLineSizeMax + 100, std::vector< _string >( 64, _T("") ) );

	int nColSizeMax = ( int ) m_impl->strTable[nLine].size();
	if( nCol >= nColSizeMax )	m_impl->strTable[nLine].resize( nColSizeMax + 32, _T("") );

	try
	{
		m_impl->strTable[nLine][nCol] = Value;
	}
	catch( ... )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	return TRUE;
}

bool excel::GetElement( int nLine, int nCol, _lpctstr* pstrValue )
{
	_string const* pstring = NULL;
	if( !GetElement( nLine, nCol, &pstring ) )	return FALSE;

	*pstrValue = pstring->c_str();
	return TRUE;
}

bool excel::GetElement( int nLine, int nCol, const _string **pstrValue )
{
	if( m_impl->strTable.size() <= ( size_t )nLine )
		return false;

	if( m_impl->strTable[nLine].size() <= ( size_t )nCol )	
		return false;

	*pstrValue = &m_impl->strTable[nLine][nCol];
	if( (*pstrValue)->empty() )
		return false;
	return true;
}

bool excel::GetElement( int nLine, _lpctstr lpszTitle, const _string **pstrValue )
{
	if( lpszTitle == NULL )
		return false;

	std::map< _string, int >::iterator idx = m_impl->ColumnMap.find( lpszTitle );
	if( idx != m_impl->ColumnMap.end() )
	{
		return GetElement( nLine, idx->second, pstrValue );
	}

	return false;
}

bool excel::GetElement( int nLine, _lpctstr lpszTitle, _lpctstr *pstrValue )
{
	if( lpszTitle == NULL )
		return false;

	std::map< _string, int >::iterator idx = m_impl->ColumnMap.find( lpszTitle );
	if( idx != m_impl->ColumnMap.end() )
	{
		return GetElement( nLine, idx->second, pstrValue );
	}

	return false;
}

float excel::GetElement( int nLine, _lpctstr lpszTitle, float *pFloatVal, float fDefault )
{
	_lpctstr pValue = NULL;
	float fValue;
	if( GetElement( nLine, lpszTitle, &pValue ) )
	{
		_stscanf( pValue, _T("%f"), &fValue );
		if( pFloatVal )
			*pFloatVal = fValue;
	}
	else if( pFloatVal )
	{
		fValue = *pFloatVal = fDefault;
	}
	return fValue;
}

int excel::GetElement( int nLine, _lpctstr lpszTitle, int *pIntVal, int nDefault )
{
	_lpctstr pValue = NULL;
	int nValue;
	if( GetElement( nLine, lpszTitle, &pValue ) )
	{
		_stscanf( pValue, _T("%d"), &nValue );
		if( pIntVal )
			*pIntVal = nValue;
	}
	else if( pIntVal )
	{
		nValue = *pIntVal = nDefault;
	}
	return nValue;
}

int excel::GetLineElementCount( int nLine )
{	
	return ( int )m_impl->strTable[nLine].size(); 
}

void excel::DestoryTable()
{
	m_impl->strTable.clear();
	m_nLineMax = 0;
	m_nColMax = 0;
}

