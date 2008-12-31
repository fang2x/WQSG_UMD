/*  WQSG_UMD
*  Copyright (C) WQSG <wqsg@qq.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
// InputBox.cpp : 实现文件
//

#include "stdafx.h"
#include "WQSG_UMD_APP.h"
#include "InputBox.h"


// CInputBox 对话框

IMPLEMENT_DYNAMIC(CInputBox, CDialog)

CInputBox::CInputBox( u32 old , u32 max , CWnd* pParent /*=NULL*/)
	: CDialog(CInputBox::IDD, pParent)
	, m_十进制(_T("0"))
	, m_十六进制(_T("0"))
	, m_val(old)
	, m_maxval( max )
{

}

CInputBox::~CInputBox()
{
}

void CInputBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_十进制);
	DDV_MaxChars(pDX, m_十进制, 10);
	DDX_Text(pDX, IDC_EDIT2, m_十六进制);
	DDV_MaxChars(pDX, m_十六进制, 8);
	DDX_Control(pDX, IDC_EDIT1, m_c10);
	DDX_Control(pDX, IDC_EDIT2, m_c16);
}


BEGIN_MESSAGE_MAP(CInputBox, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CInputBox::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CInputBox::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CInputBox 消息处理程序

void CInputBox::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	s64 val = _ttoi64( m_十进制.GetBuffer() );

	bool 改变 = true;
	if( val < 0 )
		val = 0;
	else if( val > m_maxval )
		val = m_maxval;
	else
		改变 = false;

	m_val = (u32)val;

	if( 改变 )
		m_十进制.Format( L"%d" , m_val );
	m_十六进制.Format( L"%X" , m_val );

	UpdateData(FALSE);
}

void CInputBox::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	int start , end;
	m_c16.GetSel( start , end );
	m_十六进制.MakeUpper();

	for( int i = 0 ; (i>=0) && m_十六进制[i] ; ++i )
	{
		const WCHAR ch = m_十六进制[i];
		if( ( ch < L'0' || ch > L'9' ) && ( ch < L'A' || ch > L'F' ) )
		{
			m_十六进制.Delete( i );
			if( start > i-- )
				--start;
		}
	}

	s64 val = 0;
	swscanf( m_十六进制.GetBuffer() , L"%X" , &val );

	bool 改变 = true;
	if( val < 0 )
		val = 0;
	else if( val > m_maxval )
		val = m_maxval;
	else
		改变 = false;

	m_val = (u32)val;

	m_十进制.Format( L"%d" , m_val );
	if( 改变 )
		m_十六进制.Format( L"%X" , m_val );

	UpdateData(FALSE);
	m_c16.SetSel( start , start );
}

BOOL CInputBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if( m_maxval < 0 )
		m_maxval = 0;

	if( m_val < 0 )
		m_val = 0;
	else if( m_val > m_maxval )
		m_val = m_maxval;

	m_十进制.Format( L"%d" , m_val );
	m_十六进制.Format( L"%X" , m_val );

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

s32 CInputBox::GetVal(void)
{
	return s32(m_val);
}
