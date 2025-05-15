# Amazing Reviewer Team SSD 프로젝트 입니다.

## 구현내용 참고사항
1. Test Script 를 통한 검증은 Test Shell Command 를 기준이 아닌, SSD Command 기준 작성하였습니다. SSD 명령어 기준으로 ERROR 유무를 확인하고 Script/Scenario PASS/FAIL을 결정합니다.
2. cmd_buffer.txt는 Command buffer의 내부 context 내용을 기입하는 것으로 실제 buf 폴더에 저장하는 0_E_0_3, 1_W_0_0xBEEFCAFE 와 내용이 같다.
처음에 buffer 폴더 내부의 파일명이 단지 채점을 위한 Command Buffer 내용 확인용이라 생각했으나, 개발 후반부에 이 buf 폴더 내부의 파일명이 
결국 Commad Buffer의 context를 계속 유지 시켜줄 수 있는 내부 데이터용도로도 사용되어야 했었다. 
처음에 buf내 파일명을 그냥 Command Buffer의 cout 정도로 생각 했으나 이는 잘못된 길로 나를 인도하였다.
3. buf 내 파일명은 모두 command buffer내 유효한 커맨드를 의미한다. 비록 Erase와 Write 파일의 lba가 겹치더라도, 이는 Erase가 우선한다. 즉 해당 erase보다 write가 뒤에 들어 온 것으로 유효하다.
Write에의한 write ignore, erase에 의한 write ignore, erase에 의한 erase ignore혹은 merge는 구현되어 있다.
---
<img src ="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQEFq__WA1GsvqVFhNqU_Hx0h1J424IeyClmQ&s" width=120></img>
<img src="https://lettering.org/wp-content/uploads/2023/11/t.jpg" width=120> </img>
<img src = "https://scontent-ssn1-1.xx.fbcdn.net/v/t39.30808-6/299799697_455431886597333_3598393631017228322_n.png?stp=dst-png_s960x960&_nc_cat=109&ccb=1-7&_nc_sid=cc71e4&_nc_ohc=zxMxCidHWGIQ7kNvwHDeWS8&_nc_oc=AdmyR1OD0ZZ_7llG4VjIu2VmhpHSo-HWLvsi5Q0ddIZ9pL8mJcvK818e4koNA535_mg&_nc_zt=23&_nc_ht=scontent-ssn1-1.xx&_nc_gid=0MmvMeTbxzcXbhRmGsTobw&oh=00_AfJ10_tfY_eySS6u1NO0fQWPdlKhY8atIMwoO6QL3wyong&oe=68232FF7" height=120></img>

<img src="https://m.media-amazon.com/images/I/61dvFjB0lJL._AC_UF894,1000_QL80_.jpg" width=360> </img>

