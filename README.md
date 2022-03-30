# Open-the-door-project
2015년 여름방학 프로젝트 코드
- 학부 3학년 여름방학 때 인턴쉽으로 온 IIT 학생과 같이 수행한 로봇팔로 문열기 프로젝트
- DARPA 로봇 첼린지에서 휴머노이드가 문을 여는 미션에서 착안하여 수행
- 모바일로봇과 로봇팔을 융합하여 자동으로 문고리를 인식하여 문을 찾고, 문을 여는 동작을 수행함


**Identification of Door Argorithm**
  - 문을 인식하는 영상처리 SW
  - OpenCV 2.4.1, Kinect 센서 사용
  - Positive image와 Negative image로 SVM을 학습시켜 자동으로 문고리를 인식하게 함
  - 개발환경 : Visual Studio 2013
  - [학습 데이터 다운로드](https://kaistackr-my.sharepoint.com/:u:/g/personal/jsha2913_kaist_ac_kr/EWbagk84yodHplg_iS_HHXsBo0adB9FvtASBhGKlp_d01A?e=pZLxpO)

   

**matlab simulation**
- 문고리를 열고 모바일로봇으로 나가는 모션을 수행하기 위한 로봇팔 궤적 동작 시뮬레이션 수행
- Matlab, [robotics toolbox](https://petercorke.com/toolboxes/robotics-toolbox/) 9.10 사용
- [동영상](https://youtu.be/eSiBU9Iu5fA) 


**control robot arm**
- matlab으로 시뮬레이션한 궤적을 C++ 코드로 변경. 실제 로봇팔을 이용한 동작 확인
- shunk 로봇 팔 제어, CAN 통신, Visual Studio 2010 사용
