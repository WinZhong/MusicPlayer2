﻿// ListCtrlEx.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PlayListCtrl.h"


// CPlayListCtrl

IMPLEMENT_DYNAMIC(CPlayListCtrl, CListCtrlEx)

//通过构造函数参数传递列表中所有文件的信息的引用
CPlayListCtrl::CPlayListCtrl(const vector<SongInfo>& all_song_info) :m_all_song_info{ all_song_info }
{
	m_toolTip.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);
}

CPlayListCtrl::~CPlayListCtrl()
{
}


wstring CPlayListCtrl::GetDisplayStr(const SongInfo & song_info, DisplayFormat display_format)
{
	AudioType type{ CAudioCommon::GetAudioTypeByExtension(song_info.file_name) };
	if (type == AU_MIDI)		//MIDI只显示文件名
	{
		display_format = DF_FILE_NAME;
	}
	switch (display_format)
	{
	case DF_FILE_NAME:		//显示为文件名
		return song_info.file_name;
	case DF_TITLE:			//显示为歌曲标题
		if (song_info.title == DEFAULT_TITLE)	//如果获取不到歌曲标题，就显示文件名
			return song_info.file_name;
		else
			return song_info.title;
	case DF_ARTIST_TITLE:	//显示为艺术家 - 标题
		if (song_info.title == DEFAULT_TITLE && song_info.artist == DEFAULT_ARTIST)		//如果标题和艺术家都获取不到，就显示文件名
			return song_info.file_name;
		else
			return (song_info.artist + _T(" - ") + song_info.title);
	case DF_TITLE_ARTIST:	//显示为标题 - 艺术家
		if (song_info.title == DEFAULT_TITLE && song_info.artist == DEFAULT_ARTIST)		//如果标题和艺术家都获取不到，就显示文件名
			return song_info.file_name;
		else
			return (song_info.title + _T(" - ") + song_info.artist);
	default:
		return song_info.file_name;
	}
}

void CPlayListCtrl::ShowPlaylist(DisplayFormat display_format, bool search_result)
{
	m_searched = search_result;
	if (m_all_song_info.size() == 1 && m_all_song_info[0].file_name.empty())
	{
		DeleteAllItems();
		return;
	}
	if (!search_result)		//显示所有曲目
	{
		int item_num_before = GetItemCount();
		int item_num_after = theApp.m_player.GetSongNum();
		//如果当前列表中项目的数量小于原来的，则直接清空原来列表中所有的项目，重新添加
		if (item_num_after < item_num_before)
		{
			DeleteAllItems();
			item_num_before = 0;
		}
		CString str;
		for (int i{}; i < item_num_after; i++)
		{
			str.Format(_T("%u"), i + 1);
			if (i >= item_num_before)	//如果当前列表中的项目数量大于之前的数量，则需要在不够时插入新的项目
			{
				InsertItem(i, str);
			}
			SetItemText(i, 0, str);
			SetItemText(i, 1, (GetDisplayStr(m_all_song_info[i], display_format)).c_str());
			SetItemText(i, 2, theApp.m_player.GetAllSongLength(i).time2str().c_str());
		}
	}
	else		//只显示搜索结果的曲目
	{
		if (m_search_result.empty())
		{
			DeleteAllItems();
			InsertItem(0, _T(""));
			SetItemText(0, 1, _T("没有结果可以显示。"));
			return;
		}
		int item_num_before = GetItemCount();
		int item_num_after = m_search_result.size();
		//如果当前列表中项目的数量小于原来的，则直接清空原来列表中所有的项目，重新添加
		if (item_num_after < item_num_before)
		{
			DeleteAllItems();
			item_num_before = 0;
		}
		CString str;
		for (int i{}; i < item_num_after; i++)
		{
			str.Format(_T("%u"), m_search_result[i] + 1);
			if (i >= item_num_before)	//如果当前列表中的项目数量大于之前的数量，则需要在不够时插入新的项目
			{
				InsertItem(i, str);
			}
			SetItemText(i, 0, str);
			SetItemText(i, 1, (GetDisplayStr(m_all_song_info[m_search_result[i]], display_format)).c_str());
			SetItemText(i, 2, theApp.m_player.GetAllSongLength(m_search_result[i]).time2str().c_str());
		}
	}
}

