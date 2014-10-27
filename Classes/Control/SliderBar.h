#ifndef __shaderTest__SliderBar__
#define __shaderTest__SliderBar__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
class SliderBar:public Node{
public:
	/* �������� ��������������Դ ������Դ ������С ��������С���� */
	static SliderBar * createSliderBar(const char * sliderFileName,const char * backgroundFileName,CCSize bgSize,float multiple);
	/* ��ʼ������ */
	bool init(const char * sliderFileName,const char * backgroundFileName,CCSize bgSize,float multiple);
	SliderBar(); //���캯��
	~SliderBar(); //��������
	/* ���ù������������䱳��������λ�õİٷֱ� ����[0,1] �����������Χʱ�������� */
	void setValue(float value);
public:
	/* ������Ա������get��set���� */
	CC_SYNTHESIZE(Scale9Sprite *, m_sliderSprite, SliderSprite);
	CC_SYNTHESIZE(Scale9Sprite *, m_backgroundSprite, BackgroundSprite);
	/* �����������ʼλ�� */
	CCPoint _beginPos;
	/* ��������������λ�� */
	CCPoint _endPos;
	/* ���������������С */
	CCSize _bgSize;
	/* �����������С */
	CCSize _sdSize;
};
#endif /* defined(__shaderTest__SliderBar__) */