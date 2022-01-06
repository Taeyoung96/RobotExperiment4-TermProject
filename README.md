# RobotExperiment4-TermProject 

2021 2학기 로봇학실험 4 프로젝트 코드입니다.  

팀원 : [김태영](https://github.com/Taeyoung96), [정현민](https://github.com/jeong-hyeonmin)  

|OS|Langauge|개발 환경|   
|:---:|:---:|:---:|   
|Window 10|C++(MFC), C(Atmega128)|Visual Studio 2017, Atmel Studio 7.0|   

## 프로젝트 설명  

One Arm Robot을 실제로 제작하고 위치, 속도, 전류 값을 제어하는 것을 목표로 합니다.  
ODE와 MFC를 활용하여 Simulation 환경을 구성하였고,  
Main PC와 Atmega128를 Serial 통신을 활용하여 Data를 주고받습니다.  
NTGraph를 활용하여 실제 Motor의 Data를 MFC상에서 시각화를 진행했습니다.  

제어기는 Cascade 구조로 설계하였으며  
위치 제어기는 PD 제어기, 속도 제어기는 PI 제어기, 전류 제어기는 PI 제어기로 설계했습니다.  
Anti windup을 추가하여 정상상태오차를 제거하였습니다.  

## System Architecture  

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148331911-cf8d9896-3dc8-4b83-9619-a3062f7e8393.JPG" width="600px"></p>  

### Test Board 구성  

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148332121-c2385eae-b38a-440b-ae25-9e10ac851284.JPG" width="500px"></p>  

### 회로도 구성  

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148332175-38b322eb-aa90-4af7-9b30-a1961dc5d073.JPG" width="700px"></p>  

## 결과 영상  

### Target Position을 90 degree, Target Velocity를 10, Target Torque를 0.1로 설정했을 때 결과 

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148335226-193625d2-58ca-44bc-aaf3-8b1b16b87227.gif" width="600px"></p>  
<p align="center"> < MFC(UI) 및 NTGraph 화면 ></p>  

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148335941-33d2686e-392c-424a-8d09-4ff0b8cb0b91.gif" width="360px"></p>  
<p align="center"> < 실제 Motor 움직임 (x2) ></p>  

  
### Target Position을 -90 degree, Target Velocity를 50, Target Torque를 0.1로 설정했을 때 결과 

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148336697-282952f4-194a-4900-b91e-ea20070bc1be.gif" width="600px"></p>  
<p align="center"> < MFC(UI) 및 NTGraph 화면 ></p> 
  
<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148336382-5069344c-aadc-4177-b8ee-533a0a6d8e77.gif" width="360px"></p>  
<p align="center"> < 실제 Motor 움직임 (x2) ></p>  
  

### 위와 동일한 Position, Velocity일 때 Target Torque를 0.05, 0.2로 설정했을 때 결과

<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148337243-bc05e5f9-be37-4ba9-81dd-84eb1465d2b3.gif" width="600px"></p>  
<p align="center"> < MFC(UI) 및 NTGraph 화면 (x2) ></p> 
  
<p align="center"><img src="https://user-images.githubusercontent.com/41863759/148337468-7014612c-7672-4783-ad0f-ce95d2850a5c.gif" width="360px"></p>  
<p align="center"> < 실제 Motor 움직임 (x2) ></p>  
