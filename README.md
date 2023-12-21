![header](https://capsule-render.vercel.app/api?type=rect&color=auto&height=400&section=header&text=간이%20자동차%20프로젝트&fontSize=60&animation=twinkling&desc=임베디드시스템%204조%20기말%20프로젝트)
<br/>

### Contributors

|프로필|학번|역할|
|:---:|:---:|:---|
|<a href="https://github.com/Dyoya/"><img src="https://avatars.githubusercontent.com/u/118094189?v=4" width="100px;" alt="Dyoya"/><br /><sub><b>Dyoya (김형민)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=Dyoya" title="Commit Log">💻</a>|**20190348**|- 수위 센서 코드 제작<br>- 안드로이드 앱 제작<br>- 코드 종합 및 메이크파일 제작<br>- 리드미 종합 및 정리<br>- 중간 발표|
|<a href="https://github.com/ParkSGye/"><img src="https://avatars.githubusercontent.com/u/113571828?v=4" width="100px;" alt="ParkSGye"/><br /><sub><b>ParkSGye (박성규)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=ParkSGye" title="Commit Log">💻</a>|**20190452**|- 수위 센서 코드 제작<br>- 온습도 센서 코드 제작<br>- 부저 사용 코드 제작|
|<a href="https://github.com/user71309876/"><img src="https://avatars.githubusercontent.com/u/93814905?v=4" width="100px;" alt="user71309876"/><br /><sub><b>user71309876 (박재현)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=user71309876" title="Commit Log">💻</a>|**20180474**|- NFC 코드 제작<br>- Curl 라이브러리 사용<br>- DB 및 서버 제작<br>- 최종 발표|
|<a href="https://github.com/dwgns/"><img src="https://avatars.githubusercontent.com/u/67849875?v=4" width="100px;" alt="dwgns"/><br /><sub><b>dwgns (안지훈)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=dwgns" title="Commit Log">💻</a>|**20190685**|- NFC 코드 제작<br>- 가스 센서 코드 제작<br>- 최종 발표 자료 제작|

# List
1. [아이디어 소개](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#아이디어-소개)
2. [전체 시스템 구조](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#전체-시스템-구조)
3. [회로 구조도](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#회로-구조도)
5. [제한 조건](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#제한-조건)
6. [가산점 요소](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#가산점-요소)
7. [구현 내용](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#구현-내용)
8. [실행 방법](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#실행-방법)
9. [사용법](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#사용법)
10. [개발 일정](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#개발-일정)
11. [데모 영상](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#데모-영상)
12. [참고문헌](https://github.com/Dyoya/EmbeddedProject?tab=readme-ov-file#참고문헌)

# 아이디어 소개
 **간이 자동차 프로젝트**는 차량의 여러 상황변화(ex.온도, 차량내부 수위, 가스 농도, NFC read)를 감지하여 확보한 데이터를 사용자의 스마트폰 어플에 송신하는 프로그램이다. 데이터에서 과도한 변화가 생길 경우 프로그램은 사용자의 스마트폰에 알람을 발생시키며, NFC 리더기에 타 사용자의 카드키(혹은 리더기에 처음 등록하는 카드키)에 대해 차량 내부 부저로 경고를 울리며 사용자의 스마트폰에 경고 신호를 보낸다.

# 전체 시스템 구조
 **온습도 센서**에서 온도, **수위 센서**, **가스 센서**, **NFC 리더기**가 데이터를 읽어 라즈베리 파이가 받는다. 라즈베리 파이는 해당 데이터를 하나의 **JSON 형식**으로 종합하여 서버로 전송한다. 서버의 **DB**에 값이 저장되며, 스마트폰의 **안드로이드 앱**은 서버에서 데이터를 읽어 사용자에게 보여준다. NFC가 처음 인식된 경우는 사용자가 스마트폰을 통해 **경고 신호**를 서버로 보낼 수 있다. 라즈베리 파이는 서버로부터 경고 신호를 받아 부저를 울려 경고할 수 있다.
 
![시스템 구조도](./img/시스템구조도.jpg)

# 회로 구조도
 **온습도 센서**는 읽은 데이터를 디지털 신호로 전송할 수 있다. 그래서, 라즈베리 파이의 GPIO 핀에 직접 연결했다. **수위 센서**는 아날로그 신호를 전송하기 때문에 ADC 컨버터를 사용해 라즈베리 파이에 연결했다. **가스 센서**는 디지털 신호를 전송할 수 있지만, 디지털 신호는 0과 1로 가스 감지 여부만이 있기 때문에 아날로그 신호를 ADC 컨버터를 이용해 라즈베리 파이와 연결했다. **NFC 리더기**는 UART 통신을 사용하므로 라즈베리 파이의 TX, RX핀과 연결했다.
 
![회로 구조도](./img/회로구조도.jpg)

# 제한 조건
## Thread
4가지 센서를 축정하는 함수와 측정된 센서값을 서버로 전달하는 함수를 구현하였다. 그리고 스레드를 이용하여 병렬적으로 값을 측정하고 서버에 전달하도록 하였다.
```c
    //main.c
    temperature = pthread_create(&temperature, NULL, temperatureSensorFun, NULL);
    gas = pthread_create(&gas, NULL, gasSensorFun, NULL);
    water = pthread_create(&water, NULL, waterSensorFun, NULL);
    nfc = pthread_create(&nfc, NULL, NFCReaderFun, NULL);
    send = pthread_create(&send, NULL, dataToServer, NULL);
```


## Mutex
각 센서마다 공유전역변수를 사용하여 각 센서 측정 함수가 측정값을 변수에 갱신하도록 한다.
```c
        //gas.c
        pthread_mutex_lock(&mutex); // 뮤텍스 잠금
        printf("gas : %d\n", gas_data);
        gasValue = gas_data;
        delay(100);
        share_var = 2;
        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
```

# 가산점 요소
> 서버를 통한 라즈베리 파이와 스마트폰 통신
> 
> 다른 수업의 내용과 병합한 서버 개발 및 안드로이드 앱 개발

# 구현 내용
## 1. RasberryPi
 헤더 파일 **common.h**를 만들어 여러 코드를 하나로 합쳤다. common.h는 공유 변수 및 함수가 선언되어 있고, common.c에는 정의되어 있다. 
 
- **temperature.c**  
 온습도 센서를 사용하는 temperatureSensorFun(void*) 쓰레드 함수가 있는 코드다. 온습도 센서 온도와 습도를 측정할 수 있으며, 데이터는 배열 형태로 받아온다. dht11_val[0]과 dht11_val[1]은 차례대로 습도의 정수 부분과 소수 부분이며, dht11_val[2]와 dht11_val[3]은 온도의 정수 부분과 소수 부분이다. 습도 측정은 사용하지 않으므로 dht11_val[2]와 dht11_val[3]을 sprintf() 함수를 통해 '%d.%d' 형태의 문자열로 만들고, strtof() 함수를 통해 다시 소수 형태로 변환하여 전송했다.

- **water.c**  
  수위 센서를 사용하는 waterSensorFun(void*) 쓰레드 함수가 있는 코드다. 수위 센서는 아날로그 신호로 출력하므로 디지털 신호로 변환하기 위한 ADC 컨버터가 필요하다. 컨버터의 AOUT2에 연결을 하므로 adcChannel를 2로 설정하였다. wiringPiI2CWrite() 함수를 통해 값을 읽게 되는데, 처음엔 이전 값을 읽고 두 번째는 현재 값을 읽게 된다. 그래서, 값을 전송하기 전에 wiringPiI2CWrite()를 2번 사용하고 해당 값을 전송한다.

- **gas.c**  
 가스 센서를 사용하는 gasSensorFun(void*) 쓰레드 함수가 있는 코드다. 가스 센서는 아날로그 신호와 디지털 신호 둘 다 출력할 수 있다. 디지털 신호는 가스 센서가 가스를 감지했으면 1, 아니면 0을 출력하기 때문에 아날로그 신호를 ADC 컨버터를 사용해 라즈베리 파이의 핀에 연결했다. 수위 센서와 같이 wiringPiI2CWrite() 함수를 두 번 사용해 두 번째 읽은 값을 전송한다.

- **nfc.c**  
 nfc 리더기에 대한 NFCReaderFun(void*) 쓰레드 함수가 있는 코드다. nfc에 대한 코드는 오픈소스를 많이 사용했다. [PN532 NFC Library](https://github.com/soonuse/pn532-lib) 해당 오픈소스의 코드를 사용해 UART 통신 방식으로 값을 읽어오고, 문자열 형태로 값을 전송한다.

- **buzzer.c**
 서버에서 경고 신호가 발생했을 경우, 부저를 울리는 쓰레드 함수가 있는 코드다. 다른 쓰레드와는 별개로 mutex를 사용하지 않고 작동되는 쓰레드며, 경고 신호가 발생한 경우 부저를 울리게 된다.

- **main.c**  
  모든 센서는 쓰레드로 동작한다. mutex를 사용해 모든 쓰레드에게서 차례대로 값을 받아오고, 해당 데이터를 JSON 형태로 만들어 Curl을 사용해 DB로 전송한다.

- **makefile**  
  소스 코드 파일을 모두 분리하였기 때문에 makefile을 만들었다. 컴파일러를 설정하고, 옵션과 소스 파일, 헤더 파일 목록을 입력하고, 오브젝트 파일 및 빌드 규칙은 매크로를 사용했다. 아래 코드는 makefile의 일부다. 

```
# 오브젝트 파일 목록
OBJS = $(SRCS:.c=.o)

# 빌드 규칙
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

%.o: %.c $(HDRS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
```


## 2. Server
 데이터베이스의 4가지 센서값을 받아들일 수 있게 temperature, water, gas, nfc의 속성으로 구성했다.

![db desc](./img/db.png)

아래는 센서값을 DB에 저장하는 코드다. 라즈베리파이는 CURL을 이용하여 호스트 주소에 접근하여 센서값을 POST 방식으로 서버에 전달한다. 그리고 서버는 받은 값을 이용하여 DB를 갱신한다. 센서값은 가장 최신의 값만 유효하다는 특징이 있어 튜플을 누적시키지 않고 1열의 튜플값을 갱신시키도록 했다.

```javascript
app.post("/sensor", (req, res) => {
  const sql = "update sensor set temperature=?, water=?, gas=?, nfc=?";
  var params = [
    req.body.temperature,
    req.body.water,
    req.body.gas,
    req.body.nfc,
  ];
  console.log(params);
  db.query(sql, params, (err, rows, fields) => {
    console.log(rows);
    if (err) {
      res.json({ result: "error" });
      return console.log(err);
    }
    res.json(rows);
  });
});
```

아래는 앱에서 센서의 값을 받아오는 코드다. 앱은 Retrofit을 사용하여 GET 방식으로 값을 받아온다.

```javascript
app.get("/sensor", (req, res) => {
  const sql = "select * from sensor";
  db.query(sql, (err, rows) => {
    if (err) {
      res.json({ result: "error" });
      return console.log(err);
    }
    res.json(rows);
  });
});
```

아래는 라즈베리파이에서 알람 flag값을 받아오는 코드다. 알람 flag값이 true이면 알람을 울리라는 json 형식의 데이터를 보낸다.

```javascript
app.get("/buzzer", (req, res) => {
  if (result) {
    res.json({ warning: "noise" });
    result = false;
  } else {
    res.json({ warning: "quiet" });
  }
  console.log(result);
});
```

아래는 앱에서 유효하지 않은 NFC가 접근했을 때 알람 flag값을 변경하는 코드다. 알람 flag값이 true이면 부저를 울리고 false이면 부저를 울리지 않는다.

```javascript
app.get("/warning", (req, res) => {
  if (result == false) {
    result = true;
    res.json({ warning: "Get Ok" });
    console.log(result);
  }
});
```


## 3. Android App
  안드로이드 앱은 서버로부터 데이터를 받아와 화면에 출력하며, 계속 갱신되어야 한다. 먼저, 화면에 어떻게 보여줄지 디자인했다. 4개의 영역을 만들어 측정값에 따라 해당 영역의 배경 색이 변하도록 했다. 센서의 측정 값이 경고 수준이 되면, 앱이 푸시 알림을 발생하도록 했다. NFC 리스트를 만들어 해당 리스트에 없는 NFC 태그가 인식되었을 경우엔 팝업을 띄우도록 했다. 팝업에는 등록, 취소, 경고 버튼이 있다. 경고 버튼을 누를 경우엔 서버에 경고 신호가 전송되도록 했으며, 등록 버튼을 누르면 리스트에 해당 NFC의 태그가 등록된다. 취소 버튼을 누를 경우엔 아무 것도 작동하지 않는다. 위 기능은 [MainActivity.kt](https://github.com/Dyoya/EmbeddedProject/blob/main/AndroidApp/app/src/main/java/kr/ac/kumoh/ce/s20190348/AndroidAppProject/MainActivity.kt)에서 확인할 수 있다.  
  
  [SensorViewModel.kt](https://github.com/Dyoya/EmbeddedProject/blob/main/AndroidApp/app/src/main/java/kr/ac/kumoh/ce/s20190348/AndroidAppProject/SensorViewModel.kt)는 서버에서 데이터를 받아오는 역할을 한다. URL을 통해 데이터베이스에 접근하여 데이터를 갱신할 수 있도록 한다.  

  [SensorApi.kt](https://github.com/Dyoya/EmbeddedProject/blob/main/AndroidApp/app/src/main/java/kr/ac/kumoh/ce/s20190348/AndroidAppProject/SensorApi.kt)는 서버로부터 GET을 하는 코드며, [Sensor.kt](https://github.com/Dyoya/EmbeddedProject/blob/main/AndroidApp/app/src/main/java/kr/ac/kumoh/ce/s20190348/AndroidAppProject/Sensor.kt)는 센서 데이터의 데이터 포멧이 있는 코드다.  

  안드로이드 앱 작동 화면은 아래와 같다.

  ![앱 작동 화면](./img/앱화면.jpg)



# 실행 방법
 ### 라즈베리파이 환경
  **1. wiringPi 설치**
     
  ```
  git clone https://github.com/WiringPi/WiringPi.git
  cd WiringPi
  git pull origin
  ./bulid
  ```

   **2. curl 설치**

  ```
  sudo apt update
  sudo apt install -y curl
  ```

  **3. 실행 파일 빌드**
  
  ```
  cd RaspberryPi && make
  ```

  **4. 파일 실행**
  
  ```
  ./myprogram
  ```

### 서버 환경
  **1. DB 배포**
     [[MariaDB] MariaDB 클라우드타입에 배포하고 sequelize와 연결하기](https://velog.io/@dngur9801/MariaDB-MariaDB-%ED%81%B4%EB%9D%BC%EC%9A%B0%EB%93%9C%ED%83%80%EC%9E%85%EC%97%90-%EB%B0%B0%ED%8F%AC%ED%95%98%EA%B3%A0-sequelize%EC%99%80-%EC%97%B0%EA%B2%B0%ED%95%98%EA%B8%B0)
     
  - [mysql.sql](./Server/mysql.sql)의 내용을 바탕으로 DB구성
  
  **2. node.js 설치**  
     [Node.js 홈페이지](https://nodejs.org/en/download)
     
  **3. 서버 실행**
  - [server.js](./Server/server.js)의 내용 수정
  -  **host**, **port**, **password**를 수정
  ```javascript
const db = mysql.createConnection({
  host: "svc.sel4.cloudtype.app",
  port: 30640,
  user: "root",
  password: "mysql1234",
  database: "sensor",
});
```
  - 서버 실행
  ```
  cd Server node && server.js
  ```


# 사용법
이 프로젝트는 다음 4가지 주요 센서를 사용하여 측정된 값을 사용한다.

### 1. NFC 리더
  NFC 카드를 리더기에 태그한다. 태그 시 스마트폰 어플리케이션에서 알람이 발생하며 등록이나 취소, 경고를 선택할 수 있다. 또한 경고 선택 시 부저를 통한 청각적 피드백이 발생한다.
### 2. 온도 센서
  주기적으로 온도를 측정한다. 측정된 값은 스마트폰 어플리케이션의 온도 센서 항목을 통해 확인할 수 있다.
### 3. 수위 센서
  주기적으로 수위를 측정한다. 마찬가지로 스마트폰 어플리케이션의 수위 센서 항목을 통해 확인할 수 있다.
### 4. 가스 센서
  주기적으로 가스 농도를 측정한다. 위와 동일하다.

측정된 정보는 데이터베이스에 저장되며 저장된 정보는 스마트폰 어플리케이션을 통해 확인할 수 있다. 따라서 프로그램이 작동하기 위해서는 데이터베이스 서버 구동 및 스마트폰 어플리케이션 설치를 요구한다.
NFC를 제외한 나머지 센서들의 경우 특이사항(비정상적으로 높은 온도 등) 발생 시 스마트폰 어플리케이션 화면에서 각 항목에 대해 빨간색으로 강조 처리되고 백그라운드 경고 알림을 발생시킨다.

아래는 스마트폰 어플리케이션 사용 예시이다.

![image](https://github.com/Dyoya/EmbeddedProject/assets/67849875/c53da1c3-f104-476a-814d-b17b7d85913b)


# 개발 일정
 아래 사진은 11월 30일부터 12월 15일까지의 개발 일정이다. 초록색 영역은 기존에 계획했던 부분이고 대부분 맞춰서 개발되었다. 주황색 영역은 기간 안에 완성하지 못하여 추가로 소요된 기간을 표시한 것이다.

![개발 일정](./img/개발일정2.jpg.png)

# 데모 영상
[센서 테스트 데모](https://youtu.be/WD6pnmGQc7k)  
[푸시 알림 데모](https://youtube.com/shorts/I-0o1dvdy5E?feature=share)


# 참고문헌
[가스 센서(MQ-5)](https://m.blog.naver.com/elepartsblog/221528526035)

[MCP3208 ADC 컨버터 사용하기](https://diymaker.tistory.com/155)

[MCP3008 + 수위센서](https://blog.naver.com/PostView.naver?blogId=roboholic84&logNo=220416082742)

[NFC Reader(PN532)](https://m.blog.naver.com/tiled12/221794166065)

[온습도 센서](https://blog.naver.com/PostView.naver?blogId=dokkosam&logNo=222162911335)

[CURL 명령어 사용법 정리](https://inpa.tistory.com/entry/LINUX-%F0%9F%93%9A-CURL-%EB%AA%85%EB%A0%B9%EC%96%B4-%EC%82%AC%EC%9A%A9%EB%B2%95-%EB%8B%A4%EC%96%91%ED%95%9C-%EC%98%88%EC%A0%9C%EB%A1%9C-%EC%A0%95%EB%A6%AC)
