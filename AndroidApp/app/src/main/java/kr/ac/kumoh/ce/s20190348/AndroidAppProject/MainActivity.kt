package kr.ac.kumoh.ce.s20190348.AndroidAppProject

import android.annotation.SuppressLint
import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.media.RingtoneManager
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.core.app.NotificationCompat
import androidx.core.app.NotificationManagerCompat
import androidx.core.os.postDelayed
import coil.compose.AsyncImage
import kr.ac.kumoh.ce.s20190348.AndroidAppProject.ui.theme.AndroidAppTheme


class MainActivity : ComponentActivity() {
    private val viewModel: SensorViewModel by viewModels()

    private val handler = Handler(Looper.getMainLooper())

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // 초기 데이터 가져오기
        val initialSensorList = createRandomSensorList(1)
        var sensorList by mutableStateOf(initialSensorList)

        // Compose 화면 설정
        setContent {
            MainScreen(sensorList)
        }

        // 2초마다 데이터 가져와서 업데이트
        handler.postDelayed(object : Runnable {
            override fun run() {
                // 새로운 랜덤 데이터 생성
                val newSensorList = createRandomSensorList(1)

                viewModel.fetchData()
                //checkSensorStatus(viewModel.sensorList.value)
                checkSensorStatus(newSensorList)
                Log.d("MainActivity", "checkSensorStatus called after fetching data")

                // 상태 갱신
                sensorList = newSensorList

                // 반복 실행
                handler.postDelayed(this, 2000)
            }
        }, 2000)

        createNotificationChannel()
    }
    private fun createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val name = "SensorNotificationChannel"
            val descriptionText = "Channel for sensor notifications"
            val importance = NotificationManager.IMPORTANCE_DEFAULT
            val channel = NotificationChannel("sensor_channel", name, importance).apply {
                description = descriptionText
            }

            val notificationManager: NotificationManager =
                getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            notificationManager.createNotificationChannel(channel)
        }
    }

    private fun checkSensorStatus(sensorList: List<Sensor>?) {
        sensorList?.let {
            val colorList = dangerColor(sensorList[0])

            // Check for red color indicating danger
            if (colorList.contains(Color(255, 0, 0, 100))) {
                sendNotification("!!!차량 위험!!!", "차량에 위험 신호가 발견됨!")
            }
        }
    }

    private fun sendNotification(title: String, message: String) {
        Log.d("MainActivity", "Sending notification - Title: $title, Message: $message")

        try {
            val builder = NotificationCompat.Builder(this, "sensor_channel")
                .setSmallIcon(android.R.drawable.ic_dialog_alert)
                .setContentTitle(title)
                .setContentText(message)
                .setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION))
                .setWhen(System.currentTimeMillis())
                .setAutoCancel(true)

            with(NotificationManagerCompat.from(this)) {
                notify(1, builder.build())
            }
        } catch (e: SecurityException) {
            Log.d("MainActivity", "알림 오류")
        }
    }
}

@Composable
//fun MainScreen(viewModel: SensorViewModel) {
fun MainScreen(sensorList: List<Sensor>) {
    //val sensorList by viewModel.sensorList.observeAsState(emptyList())

    AndroidAppTheme {
        Surface(modifier = Modifier.fillMaxSize(), color = MaterialTheme.colorScheme.background) {
            DisplaySensor(sensorList)
        }
    }
}

@Composable
fun DisplaySensor(sensor: List<Sensor>)
{
    val colorList = dangerColor(sensor[0]) // 임시

    val tem =  sensor[0].temperature
    val water = sensor[0].water
    val gas = sensor[0].gas
    val nfc = sensor[0].nfc

    Column (
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ){
        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[0])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = R.drawable.image_temperature,
                contentDescription = "온도 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("온도 센서")
                DataText("${tem}")
            }
        }

        Spacer(modifier = Modifier.height(10.dp))

        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[1])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = R.drawable.image_water,
                contentDescription = "수위 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("수위 센서")
                DataText("${water}")
            }
        }

        Spacer(modifier = Modifier.height(10.dp))

        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[2])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = R.drawable.image_gas,
                contentDescription = "가스 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("가스 센서")
                DataText("${gas}")
            }
        }

        Spacer(modifier = Modifier.height(10.dp))

        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[3])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = R.drawable.image_nfc,
                contentDescription = "NFC 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))

            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("NFC")
                DataText("${nfc}")
            }
        }
    }
}

@Composable
fun TitleText(title: String)
{
    Text(title, fontSize = 30.sp)
}

@Composable
fun DataText(data: String) {
    Text(data, fontSize = 16.sp)
}

fun dangerColor(sensor: Sensor): MutableList<Color> {
    val danger = Color(255, 0, 0, 100) // 위험
    val red = Color(255, 0, 0, 50) // 경고
    val yellow = Color(255, 255, 0, 50) // 주의
    val green = Color(0, 255, 0, 50) // 안전

    // 초기화
    val colorList = mutableListOf<Color>().apply {
        repeat(4) { add(Color(0, 0, 0, 0)) }
    }


    // 온도 센서
    if(sensor.temperature < -5 || sensor.temperature > 70)
        colorList[0] = danger
    else if(sensor.temperature < 0 || sensor.temperature > 60)
        colorList[0] = red
    else if (sensor.temperature < 5 || sensor.temperature > 40)
        colorList[0] = yellow
    else
        colorList[0] = green

    // 수위 센서
    if(sensor.water > 700)
        colorList[1] = danger
    else if(sensor.water > 500)
        colorList[1] = red
    else if (sensor.water > 300)
        colorList[1] = yellow
    else
        colorList[1] = green

    // 가스 센서
    if(sensor.temperature > 50)
        colorList[2] = danger
    else if(sensor.temperature > 30)
        colorList[2] = red
    else if (sensor.temperature > 10)
        colorList[2] = yellow
    else
        colorList[2] = green

    // NFC


    return colorList
}

// 임시 데이터 생성 함수
fun createRandomSensorList(size: Int): List<Sensor> {
    val random = java.util.Random()
    val sensorList = mutableListOf<Sensor>()

    repeat(size) {
        val temperature = random.nextInt(100) // 임의의 온도 데이터 (0부터 99까지의 난수)
        val water = random.nextInt(1000) // 임의의 수위 데이터 (0부터 999까지의 난수)
        val gas = random.nextInt(100) // 임의의 가스 데이터 (0부터 99까지의 난수)
        val nfc = "NFC_${random.nextInt(100)}" // 임의의 NFC 데이터 ("NFC_0"부터 "NFC_99"까지의 문자열)

        val sensor = Sensor(temperature, water, gas, nfc)
        sensorList.add(sensor)
    }

    return sensorList
}