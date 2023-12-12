package kr.ac.kumoh.ce.s20190348.AndroidAppProject

import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.support.v4.app.NotificationCompat
import android.support.v4.app.NotificationManagerCompat
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
import androidx.core.os.postDelayed
import coil.compose.AsyncImage
import kr.ac.kumoh.ce.s20190348.AndroidAppProject.ui.theme.AndroidAppTheme


class MainActivity : ComponentActivity() {
    private val viewModel: SensorViewModel by viewModels()

    private val handler = Handler(Looper.getMainLooper())

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        createNotificationChannel()

        setContent {
            MainScreen(viewModel)
        }

        // 2초마다 데이터 가져와서 업데이트
        handler.postDelayed(2000) {
            viewModel.fetchData()
            checkSensorStatus(viewModel.sensorList.value)
        }
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
                sendNotification("Danger", "차량에 위험 신호가 발견됨!")
            }
        }
    }
    private fun sendNotification(title: String, message: String) {
        val builder = NotificationCompat.Builder(this, "sensor_channel")
            .setSmallIcon(androidx.core.R.drawable.notification_bg)
            .setContentTitle(title)
            .setContentText(message)
            .setPriority(NotificationCompat.PRIORITY_DEFAULT)

        with(NotificationManagerCompat.from(this)) {
            notify(1, builder.build())
        }
    }
}

@Composable
fun MainScreen(viewModel: SensorViewModel) {
    val sensorList by viewModel.sensorList.observeAsState(emptyList())

    AndroidAppTheme {
        Surface(modifier = Modifier.fillMaxSize(), color = MaterialTheme.colorScheme.background) {
            DisplaySensor(sensorList)
        }
    }
}

@Composable
fun DisplaySensor(sensor: List<Sensor>)
{
    var colorList = dangerColor(sensor[0]) // 임시

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
                model = "https://picsum.photos/300/300?random=$1",
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
                model = "https://picsum.photos/300/300?random=$2",
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
                model = "https://picsum.photos/300/300?random=$3",
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
                model = "https://picsum.photos/300/300?random=$4",
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
    var danger = Color(255, 0, 0, 100) // 위험
    var red = Color(255, 0, 0, 50) // 경고
    var yellow = Color(255, 255, 0, 50) // 주의
    var green = Color(0, 255, 0, 50) // 안전

    val colorList = mutableListOf<Color>()
    colorList += Color(0, 0, 0, 0) // 초기화

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