//��������ڶ������ڻ�ͼ�ĺ���
#pragma once
class CDrawCommon
{
public:

	//������DrawScrollText��������ʱʹ�õ�һЩ��Ҫ�ں���������Ϻ�������ڵı���
	struct ScrollInfo
	{
		int shift_cnt{};		//�ƶ��Ĵ���
		bool shift_dir{};		//�ƶ��ķ�������Ϊfalse������Ϊtrue
		int freez{};			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
		bool dir_changed{ false };	//����������˱仯����Ϊtrue
	};

	CDrawCommon();
	~CDrawCommon();

	void Create(CDC* pDC, CWnd* pMainWnd);
	void SetBackColor(COLORREF back_color);		//���û����ı�ʱ���ı�����ɫ
	void SetFont(CFont* pfont);		//���û����ı�������
	void SetDC(CDC* pDC);		//���û�ͼ��DC

	//�����ǻ����ı��ĺ��������ܺ�CStaticEx���е�ͬ��������ͬ����ͬ��������Щ�������������ض��Ŀؼ�
	void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, bool center, bool no_clip_area = false);	//��ָ���ľ��������ڻ�������ɫ���ı������no_clip_areaΪtrue�������������ʱ���ƻ�ͼ����
	void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color1, COLORREF color2, int split, bool center, bool no_clip_area = false);	//��ָ���ľ��������ڻ��Ʒָ���ɫ���ı���splitΪ��ɫ�ָ��λ�ã�ȡֵΪ0~1000�����ڸ�ʶ�̬��ʾ�������no_clip_areaΪtrue�������������ʱ���ƻ�ͼ����
	void DrawScrollText(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);	//�ڿؼ��ϻ��ƹ������ı��������Ȳ���ʱ����pixelָ���˺�������һ���ƶ�������ֵ�����resetΪtrue�����������ʼλ��
	void DrawScrollText2(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);	//�������ܺ�DrawScrollTextһ����ֻ���������ֻ������ҹ����������������

	static void SetDrawArea(CDC* pDC, CRect rect);

private:
	CDC* m_pDC{};		//���ڻ�ͼ��CDC���ָ��
	CWnd* m_pMainWnd{};	//��ͼ���ڵľ��
	COLORREF m_backColor{ RGB(255,255,255) };
	CFont* m_pfont{};
};