void CPlayListCtrl::QuickSearch(const wstring & key_word)
{
	m_search_result.clear();
	for (size_t i{}; i < m_all_song_info.size(); i++)
	{
		if (CCommon::StringFindNoCase(m_all_song_info[i].file_name, key_word) != wstring::npos
			|| CCommon::StringFindNoCase(m_all_song_info[i].title, key_word) != wstring::npos
			|| CCommon::StringFindNoCase(m_all_song_info[i].artist, key_word) != wstring::npos
			|| CCommon::StringFindNoCase(m_all_song_info[i].album, key_word) != wstring::npos)
			m_search_result.push_back(i);
	}
}

void CPlayListCtrl::GetItemSelectedSearched(vector<int>& item_selected)
{
	item_selected.clear();
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			CString str;
			str = GetItemText(nItem, 0);
			item_selected.push_back(_ttoi(str) - 1);
		}
	}
}


BEGIN_MESSAGE_MAP(CPlayListCtrl, CListCtrlEx)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, CPlayListCtrl::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
END_MESSAGE_MAP()


// CPlayListCtrl 消息处理程序

void CPlayListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//如果开启文本提示
	if (m_bEnableTips)
	{
		CString str_tip;
		LVHITTESTINFO lvhti;

		// 判断鼠标当前所在的位置(行, 列)
		lvhti.pt = point;
		SubItemHitTest(&lvhti);

		//如果鼠标移动到另一行, 则进行处理; 否则, 不做处理
		if (lvhti.iItem != m_nItem)
		{
			// 保存当前鼠标所在的行
			m_nItem = lvhti.iItem;

			// 如果鼠标移动到一个合法的行，则显示新的提示信息，否则不显示提示
			if (m_nItem >= 0 && m_nItem < m_all_song_info.size())
			{
				CString dis_str = GetItemText(m_nItem, 1);
				int strWidth = GetStringWidth(dis_str) + theApp.DPI(10);		//获取要显示当前字符串的最小宽度
				int columnWidth = GetColumnWidth(1);	//获取鼠标指向列的宽度
				if (columnWidth < strWidth)		//当单元格内的的字符无法显示完全时在提示的第1行显示单元格内文本
				{
					str_tip += dis_str;
					str_tip += _T("\r\n");
				}
				int song_index;
				if (!m_searched)
				{
					song_index = m_nItem;
				}
				else
				{
					CString str = GetItemText(m_nItem, 0);
					song_index = _ttoi(str) - 1;
				}
				if (song_index < 0 || song_index >= m_all_song_info.size())
					return;
				str_tip += _T("文件名：");
				str_tip += m_all_song_info[song_index].file_name.c_str();
				str_tip += _T("\r\n");

				str_tip += _T("标题：");
				str_tip += m_all_song_info[song_index].title.c_str();
				str_tip += _T("\r\n");

				str_tip += _T("艺术家：");
				str_tip += m_all_song_info[song_index].artist.c_str();
				str_tip += _T("\r\n");

				str_tip += _T("唱片集：");
				str_tip += m_all_song_info[song_index].album.c_str();
				str_tip += _T("\r\n");

				CString str_bitrate;
				str_bitrate.Format(_T("比特率：%dkbps"), m_all_song_info[song_index].bitrate);
				str_tip += str_bitrate;

				m_toolTip.SetMaxTipWidth(theApp.DPI(400));		//设置提示信息的宽度，以支持提示换行

				m_toolTip.AddTool(this, str_tip);
				m_toolTip.Pop();			// 显示提示框
			}
			else
			{
				m_toolTip.AddTool(this, _T(""));
				m_toolTip.Pop();
			}
		}
	}
	CListCtrlEx::OnMouseMove(nFlags, point);
}


