![header](https://capsule-render.vercel.app/api?type=rect&color=auto&height=400&section=header&text=간이%20자동차%20프로젝트&fontSize=60&animation=twinkling&desc=임베디드시스템%204조%20기말%20프로젝트)
<br/>

### Contributors

|프로필|학번|역할|
|:---:|:---:|:---|
|<a href="https://github.com/Dyoya/"><img src="https://avatars.githubusercontent.com/u/118094189?v=4" width="100px;" alt="Dyoya"/><br /><sub><b>Dyoya (김형민)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=Dyoya" title="Commit Log">💻</a>|20190348|- 역할<br>- 역할|
|<a href="https://github.com/ParkSGye/"><img src="https://avatars.githubusercontent.com/u/113571828?v=4" width="100px;" alt="ParkSGye"/><br /><sub><b>ParkSGye (박성규)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=ParkSGye" title="Commit Log">💻</a>|20190452|- 역할<br>- 역할|
|<a href="https://github.com/user71309876/"><img src="https://avatars.githubusercontent.com/u/93814905?v=4" width="100px;" alt="user71309876"/><br /><sub><b>user71309876 (박재현)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=user71309876" title="Commit Log">💻</a>|20180474|- 역할<br>- 역할|
|<a href="https://github.com/dwgns/"><img src="https://avatars.githubusercontent.com/u/67849875?v=4" width="100px;" alt="dwgns"/><br /><sub><b>dwgns (안지훈)</b></sub></a><br /> <a href="https://github.com/Dyoya/EmbeddedProject/commits?author=dwgns" title="Commit Log">💻</a>|20190685|- 역할<br>- 역할|

# List
1. [아이디어 소개](https://github.com/Dyoya/EmbeddedProject#아이디어-소개)
2. [전체 시스템 구조](https://github.com/Dyoya/EmbeddedProject#전체-시스템-구조)
3. [회로 구조도](https://github.com/Dyoya/EmbeddedProject#회로-구조도)
5. [제한 조건](https://github.com/Dyoya/EmbeddedProject#제한-조건)
6. [가산점 요소](https://github.com/Dyoya/EmbeddedProject#가산점-요소)
7. [구현 내용](https://github.com/Dyoya/EmbeddedProject#구현-내용)
8. [실행 방법](https://github.com/Dyoya/EmbeddedProject#실행-방법)
9. [사용법](https://github.com/Dyoya/EmbeddedProject#사용법)
10. [개발 일정](https://github.com/Dyoya/EmbeddedProject#개발-일정)
11. [참고문헌](https://github.com/Dyoya/EmbeddedProject#참고문헌)

# 아이디어 소개
 내용내용

# 전체 시스템 구조
 **온습도 센서**에서 온도, **수위 센서**, **가스 센서**, **NFC 리더기**가 데이터를 읽어 라즈베리 파이가 받는다. 라즈베리 파이는 해당 데이터를 하나의 **JSON 형식**으로 종합하여 서버로 전송한다. 서버의 **DB**에 값이 저장되며, 스마트폰의 **안드로이드 앱**은 서버에서 데이터를 읽어 사용자에게 보여준다. NFC가 처음 인식된 경우는 사용자가 스마트폰을 통해 **경고 신호**를 서버로 보낼 수 있다. 라즈베리 파이는 서버로부터 경고 신호를 받아 부저를 울려 경고할 수 있다.
 
![시스템 구조도](./img/시스템구조도.jpg)

# 회로 구조도
 **온습도 센서**는 읽은 데이터를 디지털 신호로 전송할 수 있다. 그래서, 라즈베리 파이의 GPIO 핀에 직접 연결했다. **수위 센서**는 아날로그 신호를 전송하기 때문에 ADC 컨버터를 사용해 라즈베리 파이에 연결했다. **가스 센서**는 디지털 신호를 전송할 수 있지만, 디지털 신호는 0과 1로 가스 감지 여부만이 있기 때문에 아날로그 신호를 ADC 컨버터를 이용해 라즈베리 파이와 연결했다. **NFC 리더기**는 UART 통신을 사용하므로 라즈베리 파이의 TX, RX핀과 연결했다.
 
![회로 구조도](./img/회로구조도.jpg)

# 제한 조건
## Thread
 내용내용

## Mutex
 내용내용

# 가산점 요소
> 서버를 통한 라즈베리 파이와 스마트폰 통신
> 
> 다른 수업의 내용과 병합한 서버 개발 및 안드로이드 앱 개발

# 구현 내용
## 1. RasberryPi
 헤더 파일 **common.h**를 만들어 여러 코드를 하나로 합쳤다. common.h는 공유 변수 및 함수가 선언되어 있고, common.c에는 정의되어 있다. 
 
- 

## 2. Server
 데이터베이스의 4가지 센서값을 받아들일 수 있게 temperature, water, gas, nfc의 속성으로 구성했다.

[그림]

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
 내용내용
 
## 4. Makefile
 내용내용

# 실행 방법
 내용내용


# 사용법
 내용내용


# 개발 일정
 아래 사진은 11월 30일부터 12월 15일까지의 개발 일정이다.

![개발 일정](./img/개발일정.jpg)


# 참고문헌
[가스 센서(MQ-5)](https://m.blog.naver.com/elepartsblog/221528526035)

[MCP3208 ADC 컨버터 사용하기](https://diymaker.tistory.com/155)

[MCP3008 + 수위센서](https://blog.naver.com/PostView.naver?blogId=roboholic84&logNo=220416082742)

[NFC Reader(PN532)](https://m.blog.naver.com/tiled12/221794166065)

[온습도 센서](https://blog.naver.com/PostView.naver?blogId=dokkosam&logNo=222162911335)

[CURL 명령어 사용법 정리](https://inpa.tistory.com/entry/LINUX-%F0%9F%93%9A-CURL-%EB%AA%85%EB%A0%B9%EC%96%B4-%EC%82%AC%EC%9A%A9%EB%B2%95-%EB%8B%A4%EC%96%91%ED%95%9C-%EC%98%88%EC%A0%9C%EB%A1%9C-%EC%A0%95%EB%A6%AC)
