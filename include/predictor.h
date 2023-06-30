//
// Created by 14908 on 2023/6/26.
//

#ifndef MAIN_CPP_PREDICTOR_H
#define MAIN_CPP_PREDICTOR_H
class Predictor{
public:
	int correct = 0;
	int wrong = 0;
	uint8_t pre[1024];
	Predictor(){
		for(int i = 0; i < 1024; ++i){
			pre[i] = 0;
		}
	}
	bool GetPredict(uint32_t line){
		int ind = (line >> 7) & 0b1111111111;
		return (pre[ind] >> 1) & 1;
	}
	void update(int ind, bool real){
		if(real){
			if((pre[ind] & 0b11) != 0b11)pre[ind] ++;
		}else{
			if((pre[ind] & 0b11) != 0b00)pre[ind] --;
		}
	}
};
#endif //MAIN_CPP_PREDICTOR_H