BOOL CPlayListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_toolTip.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE)
	{
		m_toolTip.RelayEvent(pMsg);
	}

	return CListCtrlEx::PreTranslateMessage(pMsg);
}


void CPlayListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//CWindowDC dc(this);
	//HDC hDC = dc.GetSafeHdc();
	//m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);

	//将提示信息设为置顶
	m_toolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	//初始化播放列表
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int width0, width1, width2;
	width0 = theApp.DPI(40);
	width2 = theApp.DPI(50);
	CRect rect;
	GetWindowRect(rect);
	width1 = rect.Width() - width0 - width2 - theApp.DPI(21);
	InsertColumn(0, _T("序号"), LVCFMT_LEFT, width0);		//插入第1列
	InsertColumn(1, _T("曲目"), LVCFMT_LEFT, width1);		//插入第2列
	InsertColumn(2, _T("长度"), LVCFMT_LEFT, width2);		//插入第3列
	EnableTip();

	CListCtrlEx::PreSubclassWindow();
}


void CPlayListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW lplvdr = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW& nmcd = lplvdr->nmcd;
	static bool this_item_select = false;
	switch (lplvdr->nmcd.dwDrawStage)	//判断状态   
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:			//如果为画ITEM之前就要进行颜色的改变
		if (IsWindowEnabled())
		{
			this_item_select = false;
			if (m_searched && m_search_result.size() == 0)		//如果播放列表处于搜索状态且没有搜索结果
			{
				if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)	//不允许选中行
					SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
				lplvdr->clrText = GRAY(140);
				lplvdr->clrTextBk = GRAY(255);
			}
			else
			{
				int hightlight_item;
				if (!m_searched || m_search_result.size() == m_all_song_info.size())	//当播放列表不处理搜索状态，或搜索结果数量等于播放列表中曲目数量时
				{
					hightlight_item = m_highlight_item;
				}
				else		//如果播放列表处于搜索状态，则高亮项目应该为搜索结果的索引
				{
					auto iter = std::find(m_search_result.begin(), m_search_result.end(), m_highlight_item);
					if (iter == m_search_result.end())
						hightlight_item = -1;
					else
						hightlight_item = iter - m_search_result.begin();
				}
				//当选中行又是高亮行时设置颜色
				if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED && nmcd.dwItemSpec == hightlight_item)
				{
					this_item_select = true;
					SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
					lplvdr->clrText = m_theme_color.light3;
					lplvdr->clrTextBk = m_theme_color.dark1;
				}
				//设置选中行的颜色
				else if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)
				{
					this_item_select = true;
					SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
					lplvdr->clrText = m_theme_color.dark3;
					lplvdr->clrTextBk = m_theme_color.light2;
				}
				//设置高亮行的颜色
				else if (nmcd.dwItemSpec == hightlight_item)
				{
					lplvdr->clrText = m_theme_color.dark2;
					//lplvdr->clrText = 0;
					lplvdr->clrTextBk = m_theme_color.light3;
				}
				//设置偶数行的颜色
				else if (nmcd.dwItemSpec % 2 == 0)
				{
					lplvdr->clrText = CColorConvert::m_gray_color.dark3;
					lplvdr->clrTextBk = CColorConvert::m_gray_color.light3;
				}
				//设置奇数行的颜色
				else
				{
					lplvdr->clrText = CColorConvert::m_gray_color.dark3;
					lplvdr->clrTextBk = CColorConvert::m_gray_color.light4;
				}
			}
		}
		else		//当控件被禁用时，显示文本设为灰色
		{
			lplvdr->clrText = GRAY(140);
			lplvdr->clrTextBk = GRAY(240);
		}
		*pResult = CDRF_NOTIFYPOSTPAINT;
		break;
	case CDDS_ITEMPOSTPAINT:
		if (this_item_select)
			SetItemState(nmcd.dwItemSpec, 0xFF, LVIS_SELECTED);
		//*pResult = CDRF_DODEFAULT;
		break;
	}
}
