#ifndef USER_DATA_H_
#define USER_DATA_H_

#include "cocos2d.h"
#include "GameMacros.h"

USING_NS_CC;
class UserData : public cocos2d::Ref
{
public:
	UserData();
	~UserData();

	int getMissionIdx() {return _currentMission;}
	int getStepCount() {return _stepCount;}
	unsigned getTime() {return _time;}
	bool isDone() {return _isDone;}

	void setMissionIdx(int idx){_currentMission = idx;}
	void setStepCount(unsigned count){_stepCount = count;}
	void setTime(long dt) {_time = dt;}
	void setIsDone(bool is) {_isDone = is; }

private:
	int _currentMission;							//��ǰ�ؿ�����
	unsigned int _stepCount;						//����
	long _time;							//ʱ��
	HeroType _layout[ROWS][COLUMS];				//�佫����
	bool _isDone;								//�Ƿ�ͨ��
};



#endif // !USER_DATA_H_
