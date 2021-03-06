#pragma once
#include "afxcmn.h"
#include "AudioCommon.h"
#include "afxwin.h"
#include "ListCtrlEx.h"
#include "Common.h"


// CFindDlg 对话框

class CFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(const vector<SongInfo>& playlist, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindDlg();

	//wstring m_config_path;

	int GetSelectedTrack() const;
	bool GetFindCurrentPlaylist() const;
	wstring GetSelectedSongPath() const;
	void SaveConfig();
	void LoadConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND_DIALOG };
#endif

protected:

	const vector<SongInfo>& m_playlist;		//播放列表
	vector<int> m_find_result;			//储存当前播放列表的查找结果（曲目序号）
	wstring m_key_word;				//查找的字符串
	vector<wstring> m_all_find_result;		//储存所有播放列表的查找结果（文件绝对路径）

	CListCtrlEx m_find_result_list;		//查找结果控件
	int m_item_selected{ -1 };		//鼠标选中的项目序号
	CMenu m_menu;

	//int m_dpi;

	CButton m_find_file_check;
	CButton m_find_title_check;
	CButton m_find_artist_check;
	CButton m_find_album_check;

	bool m_find_current_playlist{ true };		//如果查找范围为当前播放列表，则为true，如果是所有播放列表，则为false
	bool m_find_file{ true };
	bool m_find_title{ true };
	bool m_find_artist{ true };
	bool m_find_album{ true };

	int m_find_option_data{};		//保存查找选项的数据，用每一个bit位表示每个查找选项是否选中

	int m_min_width{};
	int m_min_height{};

	CString m_selected_string;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void ShowFindResult();
	void ShowFindInfo();
public: 
	void ClearFindResult();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeFindEdit();
	afx_msg void OnNMClickFindList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFindButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkFindList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedFindFileCheck();
	afx_msg void OnBnClickedFindTitleCheck();
	afx_msg void OnBnClickedFindArtistCheck();
	afx_msg void OnBnClickedFindAlbumCheck();
	afx_msg void OnBnClickedFindCurrentPlaylistRadio();
	afx_msg void OnBnClickedFindAllPlaylistRadio();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFdPlay();
	afx_msg void OnFdOpenFileLocation();
	afx_msg void OnNMRClickFindList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFdCopyText();
};
